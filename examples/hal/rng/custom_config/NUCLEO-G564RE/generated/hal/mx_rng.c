/**
  ******************************************************************************
  * @file           : mx_rng.c
  * @brief          : RNG Peripheral initialization
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
#include "mx_rng.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define  RNG_DRDY_TIMEOUT_MS            10U
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_rng_handle_t hRNG;
/******************************************************************************/
/* Exported functions for RNG in HAL layer */
/******************************************************************************/
hal_rng_handle_t *mx_rng_init(void)
{
uint32_t tickstart = HAL_GetTick();
  HAL_RCC_RNG_EnableClock();

  if (HAL_RNG_Init(&hRNG, HAL_RNG) != HAL_OK)
  {
    return NULL;
  }

  /* Code HAL_RNG_Config....*/
  hal_rng_config_t rng_config;

  rng_config.config_1              = 31;
  rng_config.config_2              = 0;
  rng_config.config_3              = 13;
  rng_config.clock_divider         = HAL_RNG_CLOCK_DIVIDER_BY_1;
  rng_config.standard              = HAL_RNG_CUSTOM;
  rng_config.clock_error_detection = HAL_RNG_CLOCK_ERROR_DETECTION_ENABLED;
  rng_config.health_test           = 43719;
  rng_config.noise_src.osc_1_src   = HAL_RNG_OSCILLATOR_SOURCE_1 | HAL_RNG_OSCILLATOR_SOURCE_2 | HAL_RNG_OSCILLATOR_SOURCE_3;
  rng_config.noise_src.osc_2_src   = HAL_RNG_OSCILLATOR_SOURCE_1 | HAL_RNG_OSCILLATOR_SOURCE_2 | HAL_RNG_OSCILLATOR_SOURCE_3;
  rng_config.noise_src.osc_3_src   = HAL_RNG_OSCILLATOR_SOURCE_1 | HAL_RNG_OSCILLATOR_SOURCE_2 | HAL_RNG_OSCILLATOR_SOURCE_3;
  if (HAL_RNG_SetConfig(&hRNG, &rng_config) != HAL_OK)
  {
    return NULL;
  }

  LL_RNG_Enable(RNG);
  while (LL_RNG_IsActiveFlag_DRDY(RNG) == 0U)
  {
    /* Check for the timeout */
    if ((HAL_GetTick() - tickstart) > RNG_DRDY_TIMEOUT_MS)
    {
      return NULL;
    }
  }
  LL_RNG_Disable(RNG);
  return &hRNG;
}

void mx_rng_deinit(void)
{
  (void)HAL_RNG_DeInit(&hRNG);
}

hal_rng_handle_t *mx_rng_gethandle(void)
{
  return &hRNG;
}
