/**
  ******************************************************************************
  * file           : example.c
  * brief          : HRTIM basic PWM generation using the master timer with HAL API
  ******************************************************************************
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_hrtim_handle_t *pHRTIM;  /* pointer referencing the HRTIM handle from the generated code */

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * Initializes the GPIO and the HRTIM for PWM generation.
  * The init of the HRTIM and the GPIO is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pHRTIM = mx_example_hrtim_init();
  if (pHRTIM != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Enables the HRTIM output channels then starts the master timer and the timing unit.
    * One output is driven by the master timer events, the other by the timing unit events.
    */
  if (HAL_HRTIM_OUT_Enable(pHRTIM, HRTIM_MASTER_CHANNEL | HRTIM_TIMER_CHANNEL) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_HRTIM_TIM_Start(pHRTIM, HRTIM_MASTER_TIMER | HRTIM_TIMER) != HAL_OK)
  {
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** Deinitializes the GPIO and HRTIM peripherals before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_hrtim_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
