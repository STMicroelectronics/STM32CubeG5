/**
  ******************************************************************************
  * @file           : mx_cordic1.c
  * @brief          : CORDIC1 Peripheral initialization
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
#include "mx_cordic1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototypes ----------------------------------------------*/
/* Exported variables by reference -------------------------------------------*/

/******************************************************************************/
/* Exported functions for CORDIC in LL layer                                 */
/******************************************************************************/
CORDIC_TypeDef *mx_cordic1_init(void)
{
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CORDIC1);
  LL_CORDIC_SetFunction(CORDIC1, LL_CORDIC_FUNCTION_SINE);
  LL_CORDIC_SetPrecision(CORDIC1, LL_CORDIC_PRECISION_6_CYCLE);
  /* LL_CORDIC_SetScale(CORDIC1, LL_CORDIC_SCALING_FACTOR_0); */ /* Configuration matches register reset state at startup. */
  /* LL_CORDIC_SetNbWrite(CORDIC1, LL_CORDIC_NBWRITE_1); */ /* Configuration matches register reset state at startup. */
  LL_CORDIC_SetNbRead(CORDIC1, LL_CORDIC_NBREAD_2);
  /* LL_CORDIC_SetInWidth(CORDIC1, LL_CORDIC_INWIDTH_32_BIT); */ /* Configuration matches register reset state at startup. */
  /* LL_CORDIC_SetOutWidth(CORDIC1, LL_CORDIC_OUTWIDTH_32_BIT); */ /* Configuration matches register reset state at startup. */
  LL_CORDIC_EnableIT(CORDIC1);
  /* Enable the interruption for CORDIC1 */
  NVIC_SetPriority(CORDIC1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(CORDIC1_IRQn);
  return CORDIC1;
}
void mx_cordic1_deinit(void)
{
  /* Disable the interruption for CORDIC */
  LL_CORDIC_DisableIT(CORDIC1);
  NVIC_DisableIRQ(CORDIC1_IRQn);

  LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_CORDIC1);
  LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_CORDIC1);

  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_CORDIC1);
}

/******************************************************************************/
/*            CORDIC1 interrupt is managed directly in user code.             */
/******************************************************************************/
/* void CORDIC1_IRQHandler(void)
{
}
  */
