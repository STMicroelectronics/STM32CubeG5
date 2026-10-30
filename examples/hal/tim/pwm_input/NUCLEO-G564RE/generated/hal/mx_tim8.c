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

  /* Timer configuration to reach the output frequency at 15 Hz */
  hal_tim_config_t config;
  config.clock_sel.clock_source = HAL_TIM_CLK_INTERNAL;
  config.prescaler              = 279;
  config.counter_mode           = HAL_TIM_COUNTER_UP;
  config.period                 = 0xFFFF;
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

  hal_tim_ic_channel_config_t ic_config;

  ic_config.source    = HAL_TIM_INPUT_TIM8_TI1_GPIO;
  ic_config.polarity  = HAL_TIM_IC_RISING;
  ic_config.filter    = HAL_TIM_FDIV1;
  if (HAL_TIM_IC_SetConfigChannel(&hTIM8, HAL_TIM_CHANNEL_1, &ic_config) != HAL_OK)
  {
    return NULL;
  }

  hal_tim_ic_capture_unit_config_t ic_capture_unit_config;

  ic_capture_unit_config.source     = HAL_TIM_IC_DIRECT;
  ic_capture_unit_config.prescaler  = HAL_TIM_IC_DIV1;
  if (HAL_TIM_IC_SetConfigCaptureUnit(&hTIM8, HAL_TIM_IC_CAPTURE_UNIT_1, &ic_capture_unit_config) != HAL_OK)
  {
    return NULL;
  }

  ic_capture_unit_config.source     = HAL_TIM_IC_INDIRECT_FALLING;
  ic_capture_unit_config.prescaler  = HAL_TIM_IC_DIV1;
  if (HAL_TIM_IC_SetConfigCaptureUnit(&hTIM8, HAL_TIM_IC_CAPTURE_UNIT_2, &ic_capture_unit_config) != HAL_OK)
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

  /* Slave Mode Configuration */
  hal_tim_slave_config_t slave_config;
  slave_config.mode     = HAL_TIM_SLAVE_COMBINED_RESET_TRIGGER;
  slave_config.trigger  = HAL_TIM_TRIG_TI1FP1;
  if (HAL_TIM_SetSynchroSlave(&hTIM8, &slave_config) != HAL_OK)
  {
    return NULL;
  }

  /* ### TIM8 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOC_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name]

       PC6     ------>   TIM8_CH1
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_4;
  HAL_GPIO_Init(HAL_GPIOC, HAL_GPIO_PIN_6, &gpio_config);

  /* Enable Timer Capture/Compare interrupt */
  HAL_CORTEX_NVIC_SetPriority(TIM8_CC_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(TIM8_CC_IRQn);

  return &hTIM8;
}

void mx_tim8_deinit(void)
{
  (void)HAL_TIM_DeInit(&hTIM8);

  HAL_RCC_TIM8_DisableClock();

  HAL_RCC_TIM8_Reset();

  /* De-initialize all GPIOC pins associated with TIM8 */
  HAL_GPIO_DeInit(HAL_GPIOC, HAL_GPIO_PIN_6);

  /* Disable Timer Capture/Compare interrupt */
  HAL_CORTEX_NVIC_DisableIRQ(TIM8_CC_IRQn);
}

hal_tim_handle_t *mx_tim8_gethandle(void)
{
  return &hTIM8;
}

/******************************************************************************/
/*                       TIM8 capture compare interrupt                       */
/******************************************************************************/
void TIM8_CC_IRQHandler(void)
{
  HAL_TIM_CC_IRQHandler(&hTIM8);
}
