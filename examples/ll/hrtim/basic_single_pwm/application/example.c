/**
  ******************************************************************************
  * file           : example.c
  * brief          : generate a 100 kHz PWM signal with a 50% duty cycle using the High-Resolution Timer (HRTIM) through
  *                  the LL HRTIM API.
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
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * Initializes the MX_HRTIMx and the GPIO for PWM generation.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_hrtimx_init() == NULL)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Starts the MX_HRTIMx PWM generation.
  */
app_status_t app_process(void)
{
  /* Start the MX_HRTIMx channels */
  HRTIM_StartChannel();

  /* Start the MX_HRTIMx */
  HRTIM_Start();

  return EXEC_STATUS_OK;
} /* end app_process */


/** Deinitializes the MX_HRTIMx before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_hrtimx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
