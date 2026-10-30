/**
  ******************************************************************************
  * @file           : mx_tim2.c
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
#include "mx_tim2.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_tim_handle_t hTIM2;

/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for TIM2 in HAL layer */
/******************************************************************************/
hal_tim_handle_t *mx_tim2_init(void)
{
  if (HAL_TIM_Init(&hTIM2, HAL_TIM2) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_TIM2_EnableClock();

  /* Timer configuration to reach the output frequency at 0 Hz */
  hal_tim_config_t config;
  config.clock_sel.clock_source = HAL_TIM_CLK_INTERNAL;
  config.prescaler              = 1;
  config.counter_mode           = HAL_TIM_COUNTER_UP;
  config.period                 = 0xFFFFFFFF;
  config.repetition_counter     = 0;
  if (HAL_TIM_SetConfig(&hTIM2, &config) != HAL_OK)
  {
    return NULL;
  }

  /* Sampling Clock */
  if (HAL_TIM_SetDTSPrescaler(&hTIM2, HAL_TIM_DTS_DIV1) != HAL_OK)
  {
    return NULL;
  }
  if (HAL_TIM_SetDTS2Prescaler(&hTIM2, HAL_TIM_DTS2_DIV1) != HAL_OK)
  {
    return NULL;
  }

  hal_tim_oc_compare_unit_config_t oc_compare_unit_config;

  oc_compare_unit_config.mode  = HAL_TIM_OC_PWM1;
  oc_compare_unit_config.pulse = 0x00000000;
  if (HAL_TIM_OC_SetConfigCompareUnit(&hTIM2, HAL_TIM_OC_COMPARE_UNIT_1,
                                      &oc_compare_unit_config) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_TIM_OC_EnableComparePreload(&hTIM2, HAL_TIM_OC_COMPARE_UNIT_1) != HAL_OK)
  {
    return NULL;
  }

  /* Update Event Management */
  if (HAL_TIM_SetUpdateSource(&hTIM2, HAL_TIM_UPDATE_REGULAR) != HAL_OK)
  {
    return NULL;
  }
  if (HAL_TIM_EnableUpdateGeneration(&hTIM2) != HAL_OK)
  {
    return NULL;
  }

  /* No GPIO configuration required for TIM2 */

  return &hTIM2;
}

void mx_tim2_deinit(void)
{
  (void)HAL_TIM_DeInit(&hTIM2);

  HAL_RCC_TIM2_DisableClock();

  HAL_RCC_TIM2_Reset();

  /* No GPIO de-initialization required for TIM2 */
}

hal_tim_handle_t *mx_tim2_gethandle(void)
{
  return &hTIM2;
}
