#include "lcd.h"

#include "gpio.h"

TaskHandle_t lcdTaskHandle;
QueueHandle_t lcdQueueHandle;

void LCD_Write(LCD_CommandAddress address, LCD_CommandValue value)
{
	GPIOC->ODR = ((address & 0x1) << 5) | ((value >> 4) & 0xF);
	GPIOC->ODR |= LCD_ENABLE_BIT;
	GPIOC->ODR &= ~LCD_ENABLE_BIT;

	GPIOC->ODR = ((address & 0x1) << 5) | ((value >> 0) & 0xF);
	GPIOC->ODR |= LCD_ENABLE_BIT;
	GPIOC->ODR &= ~LCD_ENABLE_BIT;

	vTaskDelay(5 / portMAX_DELAY);
}

void LCD_Init()
{
	lcdQueueHandle = xQueueCreate(128, sizeof(LCD_Command));
	xTaskCreate(LCD_Task, "LCD_Task", 128, NULL, 2, &lcdTaskHandle);
}

void LCD_EnqueueCommand(LCD_CommandAddress address, LCD_CommandValue value)
{
	LCD_Command command = { address, value };
	xQueueSend(lcdQueueHandle, &command, portMAX_DELAY);
}

void LCD_EnqueueCommandFromISR(LCD_CommandAddress address, LCD_CommandValue value, BaseType_t* t)
{
	LCD_Command command = { address, value };
	xQueueSendFromISR(lcdQueueHandle, &command, t);
}

void LCD_Task(void* parameter)
{
	vTaskDelay(50 / portMAX_DELAY);

	GPIOC->ODR = 0x02;
	GPIOC->ODR |= LCD_ENABLE_BIT;
	GPIOC->ODR &= ~LCD_ENABLE_BIT;

	vTaskDelay(5 / portMAX_DELAY);

	LCD_Write(INSTRUCTION,
			LCD_FUNCTION_SET_INSTRUCTION |
			LCD_FUNCTION_SET_4_BIT_INTERFACE |
			LCD_FUNCTION_SET_1_LINE |
			LCD_FUNCTION_SET_5x7_DOTS);

	LCD_Write(INSTRUCTION,
			LCD_CONTROL_INSTRUCTION |
			LCD_CONTROL_DISPLAY_ON);

	LCD_Write(INSTRUCTION,
			LCD_ENTRY_MODE_INSTRUCTION |
			LCD_ENTRY_MODE_INC_ADR);

	LCD_Write(INSTRUCTION,
			LCD_RETURN_HOME_INSTRUCTION);

	LCD_Command command;
	while (1)
	{
		while (xQueueReceive(lcdQueueHandle, &command, portMAX_DELAY) != pdTRUE);
		LCD_Write(command.address, command.value);
	}
}
