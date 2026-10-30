/**
  ******************************************************************************
  * @file           : mx_led.h
  * @brief          : Code generation for the LED part driver.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef MX_LED_H
#define MX_LED_H

#include "led.h"
#include "mx_hal_def.h"

#define LED_0                              0
#define MX_STATUS_LED                      LED_0

#define LED_0_GPIO_PORT                    HAL_GPIOG
#define LED_0_PIN                          HAL_GPIO_PIN_3
#define LED_0_ACTIVE_STATE                 HAL_GPIO_PIN_SET
#define LED_0_INACTIVE_STATE               HAL_GPIO_PIN_RESET

#define MX_STATUS_LED_GPIO_PORT            LED_0_GPIO_PORT
#define MX_STATUS_LED_PIN                  LED_0_PIN
#define MX_STATUS_LED_ACTIVE_STATE         LED_0_ACTIVE_STATE
#define MX_STATUS_LED_INACTIVE_STATE       LED_0_INACTIVE_STATE

#endif /* MX_LED_H */