/**
  ******************************************************************************
  * @file           : mx_dac6.c
  * @brief          : DAC6 Peripheral initialization
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
#include "mx_dac6.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_dac_handle_t hDAC6;

/******************************************************************************/
/* Exported functions for DAC6 in HAL layer */
/******************************************************************************/
hal_dac_handle_t *mx_dac6_init(void)
{
  if (HAL_DAC_Init(&hDAC6, HAL_DAC6) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_DAC6_EnableClock();

  if (HAL_RCC_DAC6_SetKernelClkSource(HAL_RCC_DAC6_CLK_SRC_TIM) != HAL_OK)
  {
    return NULL;
  }

  /****************************************************************************/
  /* Initialization of DAC instance                                           */
  /****************************************************************************/

  hal_dac_config_t dac_config;
  dac_config.high_frequency_mode = HAL_DAC_HIGH_FREQ_MODE_ABOVE_160MHZ;
  if (HAL_DAC_SetConfig(&hDAC6, &dac_config) != HAL_OK)
  {
    return NULL;
  }

  /****************************************************************************/
  /* Configuration of DAC channel                                             */
  /****************************************************************************/

  hal_dac_channel_config_t dac_channel_config;

  /* ========== Channel 1 ========== */
  dac_channel_config.alignment         = HAL_DAC_DATA_ALIGN_12_BITS_RIGHT;
  dac_channel_config.trigger           = HAL_DAC_TRIGGER_NONE;
  dac_channel_config.output_buffer     = HAL_DAC_OUTPUT_BUFFER_DISABLED;
  dac_channel_config.output_connection = HAL_DAC_OUTPUT_CONNECTION_INTERNAL;
  dac_channel_config.data_sign_format  = HAL_DAC_SIGN_FORMAT_UNSIGNED;
  if (HAL_DAC_SetConfigChannel(&hDAC6, HAL_DAC_CHANNEL_1, &dac_channel_config) != HAL_OK)
  {
    return NULL;
  }

  return &hDAC6;
}

void mx_dac6_deinit(void)
{
  (void)HAL_DAC_DeInit(&hDAC6);

  HAL_RCC_DAC6_DisableClock();

  HAL_RCC_DAC6_Reset();

  /* No GPIO de-initialization required for DAC6 */
}

hal_dac_handle_t *mx_dac6_gethandle(void)
{
  return &hDAC6;
}
