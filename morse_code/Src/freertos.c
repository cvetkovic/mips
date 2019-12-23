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
#include "timers.h"
#include "queue.h"

extern UART_HandleTypeDef huart1;
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
TaskHandle_t defaultTaskHandle;
TaskHandle_t morseCodeTaskHandle;

QueueHandle_t queueHandle;
TimerHandle_t timerHandle;
/* USER CODE END Variables */


/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void MorseTask(void * handle);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];
  
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )  
{
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
	queueHandle = xQueueCreate(128, sizeof(uint8_t));
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
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
	xTaskCreate(StartDefaultTask, "StartDefaultTask", 128, NULL, 1, &defaultTaskHandle);
	xTaskCreate(MorseTask, "MorseTask", 128, NULL, 2, &morseCodeTaskHandle);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
typedef enum Command { ON = 1, OFF = 0 } Command;
uint8_t state[4] = {0};

void ApplyBitmask(Command command, uint8_t mask)
{
	if (mask & 0x1)
	{
		if (command == ON)
		{
			state[0] = ON;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
		}
		else
		{
			state[0] = OFF;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
		}
	}

	if (mask & 0x2)
	{
		if (command == ON)
		{
			state[1] = ON;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
		}
		else
		{
			state[1] = OFF;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
		}
	}

	if (mask & 0x4)
	{
		if (command == ON)
		{
			state[2] = ON;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
		}
		else
		{
			state[2] = OFF;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
		}
	}

	if (mask & 0x8)
	{
		if (command == ON)
		{
			state[3] = ON;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
		}
		else
		{
			state[3] = OFF;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
		}
	}
}

/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  uint8_t c[8];

  for(;;)
  {
	  HAL_UART_Receive(&huart1, c, 1, HAL_MAX_DELAY);
	  if (c[0] == 'O')
	  {
		  HAL_UART_Receive(&huart1, &c[1], 2, HAL_MAX_DELAY);

		  if (c[1] == 'N')
		  {
			  // in c[2] is bit mask
			  ApplyBitmask(ON, c[2] - 'a' + 1);
		  }
		  else if (c[1] == 'F')
		  {
			  if (c[2] == 'F')
			  {
				  HAL_UART_Receive(&huart1, c, 1, HAL_MAX_DELAY);

				  // in c[0] is bit mask
				  ApplyBitmask(OFF, c[0] - 'a' + 1);
			  }
		  }
	  }
	  else if (c[0] == '4')
	  {
		  uint8_t writeAt = 0;
		  uint8_t cnt = 0;

		  while(1)
		  {
			  HAL_UART_Receive(&huart1, &c[writeAt], 1, HAL_MAX_DELAY);
			  cnt++;

			  if (c[writeAt++] == '.')
				  break;
		  }

		  for (uint8_t i = 0; i < cnt; i++)
		  {
			  if (c[i] == '0')
			  {
				  if (state[0] == ON)
					  HAL_UART_Transmit(&huart1, "0:UPALJENA", 10, HAL_MAX_DELAY);
				  else
					  HAL_UART_Transmit(&huart1, "0:UGASENA", 9, HAL_MAX_DELAY);
			  }
			  else if (c[i] == '1')
			  {
				  if (state[1] == ON)
					  HAL_UART_Transmit(&huart1, "1:UPALJENA", 10, HAL_MAX_DELAY);
				  else
					  HAL_UART_Transmit(&huart1, "1:UGASENA", 9, HAL_MAX_DELAY);
			  }
			  else if (c[i] == '2')
			  {
				  if (state[2] == ON)
					  HAL_UART_Transmit(&huart1, "2:UPALJENA", 10, HAL_MAX_DELAY);
				  else
					  HAL_UART_Transmit(&huart1, "2:UGASENA", 9, HAL_MAX_DELAY);
			  }
			  else if (c[i] == '3')
			  {
				  if (state[3] == ON)
					  HAL_UART_Transmit(&huart1, "3:UPALJENA", 10, HAL_MAX_DELAY);
				  else
					  HAL_UART_Transmit(&huart1, "3:UGASENA", 9, HAL_MAX_DELAY);
			  }
		  }
	  }
	  else if (c[0] == '7')
	  {
		  HAL_UART_Receive(&huart1, &c[0], 1, HAL_MAX_DELAY);

		  xQueueSend(queueHandle, &c[0], portMAX_DELAY);
	  }
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
uint8_t currentChar = 0;

typedef enum MorseSymbol { DOT, LINE } MorseSymbol;

uint8_t morseCode[][5] = { { DOT, LINE }, { LINE, DOT, DOT, DOT }, { LINE, DOT, LINE, DOT }};
uint8_t morseCodeLength[] = { 2, 3, 4 };

void MorseTask(void * handle)
{
	for (;;)
	{
		while (xQueueReceive(queueHandle, &currentChar, portMAX_DELAY) != pdTRUE);
		currentChar -= 'a';

		for (uint8_t i = 0; i < morseCodeLength[currentChar]; i++)
		{
			if (morseCode[currentChar][i] == DOT)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
				vTaskDelay(500 / portTICK_PERIOD_MS);
			}
			else if (morseCode[currentChar][i] == LINE)
			{
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
				vTaskDelay(1500 / portTICK_PERIOD_MS);
			}

			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
			vTaskDelay(500 / portTICK_PERIOD_MS);
		}
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
