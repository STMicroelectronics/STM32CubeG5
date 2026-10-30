/**
  ******************************************************************************
  * @file           : mx_opamp2.c
  * @brief          : OPAMP2 Peripheral initialization
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
#include "mx_opamp2.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_opamp_handle_t hOPAMP2;

/******************************************************************************/
/* Exported functions for OPAMP2 in HAL layer */
/******************************************************************************/
hal_opamp_handle_t *mx_opamp2_init(void)
{
  if (HAL_OPAMP_Init(&hOPAMP2, HAL_OPAMP2) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_OPAMP2_EnableClock();

  /****************************************************************************/
  /* Configuration of basic features (mandatory)                              */
  /****************************************************************************/

  hal_opamp_config_t opamp_config;
  hal_opamp_config_input_connection_t config_input_connection;

  opamp_config.speed_mode          = HAL_OPAMP_SPEED_MODE_NORMAL;
  opamp_config.configuration_mode  = HAL_OPAMP_MODE_FOLLOWER;
  opamp_config.opamp_output        = HAL_OPAMP_OUTPUT_CONNECTION_EXTERNAL;
  HAL_OPAMP_SetConfig(&hOPAMP2, &opamp_config);

  config_input_connection.inverting_input = HAL_OPAMP_INVERTING_INPUT_INT;
  config_input_connection.non_inverting_input = HAL_OPAMP_NON_INVERTING_INPUT_GPIO_0;
  HAL_OPAMP_SetConfigInputConnection(&hOPAMP2, &config_input_connection);

  HAL_OPAMP_SetPGAExternalMode(&hOPAMP2, HAL_OPAMP_PGA_EXT_NONE);

  /* ### OPAMP2 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOA_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name]

       PA6     ------>   OPAMP2_VOUT
       PA7     ------>   OPAMP2_VINP0
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ANALOG;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_6 | HAL_GPIO_PIN_7, &gpio_config);

  return &hOPAMP2;
}

void mx_opamp2_deinit(void)
{
  (void)HAL_OPAMP_DeInit(&hOPAMP2);

  HAL_RCC_OPAMP2_Reset();

  HAL_RCC_OPAMP2_DisableClock();

  /* De-initialize all GPIOA pins associated with OPAMP2 */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_6 | HAL_GPIO_PIN_7);
}
hal_opamp_handle_t *mx_opamp2_gethandle(void)
{
  return &hOPAMP2;
}
