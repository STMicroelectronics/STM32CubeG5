/**
  ******************************************************************************
  * file           : example.c
  * brief          : Use the 2 HRTIM instances to generate 24 ysnchronized PWM signals
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
#define HRTIM_DLL_CALIBRATION_TIMEOUT_MS (5U)


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_hrtim_handle_t *pHRTIM1; /* pointer referencing the handle of the reference HRTIM instance */

/* Private functions prototype -----------------------------------------------*/

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * Initializes both HRTIM instances (`HRTIM1` as reference and `HRTIM2` as synchronized),
    * then registers callbacks for HRTIM1 master repetition event and HRTIM2 synchronization event.
    */
  pHRTIM1 = mx_example_hrtim_high_resolution_pwm_init();
  if (pHRTIM1 == NULL)
  {
    goto _app_init_exit;
  }

  /** ########## Step 2 ##########
    * Launch periodic DLL calibration and wait for the end of calibration process.
    */
  if (HAL_HRTIM_StartPeriodicDLLCalibration(pHRTIM1, HAL_HRTIM_DLL_CALIB_RATE_0) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_HRTIM_PollForDLLCalibration(pHRTIM1, HRTIM_DLL_CALIBRATION_TIMEOUT_MS) != HAL_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;


  /** ########## Step 3 ##########
    * Start complementary PWM signals generation.
    */
  if (HAL_HRTIM_OUT_Enable(pHRTIM1, (MX_HRTIM_CHX1 | MX_HRTIM_CHX2)) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_HRTIM_TIM_Start(pHRTIM1, MX_HRTIM_TIMX) != HAL_OK)
  {
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 4 ##########
  * Deinitializes the HRTIM instances before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status = EXEC_STATUS_OK;

  mx_example_hrtim_high_resolution_pwm_deinit();

  return return_status;
} /* end app_deinit */


