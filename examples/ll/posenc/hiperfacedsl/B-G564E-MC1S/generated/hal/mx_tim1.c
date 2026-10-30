/**
  ******************************************************************************
  * @file           : mx_tim1.c
  * @brief          : Peripheral initialization
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_stm32g5xx_hal_drivers_license.md file
  * in the same directory as the generated code.
  * If no mx_stm32g5xx_hal_drivers_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx_tim1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for TIM1 in LL layer */
/******************************************************************************/

TIM_TypeDef *mx_tim1_init(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);

  /* Timer configuration to reach the output frequency at 10 kHz */
  LL_TIM_SetPrescaler(TIM1, 9);
  /* LL_TIM_SetCounterMode(TIM1, LL_TIM_COUNTERMODE_UP); */ /* Configuration matches register reset state at startup. */
  LL_TIM_SetAutoReload(TIM1, 2799);
  /* LL_TIM_SetClockDivision(TIM1, LL_TIM_CLOCKDIVISION_DIV1); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_SetClockDivision2(TIM1, LL_TIM_CLOCKDIVISION2_DIV1); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_SetClockSource(TIM1, LL_TIM_CLK_INTERNAL); */ /* Configuration matches register reset state at startup. */

  /* Disable update interrupt with update generation (UG)
     by changing update request source (URS) to avoid update flag (UIF) */
  LL_TIM_SetUpdateSource(TIM1, LL_TIM_UPDATESOURCE_COUNTER);
  /* Generate an update event to reload the prescaler
     and the repetition counter (if applicable) values immediately */
  LL_TIM_GenerateEvent_UPDATE(TIM1);

  /* Update Event Management */
  LL_TIM_SetUpdateSource(TIM1, LL_TIM_UPDATESOURCE_REGULAR);
  /* LL_TIM_EnableUpdateEvent(TIM1); */ /* Configuration matches register reset state at startup. */

  /* Master Mode Configuration */
  LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_UPDATE);

  /* No GPIO configuration required for TIM1 */

  return TIM1;
}

void mx_tim1_deinit(void)
{
  LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM1);

  LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM1);
  LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM1);

  /* No GPIO de-initialization required for TIM1 */
}
