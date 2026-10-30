/**
  ******************************************************************************
  * file           : stm32g5xx_ll_example.c
  * brief          : generate a PWM waveform using the HRTIM master timer alongside a timing unit through
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
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  *  Enables the MX_HRTIMx configured output channels.
  */
inline void HRTIM_StartChannel(void)
{
  LL_HRTIM_EnableOutput(MX_HRTIMx, MX_HRTIM_MASTER_OUTPUT | MX_HRTIM_TIMING_OUTPUT);
}


/**
  *  Starts the MX_HRTIMx master timer and timing unit counters.
  */
inline void HRTIM_Start(void)
{
  LL_HRTIM_TIM_CounterEnable(MX_HRTIMx, MX_HRTIM_MASTER_TIMER | MX_HRTIM_TIMING_TIMER);
}
