/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "semphr.h"
#include "timers.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
TimerHandle_t timerHandle;
SemaphoreHandle_t mutexHandle;
TaskHandle_t defaultTaskHandle;

extern UART_HandleTypeDef huart1;

uint8_t minutes = 0, seconds = 0;
uint8_t lapMinutes = 0, lapSeconds = 0, lapNumber = 0;
uint8_t LCD_String[16] = { ' ' };

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void TimerCallback(TimerHandle_t handle);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
		StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
		StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
		StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
	*ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
	*ppxIdleTaskStackBuffer = &xIdleStack[0];
	*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
	/* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
		StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize) {
	*ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
	*ppxTimerTaskStackBuffer = &xTimerStack[0];
	*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
	/* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	//mutexHandle = xSemaphoreCreateMutex();
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* definition and creation of defaultTask */

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	xTaskCreate(StartDefaultTask, "StartDefaultTask", 128, NULL, 1,
			&defaultTaskHandle);
	/* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument) {
	/* USER CODE BEGIN StartDefaultTask */
	/* Infinite loop */

	/*timerHandle = xTimerCreate("Timer", 1000 / portTICK_PERIOD_MS, pdTRUE, NULL, TimerCallback);

	for (;;) {
		ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
		xSemaphoreTake(mutexHandle, portMAX_DELAY);

		LCD_Write(INSTRUCTION, LCD_RETURN_HOME_INSTRUCTION);
		for (uint8_t i = 0; i < 16; i++)
			LCD_EnqueueCommand(DATA, LCD_String[i]);

		xSemaphoreGive(mutexHandle);
	}*/

	uint8_t writeAt = 0;
	uint8_t c;

	for (;;)
	{
		HAL_UART_Receive(&huart1, &c, 1, HAL_MAX_DELAY);
		LCD_EnqueueCommand(DATA, c);

		if (++writeAt == 16)
		{
			writeAt = 0;
			LCD_EnqueueCommand(INSTRUCTION, LCD_RETURN_HOME_INSTRUCTION);
		}
	}






	/* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/*void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_6)
	{
		if (xSemaphoreTakeFromISR(mutexHandle, NULL) != pdTRUE)
		{
			for (uint8_t i = 0; i < 16; i++)
				LCD_String[i] = ' ';

			seconds = minutes = 0;
			lapSeconds = lapMinutes = lapNumber = 0;

			LCD_String[1] = '0';
			LCD_String[2] = '0';
			LCD_String[3] = ':';
			LCD_String[4] = '0';
			LCD_String[5] = '0';

			xSemaphoreGiveFromISR(mutexHandle, NULL);
			xTimerResetFromISR(timerHandle, NULL);
			vTaskNotifyGiveFromISR(defaultTaskHandle, NULL);
		}
	}
	else if (GPIO_Pin == GPIO_PIN_7)
	{
		if (xSemaphoreTakeFromISR(mutexHandle, NULL) != pdTRUE)
		{
			LCD_String[7] = 'L';
			LCD_String[8] = '0' + lapNumber;
			LCD_String[9] = ' ';
			LCD_String[10] = '0' + ((minutes - lapMinutes) / 10);
			LCD_String[11] = '0' + ((minutes - lapMinutes) % 10);
			LCD_String[12] = ':';
			LCD_String[13] = '0' + ((seconds - lapSeconds) / 10);
			LCD_String[14] = '0' + ((seconds - lapSeconds) % 10);

			lapMinutes = minutes;
			lapSeconds = seconds;
			lapNumber = (lapNumber + 1) % 10;

			xSemaphoreGiveFromISR(mutexHandle, NULL);
			vTaskNotifyGiveFromISR(defaultTaskHandle, NULL);
		}
	}
}

void TimerCallback(TimerHandle_t handle)
{
	xSemaphoreTake(mutexHandle, portMAX_DELAY);

	seconds++;
	if (seconds >= 60)
	{
		seconds = 0;
		minutes++;
		if (minutes >= 60)
			minutes = 0;
	}

	LCD_String[1] = '0' + (minutes / 10);
	LCD_String[2] = '0' + (minutes % 10);
	LCD_String[3] = ':';
	LCD_String[4] = '0' + (seconds / 10);
	LCD_String[5] = '0' + (seconds % 10);

	xSemaphoreGive(mutexHandle);
	xTaskNotifyGive(defaultTaskHandle);
}*/

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
