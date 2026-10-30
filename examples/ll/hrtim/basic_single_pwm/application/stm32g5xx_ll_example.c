/**
  ******************************************************************************
  * file           : stm32g5xx_ll_example.c
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
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  *  Start the MX_HRTIMx output channel.
  */
inline void HRTIM_StartChannel(void)
{
  LL_HRTIM_EnableOutput(MX_HRTIMx, MX_HRTIM_X_OUTPUT);
}


/**
  *  Start the MX_HRTIMx counter.
  */
inline void HRTIM_Start(void)
{
  LL_HRTIM_TIM_CounterEnable(MX_HRTIMx, MX_HRTIM_X_TIMER);
}
