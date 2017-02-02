/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "../GSM/1Wire.h"
#include "../GSM/gsm.h"
#include "../GSM/ComPort.h"
#include "../GSM/Arch.h"
#include "../GSM/programming.h"


/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId GSM_TaskHandle;
osThreadId ComPort_TaskHandle;
osThreadId OnwWireTaskHandle;
osThreadId ComminicationTaHandle;
osThreadId SupportUtil_TasHandle;

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void GSM_Task_Start(void const * argument);
void ComPort_Task_Start(void const * argument);
void OnwWireTask_Start(void const * argument);
void ComminicationTask_Start(void const * argument);
void SupportUtil_Start(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
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

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of GSM_Task */
  osThreadDef(GSM_Task, GSM_Task_Start, osPriorityNormal, 0, 1000);
  GSM_TaskHandle = osThreadCreate(osThread(GSM_Task), NULL);

  /* definition and creation of ComPort_Task */
  osThreadDef(ComPort_Task, ComPort_Task_Start, osPriorityNormal, 0, 128);
  ComPort_TaskHandle = osThreadCreate(osThread(ComPort_Task), NULL);

  /* definition and creation of OnwWireTask */
  osThreadDef(OnwWireTask, OnwWireTask_Start, osPriorityNormal, 0, 128);
  OnwWireTaskHandle = osThreadCreate(osThread(OnwWireTask), NULL);

  /* definition and creation of ComminicationTa */
  osThreadDef(ComminicationTa, ComminicationTask_Start, osPriorityNormal, 0, 128);
  ComminicationTaHandle = osThreadCreate(osThread(ComminicationTa), NULL);

  /* definition and creation of SupportUtil_Tas */
  osThreadDef(SupportUtil_Tas, SupportUtil_Start, osPriorityNormal, 0, 128);
  SupportUtil_TasHandle = osThreadCreate(osThread(SupportUtil_Tas), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* GSM_Task_Start function */
void GSM_Task_Start(void const * argument)
{
  /* USER CODE BEGIN GSM_Task_Start */
  /* Infinite loop */
  GSM_Task_Loop();
  /* USER CODE END GSM_Task_Start */
}

/* ComPort_Task_Start function */
void ComPort_Task_Start(void const * argument)
{
  /* USER CODE BEGIN ComPort_Task_Start */
  /* Infinite loop */
  ComPort_Task_Loop();
  /* USER CODE END ComPort_Task_Start */
}

/* OnwWireTask_Start function */
void OnwWireTask_Start(void const * argument)
{
  /* USER CODE BEGIN OnwWireTask_Start */
  /* Infinite loop */
  OnrWireTaskLoop();
  /* USER CODE END OnwWireTask_Start */
}

/* ComminicationTask_Start function */
void ComminicationTask_Start(void const * argument)
{
  /* USER CODE BEGIN ComminicationTask_Start */
  /* Infinite loop */
  ComminicationTaskLoop();
  /* USER CODE END ComminicationTask_Start */
}

/* SupportUtil_Start function */
void SupportUtil_Start(void const * argument)
{
  /* USER CODE BEGIN SupportUtil_Start */
  
  SupportUtilLoop();
  /* USER CODE END SupportUtil_Start */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
