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

/* GPIO pin aliases for PC13 */
#define PC13_PORT                                       GPIOC
#define PC13_PIN                                        LL_GPIO_PIN_13

/* EXTI aliases */
#define MX_EXTIX_EXTI_LINE                              LL_EXTI_LINE_13
#define MX_EXTIX_EXTI_TRIGGER                           LL_EXTI_TRIGGER_RISING

/* GPIO pin aliases for PG3 */
#define PG3_PORT                                        GPIOG
#define PG3_PIN                                         LL_GPIO_PIN_3
#define PG3_INIT_STATE                                  LL_GPIO_PIN_RESET
#define PG3_ACTIVE_STATE                                LL_GPIO_PIN_SET
#define PG3_INACTIVE_STATE                              LL_GPIO_PIN_RESET

#define MX_STATUS_LED_PORT                              GPIOG
#define MX_STATUS_LED_PIN                               LL_GPIO_PIN_3
#define MX_STATUS_LED_INIT_STATE                        LL_GPIO_PIN_RESET
#define MX_STATUS_LED_ACTIVE_STATE                      LL_GPIO_PIN_SET
#define MX_STATUS_LED_INACTIVE_STATE                    LL_GPIO_PIN_RESET

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

/******************************************************************************/
/*          EXTI Line13 interrupt is managed directly in user code.           */
/******************************************************************************/
/* void EXTI13_IRQHandler(void); */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_GPIO_DEFAULT_H */
