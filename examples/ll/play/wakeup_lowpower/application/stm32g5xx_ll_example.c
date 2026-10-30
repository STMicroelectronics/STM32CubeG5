/**
  ******************************************************************************
  * file           : stm32g5xx_ll_example.c
  * brief          : PLAY wakeup from low power with LL API
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
void mx_playx_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/** brief:  Locks the MX_PLAYx module.
  */
void PLAY_Lock(void)
{
  LL_PLAY_Lock(MX_PLAYx);
}


/** brief: Enable the PLAY EXTI Line interrupt.
  */
void EXTI_EnableLineMode_IT(void)
{
  if (((EXTI_PLAY_LINE) & LL_EXTI_REG1) == LL_EXTI_REG1)
  {
    LL_EXTI_EnableIT_0_31(EXTI_PLAY_LINE);
  }
  else
  {
    LL_EXTI_EnableIT_32_63(EXTI_PLAY_LINE);
  }
}


/** brief: Handles the EXTI line interrupts.
  */
void mx_playx_irqhandler(void)
{
  if (((EXTI_PLAY_LINE) & LL_EXTI_REG1) == LL_EXTI_REG1)
  {
    LL_EXTI_ClearRisingFlag_0_31(EXTI_PLAY_LINE);
    LL_EXTI_ClearFallingFlag_0_31(EXTI_PLAY_LINE);
  }
  else
  {
    LL_EXTI_ClearRisingFlag_32_63(EXTI_PLAY_LINE);
    LL_EXTI_ClearFallingFlag_32_63(EXTI_PLAY_LINE);
  }
}