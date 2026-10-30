/**
  ******************************************************************************
  * @file           : mx_hal_def.h
  * @brief          : Exporting peripherals initialization
  *                   Include entry for the target folder to the application.
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
#ifndef MX_HAL_DEF_H
#define MX_HAL_DEF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mx_def.h"
#include "stm32_hal.h"
#include "mx_cortex_mpu.h"
#include "mx_cortex_nvic.h"
#include "mx_gpio_default.h"
#include "mx_icache.h"
#include "mx_rcc.h"
#include "mx_tim3.h"
#include "mx_tim4.h"
#include "mx_tim2.h"
#include "mx_uart5.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* ########### Aliases to initialization functions ########### */

  /* *************************************************************
    Cortex_MPU: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_cortex_mpu_init
    ************************************************************* */

  /* *************************************************************
    Cortex_NVIC: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_cortex_nvic_init
    ************************************************************* */

  /* ***********************************************************
      gpio_default: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_gpio_default with HAL layer
  *         Name of the User label:
  *                   mx_gpio_status_led
  * @retval system_status_t Value based on the system_status_t
  */
#define mx_gpio_status_led_init mx_gpio_default_init

  /* *************************************************************
    ICACHE: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_icache_init
    ************************************************************* */

  /* *************************************************************
    RCC: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_rcc_init
    ************************************************************* */

  /* ***********************************************************
      TIM3: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_tim3 with HAL layer
  *         Name of the User label:
  *                   mx_example_tim_red
  * @retval hal_tim_handle_t Pointer on the handle on the TIM3 instance
  */
#define mx_example_tim_red_init mx_tim3_init

  /* ***********************************************************
      TIM4: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_tim4 with HAL layer
  *         Name of the User label:
  *                   mx_example_tim_green
  * @retval hal_tim_handle_t Pointer on the handle on the TIM4 instance
  */
#define mx_example_tim_green_init mx_tim4_init

  /* ***********************************************************
      TIM2: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_tim2 with HAL layer
  *         Name of the User label:
  *                   mx_example_tim_blue
  * @retval hal_tim_handle_t Pointer on the handle on the TIM2 instance
  */
#define mx_example_tim_blue_init mx_tim2_init

  /* *************************************************************
    UART5: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_uart5_uart_init
    ************************************************************* */

/* ########################################################### */

/* ########### Aliases to De-Initialization functions ########### */

  /* *************************************************************
    Cortex_MPU: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_cortex_mpu_deinit
    ************************************************************* */

  /* *************************************************************
    Cortex_NVIC: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_cortex_nvic_deinit
    ************************************************************* */

  /* ***********************************************************
      gpio_default: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_gpio_default with HAL layer
  *         Name of the User label:
  *                   mx_gpio_status_led
  */
#define mx_gpio_status_led_deinit mx_gpio_default_deinit

  /* *************************************************************
    ICACHE: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_icache_deinit
    ************************************************************* */

  /* ***********************************************************
      TIM3: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_tim3 with HAL layer
  *         Name of the User label:
  *                   mx_example_tim_red
  */
#define mx_example_tim_red_deinit mx_tim3_deinit

  /* ***********************************************************
      TIM4: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_tim4 with HAL layer
  *         Name of the User label:
  *                   mx_example_tim_green
  */
#define mx_example_tim_green_deinit mx_tim4_deinit

  /* ***********************************************************
      TIM2: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_tim2 with HAL layer
  *         Name of the User label:
  *                   mx_example_tim_blue
  */
#define mx_example_tim_blue_deinit mx_tim2_deinit

  /* *************************************************************
    UART5: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_uart5_uart_deinit
    ************************************************************* */

/* ########################################################### */

/* ########### Aliases to get HAL handle functions ########### */

  /* *************************************************************
    ICACHE: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_icache_gethandle
    ************************************************************* */

  /* ***********************************************************
      TIM3: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for TIM3
  *         Name of the User label:
  *                   mx_example_tim_red
  * @retval hal_tim_handle_t Pointer on the handle on the TIM3 instance
  */
#define mx_example_tim_red_gethandle mx_tim3_gethandle

  /* ***********************************************************
      TIM4: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for TIM4
  *         Name of the User label:
  *                   mx_example_tim_green
  * @retval hal_tim_handle_t Pointer on the handle on the TIM4 instance
  */
#define mx_example_tim_green_gethandle mx_tim4_gethandle

  /* ***********************************************************
      TIM2: aliases for get handle functions
    *********************************************************** *//**
  * @brief  Get the HAL handle for TIM2
  *         Name of the User label:
  *                   mx_example_tim_blue
  * @retval hal_tim_handle_t Pointer on the handle on the TIM2 instance
  */
#define mx_example_tim_blue_gethandle mx_tim2_gethandle

  /* *************************************************************
    UART5: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_uart5_uart_gethandle
    ************************************************************* */

/* ########################################################### */

/* ########### Aliases to get IRQ Handlers functions ########### */
  /* ########################################################### */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_HAL_DEF_H */
