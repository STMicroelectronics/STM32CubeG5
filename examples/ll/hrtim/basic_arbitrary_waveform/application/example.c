/**
  ******************************************************************************
  * file           : example.c
  * brief          : HRTIM basic arbitrary waveform generation with LL API
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
/* Private functions ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * Initializes the GPIO and the HRTIM output channels for non-PWM waveform generation.
  * The init of the HRTIM and the GPIO is triggered by the applicative code.
  * Starts the periodic DLL calibration.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_hrtimx_init() != NULL)
  {
    /* Start the DLL calibration periodically. It is needed to reach high resolution */
    if (HRTIM_StartDLLCalibration() == SYSTEM_OK)
    {
      return_status = EXEC_STATUS_INIT_OK;
    }
  }

  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Enables the HRTIM output channels and starts the timer waveform generation on each output.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  HRTIM_StartWaveformGeneration();

  return_status = EXEC_STATUS_OK;

  return return_status;
} /* end app_process */


/** Deinitializes the GPIO and HRTIM peripherals before leaving the scenario.
  * This example runs infinitely, so this function is never called. It is provided for reference only.
  */
app_status_t app_deinit(void)
{
  mx_hrtimx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
