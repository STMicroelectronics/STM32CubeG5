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

  /* Timer configuration to reach the output frequency at 24.39 kHz */
  hal_tim_config_t config;
  config.clock_sel.clock_source = HAL_TIM_CLK_INTERNAL;
  config.prescaler              = 279;
  config.counter_mode           = HAL_TIM_COUNTER_UP;
  config.period                 = 0x28;
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

  hal_tim_oc_channel_config_t oc_config;

  oc_config.polarity       = HAL_TIM_OC_LOW;
  if (HAL_TIM_OC_SetConfigChannel(&hTIM2, HAL_TIM_CHANNEL_1, &oc_config) != HAL_OK)
  {
    return NULL;
  }
  hal_tim_oc_compare_unit_config_t oc_compare_unit_config;

  oc_compare_unit_config.mode  = HAL_TIM_OC_PWM1;
  oc_compare_unit_config.pulse = 0x14;
  if (HAL_TIM_OC_SetConfigCompareUnit(&hTIM2, HAL_TIM_OC_COMPARE_UNIT_1,
                                      &oc_compare_unit_config) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_TIM_OC_EnableComparePreload(&hTIM2, HAL_TIM_OC_COMPARE_UNIT_1) != HAL_OK)
  {
    return NULL;
  }

  hal_tim_ic_channel_config_t ic_config;

  ic_config.source    = HAL_TIM_INPUT_TIM2_TI2_GPIO;
  ic_config.polarity  = HAL_TIM_IC_RISING;
  ic_config.filter    = HAL_TIM_FDIV1;
  if (HAL_TIM_IC_SetConfigChannel(&hTIM2, HAL_TIM_CHANNEL_2, &ic_config) != HAL_OK)
  {
    return NULL;
  }

  hal_tim_ic_capture_unit_config_t ic_capture_unit_config;

  ic_capture_unit_config.source     = HAL_TIM_IC_DIRECT;
  ic_capture_unit_config.prescaler  = HAL_TIM_IC_DIV1;
  if (HAL_TIM_IC_SetConfigCaptureUnit(&hTIM2, HAL_TIM_IC_CAPTURE_UNIT_2, &ic_capture_unit_config) != HAL_OK)
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

  /* Slave Mode Configuration */
  hal_tim_slave_config_t slave_config;
  slave_config.mode     = HAL_TIM_SLAVE_GATED;
  slave_config.trigger  = HAL_TIM_TRIG_TI2FP2;
  if (HAL_TIM_SetSynchroSlave(&hTIM2, &slave_config) != HAL_OK)
  {
    return NULL;
  }

  /* Master Mode Configuration */
  if (HAL_TIM_SetTriggerOutput(&hTIM2, HAL_TIM_TRGO_ENABLE) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_TIM_EnableMasterSlaveMode(&hTIM2) != HAL_OK)
  {
    return NULL;
  }

  /* ### TIM2 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOA_EnableClock();

  HAL_RCC_GPIOB_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name]

       PA0     ------>   TIM2_CH1
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_1;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_0, &gpio_config);

  /**
    [GPIO Pin] ------> [Signal Name]

       PB3     ------>   TIM2_CH2
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_1;
  HAL_GPIO_Init(HAL_GPIOB, HAL_GPIO_PIN_3, &gpio_config);

  return &hTIM2;
}

void mx_tim2_deinit(void)
{
  (void)HAL_TIM_DeInit(&hTIM2);

  HAL_RCC_TIM2_DisableClock();

  HAL_RCC_TIM2_Reset();

  /* De-initialize all GPIOA pins associated with TIM2 */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_0);

  /* De-initialize all GPIOB pins associated with TIM2 */
  HAL_GPIO_DeInit(HAL_GPIOB, HAL_GPIO_PIN_3);
}

hal_tim_handle_t *mx_tim2_gethandle(void)
{
  return &hTIM2;
}
