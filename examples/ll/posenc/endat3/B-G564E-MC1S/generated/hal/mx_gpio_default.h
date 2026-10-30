/**
  ******************************************************************************
  * @file           : mx_gpio_default.h
  * @brief          : Header for mx_gpio_default.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_GPIO_DEFAULT_H
#define MX_GPIO_DEFAULT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"
#include "mx_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/******************************************************************************/
/* Exported defines for gpio_default in LL layer                             */
/******************************************************************************/

/* GPIO pin aliases for PA11 */
#define PA11_GREEN_LED_PORT                             GPIOA
#define PA11_GREEN_LED_PIN                              LL_GPIO_PIN_11
#define PA11_GREEN_LED_INIT_STATE                       LL_GPIO_PIN_SET
#define PA11_GREEN_LED_ACTIVE_STATE                     LL_GPIO_PIN_RESET
#define PA11_GREEN_LED_INACTIVE_STATE                   LL_GPIO_PIN_SET

#define MX_STATUS_LED_PORT                              GPIOA
#define MX_STATUS_LED_PIN                               LL_GPIO_PIN_11
#define MX_STATUS_LED_INIT_STATE                        LL_GPIO_PIN_SET
#define MX_STATUS_LED_ACTIVE_STATE                      LL_GPIO_PIN_RESET
#define MX_STATUS_LED_INACTIVE_STATE                    LL_GPIO_PIN_SET

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for gpio_default in LL layer                           */
/******************************************************************************/
/**
  * @brief mx_gpio_default init function
  * This function configures the hardware resources used in this example
  * @retval 0  GPIO group correctly initialized
  * @retval -1 Issue detected during GPIO group initialization
  */
system_status_t mx_gpio_default_init(void);

/**
  * @brief  De-initialize gpio_default instance.
  */
system_status_t mx_gpio_default_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_GPIO_DEFAULT_H */
