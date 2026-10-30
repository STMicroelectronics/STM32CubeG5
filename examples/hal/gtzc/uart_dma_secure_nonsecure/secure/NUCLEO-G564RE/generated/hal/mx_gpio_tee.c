/**
  ******************************************************************************
  * @file           : mx_gpio_tee.c
  * @brief          : GPIO trusted execution environment configuration
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
#include "mx_gpio_tee.h"
#include "stm32g5xx_ll_gpio.h"

/* Exported functions --------------------------------------------------------*/
system_status_t mx_gpio_tee_setup(void)
{
  /* Make the LPUART1 pins available to the non-secure application. */
  HAL_RCC_GPIOA_EnableClock();
  HAL_GPIO_SetSecAttr(HAL_GPIOA, (HAL_GPIO_PIN_2 | HAL_GPIO_PIN_3), HAL_GPIO_NSEC);

  HAL_RCC_GPIOG_EnableClock();
  HAL_GPIO_SetSecAttr(HAL_GPIOG, HAL_GPIO_PIN_3, HAL_GPIO_NSEC);

  return SYSTEM_OK;
}