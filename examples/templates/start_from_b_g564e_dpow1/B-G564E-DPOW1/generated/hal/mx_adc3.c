/**
  ******************************************************************************
  * @file           : mx_adc3.c
  * @brief          : ADC3 Peripheral initialization
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
#include "mx_adc3.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_adc_handle_t hADC3;

/******************************************************************************/
/* Exported functions for ADC3 in HAL layer */
/******************************************************************************/
hal_adc_handle_t *mx_adc3_init(void)
{
  if (HAL_ADC_Init(&hADC3, HAL_ADC3) != HAL_OK)
  {
    return NULL;
  }

    HAL_RCC_ADC3_EnableClock();

  if (HAL_RCC_ADC_SetKernelClkSource(HAL_RCC_ADC_CLK_SRC_TIM) != HAL_OK)
  {
    return NULL;
  }

  hal_adc_config_t adc_config;
  hal_adc_channel_config_t adc_channel_config;

  adc_config.resolution          = HAL_ADC_RESOLUTION_8_BIT;
  adc_config.sampling_mode       = HAL_ADC_SAMPLING_MODE_NORMAL;
  if (HAL_ADC_SetConfig(&hADC3, &adc_config) != HAL_OK)
  {
    return NULL;
  }

  /****************************************************************************/
  /* Configuration of basic features (mandatory)                              */
  /****************************************************************************/

  /* ==================== Group Regular ====================*/
  hal_adc_reg_config_t reg_config;
  reg_config.trigger_src        = HAL_ADC_REG_TRIG_SOFTWARE;
  reg_config.sequencer_length   = 1;
  reg_config.sequencer_discont  = HAL_ADC_REG_SEQ_DISCONT_DISABLE;
  reg_config.continuous         = HAL_ADC_REG_CONV_CONTINUOUS;
  reg_config.overrun            = HAL_ADC_REG_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_REG_SetConfig(&hADC3, &reg_config) != HAL_OK)
  {
    return NULL;
  }

  adc_channel_config.group           = HAL_ADC_GROUP_REGULAR;
  adc_channel_config.sequencer_rank  = 1;
  adc_channel_config.sampling_time   = HAL_ADC_SAMPLING_TIME_256CYCLES_5;
  adc_channel_config.input_mode      = HAL_ADC_IN_SINGLE_ENDED;
  if (HAL_ADC_SetConfigChannel(&hADC3, HAL_ADC_CHANNEL_9, &adc_channel_config) != HAL_OK)
  {
    return NULL;
  }

  /****************************************************************************/
  /* Configuration of additional features (optional)                          */
  /****************************************************************************/

  /* ### ADC3 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOD_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name]

       PD12    ------>   ADC3_INP9
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ANALOG;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  HAL_GPIO_Init(HAL_GPIOD, HAL_GPIO_PIN_12, &gpio_config);

  return &hADC3;
}

void mx_adc3_deinit(void)
{
  (void)HAL_ADC_DeInit(&hADC3);

  HAL_RCC_ADC3_DisableClock();

  HAL_RCC_ADC3_Reset();

  /* De-initialize all GPIOD pins associated with ADC3 */
  HAL_GPIO_DeInit(HAL_GPIOD, HAL_GPIO_PIN_12);
}

hal_adc_handle_t *mx_adc3_gethandle(void)
{
  return &hADC3;
}
