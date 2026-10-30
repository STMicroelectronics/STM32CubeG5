/**
  ******************************************************************************
  * file           : stm32g5xx_ll_example.c
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
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
#define CALIBRATION_TIMEOUT_MS 10U
#endif /* USE_LL_APP_TIMEOUT */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

inline system_status_t HRTIM_StartDLLCalibration(void)
{

  system_status_t return_status = SYSTEM_OK;

#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
  uint32_t timeout_ms = CALIBRATION_TIMEOUT_MS;
#endif /* USE_LL_APP_TIMEOUT */

  /* Start periodic DLL calibration */
  LL_HRTIM_SetDLLCalibrationRate(MX_HRTIMx, LL_HRTIM_DLLCALIBRATION_RATE_3);
  LL_HRTIM_EnablePeriodicDLLCalibration(MX_HRTIMx);

  /* Poll for end of DLL calibration */
  while (LL_HRTIM_IsActiveFlag_DLLRDY(MX_HRTIMx) == 0U)
  {
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
    /* Check Systick counter flag to decrement the time-out value */
    if (SysTick_IsActiveCounterFlag() != 0)
    {
      if (timeout_ms-- == 0)
      {
        return_status = SYSTEM_PERIPHERAL_ERROR;
        break;
      }
    }
#endif /* USE_LL_APP_TIMEOUT */
  }

  return return_status;
}

inline system_status_t HRTIM_StartWaveformGeneration(void)
{
  /* Enable the HRTIM output channels */
  LL_HRTIM_EnableOutput(MX_HRTIMx, MX_HRTIM_1_OUTPUT | MX_HRTIM_2_OUTPUT);

  /* Start the timer waveform generation on each output */
  LL_HRTIM_TIM_CounterEnable(MX_HRTIMx, MX_HRTIM_TIMER);

  /* No point of failure */
  return SYSTEM_OK;
}
