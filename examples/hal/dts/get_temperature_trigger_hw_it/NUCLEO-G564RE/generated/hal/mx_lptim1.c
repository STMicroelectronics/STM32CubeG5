/**
  ******************************************************************************
  * @file           : mx_lptim1.c
  * @brief          : LPTIM1 Peripheral initialization
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
#include "mx_lptim1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/* Exported variables by reference--------------------------------------------*/
static hal_lptim_handle_t hLPTIM1;

/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for SW instance in HAL layer */
/******************************************************************************/

hal_lptim_handle_t *mx_lptim1_init(void)
{
  hal_lptim_config_t  config;

  if (HAL_LPTIM_Init(&hLPTIM1, HAL_LPTIM1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_LPTIM1_EnableClock();

  if (HAL_RCC_LPTIM1_SetKernelClkSource(HAL_RCC_LPTIM1_CLK_SRC_PCLK3) != HAL_OK)
  {
    return NULL;
  }

  /* Timer configuration to reach the output frequency at 1.398 kHz */
  config.mode               = HAL_LPTIM_CONTINUOUS;
  config.clock_source       = HAL_LPTIM_CLK_INTERNAL;
  config.period             = 0x30D;
  config.prescaler          = HAL_LPTIM_CLK_SRC_DIV128;
  config.repetition_counter = 0;

  if (HAL_LPTIM_SetConfig(&hLPTIM1, &config) != HAL_OK)
  {
    return NULL;
  }

  hal_lptim_oc_config_t oc_config1;
  oc_config1.pulse = 0x186;
  oc_config1.polarity = HAL_LPTIM_OC_HIGH;
  if (HAL_LPTIM_OC_SetConfigChannel(&hLPTIM1, HAL_LPTIM_CHANNEL_1, &oc_config1) != HAL_OK)
  {
    return NULL;
  }

  return &hLPTIM1;
}

void mx_lptim1_deinit(void)
{
  (void)HAL_LPTIM_DeInit(&hLPTIM1);

  HAL_RCC_LPTIM1_Reset();

  HAL_RCC_LPTIM1_DisableClock();
}

hal_lptim_handle_t *mx_lptim1_gethandle(void)
{
  return &hLPTIM1;
}
