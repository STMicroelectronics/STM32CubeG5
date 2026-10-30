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
#include "stm32_ll.h"
#include "mx_cortex_mpu.h"
#include "mx_cortex_nvic.h"
#include "mx_gpio_default.h"
#include "mx_icache.h"
#include "mx_rcc.h"
#include "mx_usart1.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
  /* *************************************************************
    Cortex_MPU (LL): No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for CMSIS instance
    ************************************************************* */

  /* *************************************************************
    Cortex_NVIC (LL): No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for CMSIS instance
    ************************************************************* */

  /* *************************************************************
    ICACHE (LL): No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for CMSIS instance
    ************************************************************* */

  /* *************************************************************
    RCC (LL): No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for CMSIS instance
    ************************************************************* */

  /* ***********************************************************
                Aliases to USART1 used in LL
    *********************************************************** */
#define MX_USARTx  USART1

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

  /* *************************************************************
    gpio_default: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_gpio_default_init
    ************************************************************* */

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
      USART1: aliases for initialization functions
    *********************************************************** */

/**
  * @brief  Initialize the mx_usart1_usart with LL layer
  *         Name of the User label:
  *                   MX_USARTx
  * @retval USART1 Pointer on the USART1
  */
#define mx_usartx_init mx_usart1_usart_init

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

  /* *************************************************************
    gpio_default: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_gpio_default_deinit
    ************************************************************* */

  /* *************************************************************
    ICACHE: No software label has been defined for this peripheral instance
      in the STM32CubeMX2 configuration panel.
      As a result, no aliases are generated for mx_icache_deinit
    ************************************************************* */

  /* ***********************************************************
      USART1: aliases for De-Initialization functions
    *********************************************************** */

/**
  * @brief  De-Initialize the mx_usart1_usart with LL layer
  *         Name of the User label:
  *                   MX_USARTx
  */
#define mx_usartx_deinit mx_usart1_usart_deinit

/* ########################################################### */

/* ########### Aliases to get HAL handle functions ########### */

/* ########################################################### */

/* ########### Aliases to get IRQ Handlers functions ########### */
  /* *************************************************************
    USART1: USART1 global interrupt
    Alias on USART1_IRQHandler and USART1_IRQn
    ************************************************************* */
#define mx_usartx_irqhandler USART1_IRQHandler
#define MX_USARTX_IRQN       USART1_IRQn

  /* *************************************************************
    USART1 / GPDMA1_CH0: GPDMA1 channel 0 global interrupt
    Alias on GPDMA1_CH0_IRQHandler and GPDMA1_CH0_IRQn
    ************************************************************* */
#define mx_lpdmax_ch1y_irqhandler GPDMA1_CH0_IRQHandler
#define MX_LPDMAX_CH1Y_IRQN       GPDMA1_CH0_IRQn

  /* *************************************************************
    USART1 / GPDMA1_CH1: GPDMA1 channel 1 global interrupt
    Alias on GPDMA1_CH1_IRQHandler and GPDMA1_CH1_IRQn
    ************************************************************* */
#define mx_lpdmax_ch2y_irqhandler GPDMA1_CH1_IRQHandler
#define MX_LPDMAX_CH2Y_IRQN       GPDMA1_CH1_IRQn

  /* ########################################################### */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_HAL_DEF_H */
