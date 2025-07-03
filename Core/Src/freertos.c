/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "driver_led.h"
#include "oled.h"
#include "bsp_hw269.h"
#include "driver_timer.h"
#include "driver_active_buzzer.h"
#include "driver_esp8266.h"
#include "timestamp.h"
#include "usart.h"
#include "key.h"
#include "tim.h"

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
static StackType_t g_puStackoflightTask[128];
static StaticTask_t g_TCBoflightTask;

osThreadId_t encoderTaskHandle;
const osThreadAttr_t encoderTask_attributes = {
  .name = "encoderTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
struct taskprintinfo{
  uint8_t x;
  uint8_t y;
  char name[16];
};

static struct taskprintinfo g_task1printinfo = {0,0,"task1"};
static struct taskprintinfo g_task2printinfo = {0,3,"task2"};
static struct taskprintinfo g_task3printinfo = {0,6,"task3"};
static uint8_t Oled_flag = 0;

void EncoderTask(void *argument);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
      
  OLED_Init();
  OLED_Clear();
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  
  encoderTaskHandle = osThreadNew(EncoderTask, NULL, &encoderTask_attributes);

  /* add threads, ... */
  /**/
  //xTaskCreate(ActiveBuzzer_Test, "playmusic", 128, NULL, osPriorityNormal, &xsoundtaskhandle);

  /*ssssssss*/
  //xlighttaskhandle = xTaskCreateStatic(Led_Test, "lighttask", 128, NULL, osPriorityNormal, g_puStackoflightTask,&g_TCBoflightTask);
  
  /*task1*/
//  xTaskCreate(OLEDprintTask, "task1", 128, &g_task1printinfo, osPriorityNormal, NULL);
  /*task2*/
//  xTaskCreate(OLEDprintTask, "task2", 128, &g_task2printinfo, osPriorityNormal, NULL);
  /*task3*/
  //xTaskCreate(OLEDprintTask, "task3", 128, &g_task3printinfo, osPriorityNormal, NULL);


  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */

  extern RTC_HandleTypeDef hrtc;
  Times time_now;
  RTC_TimeTypeDef RTC_TimeStruct = {0};
  RTC_DateTypeDef RTC_DataStruct = {0};

  int c_time = Get_current_time();
  time_now = stamp_to_standard(c_time);
  save_time_init(&RTC_TimeStruct,&RTC_DataStruct,time_now);
  uint8_t cycle = 0;
  OLED_Clear();
  HAL_RTC_SetTime(&hrtc, &RTC_TimeStruct,RTC_FORMAT_BIN);
  HAL_RTC_SetDate(&hrtc, &RTC_DataStruct,RTC_FORMAT_BIN);
  for(;;){
    HAL_RTC_GetTime(&hrtc, &RTC_TimeStruct, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &RTC_DataStruct, RTC_FORMAT_BIN);
    OLED_ShowString(0,16, "H:",OLED_8X16_HALF);
    OLED_ShowNum(16, 16, RTC_TimeStruct.Hours,2,OLED_8X16_HALF);

    OLED_ShowString(40,16, "M:",OLED_8X16_HALF);
    OLED_ShowNum(56, 16, RTC_TimeStruct.Minutes,2,OLED_8X16_HALF);

    OLED_ShowString(80,16, "S:",OLED_8X16_HALF);
    OLED_ShowNum(96, 16, RTC_TimeStruct.Seconds,2,OLED_8X16_HALF);

    OLED_ShowString(0,0, "Y:",OLED_8X16_HALF);
    OLED_ShowNum(16, 0, RTC_DataStruct.Year,2,OLED_8X16_HALF);

    OLED_ShowString(40,0, "M:",OLED_8X16_HALF);
    OLED_ShowNum(56, 0, RTC_DataStruct.Month,2,OLED_8X16_HALF);

    OLED_ShowString(80,0, "D:",OLED_8X16_HALF);
    OLED_ShowNum(96, 0, RTC_DataStruct.Date,2,OLED_8X16_HALF);

    OLED_Update();
    vTaskDelay(500);
  }


  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/**
  * @brief  Function implementing the encoderTask thread.
  * @param  argument: Not used
  * @retval None
  */
void EncoderTask(void *argument)
{
  /* USER CODE BEGIN EncoderTask */
  
  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
  

  int32_t encoderCount = 0;
  int32_t lastCount = 0;
  int32_t encoderValue = 0;  
  int32_t minValue = 0;      
  int32_t maxValue = 100;    
  
  __HAL_TIM_SET_COUNTER(&htim1, 32768);
  
  OLED_Clear();
  
  OLED_Update();
  
  for(;;)
  {
    
    encoderCount = __HAL_TIM_GET_COUNTER(&htim1);
    
    
    if(encoderCount != lastCount)
    {
      
      if(encoderCount > lastCount)
      {
        
        if(encoderCount - lastCount > 32768)
          encoderValue--;
        else
          encoderValue++;
      }
      else
      {
        
        if(lastCount - encoderCount > 32768)
          encoderValue++;
        else
          encoderValue--;
      }
      
      
      if(encoderValue > maxValue)
        encoderValue = minValue;
      if(encoderValue < minValue)
        encoderValue = maxValue;
      
      
      OLED_ClearArea(0, 32, 128, 32);
      
      
      OLED_ShowNum(64, 32, encoderValue, 3, OLED_8X16_HALF);
      
      
      int barWidth = (encoderValue - minValue) * 100 / (maxValue - minValue);
      OLED_DrawRectangle(0, 48, 100, 10, 0);  
      OLED_DrawRectangle(0, 48, barWidth, 10, 1);  
      
      
      OLED_UpdateArea(0, 32, 128, 32);
    
      lastCount = encoderCount;
    }
    
    
    vTaskDelay(100);
  }
  /* USER CODE END EncoderTask */
}

/* USER CODE END Application */

