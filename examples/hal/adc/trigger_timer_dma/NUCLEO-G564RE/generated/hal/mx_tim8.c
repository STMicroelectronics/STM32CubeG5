/**
  ******************************************************************************
  * @file           : mx_tim8.c
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
#include "mx_tim8.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_tim_handle_t hTIM8;

/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for TIM8 in HAL layer */
/******************************************************************************/
hal_tim_handle_t *mx_tim8_init(void)
{
  if (HAL_TIM_Init(&hTIM8, HAL_TIM8) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_TIM8_EnableClock();

  /* Timer configuration to reach the output frequency at 1 kHz */
  hal_tim_config_t config;
  config.clock_sel.clock_source = HAL_TIM_CLK_INTERNAL;
  config.prescaler              = 4374;
  config.counter_mode           = HAL_TIM_COUNTER_UP;
  config.period                 = 0x3F;
  config.repetition_counter     = 0;
  if (HAL_TIM_SetConfig(&hTIM8, &config) != HAL_OK)
  {
    return NULL;
  }

  /* Sampling Clock */
  if (HAL_TIM_SetDTSPrescaler(&hTIM8, HAL_TIM_DTS_DIV1) != HAL_OK)
  {
    return NULL;
  }
  if (HAL_TIM_SetDTS2Prescaler(&hTIM8, HAL_TIM_DTS2_DIV1) != HAL_OK)
  {
    return NULL;
  }

  /* Update Event Management */
  if (HAL_TIM_SetUpdateSource(&hTIM8, HAL_TIM_UPDATE_REGULAR) != HAL_OK)
  {
    return NULL;
  }
  if (HAL_TIM_EnableUpdateGeneration(&hTIM8) != HAL_OK)
  {
    return NULL;
  }

  /* Master Mode Configuration */
  if (HAL_TIM_SetTriggerOutput(&hTIM8, HAL_TIM_TRGO_UPDATE) != HAL_OK)
  {
    return NULL;
  }

  /* No GPIO configuration required for TIM8 */

  return &hTIM8;
}

void mx_tim8_deinit(void)
{
  (void)HAL_TIM_DeInit(&hTIM8);

  HAL_RCC_TIM8_DisableClock();

  HAL_RCC_TIM8_Reset();

  /* No GPIO de-initialization required for TIM8 */
}

hal_tim_handle_t *mx_tim8_gethandle(void)
{
  return &hTIM8;
}
