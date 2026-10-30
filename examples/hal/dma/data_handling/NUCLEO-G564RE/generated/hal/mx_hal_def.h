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
#include "mx_gpdma1_ch0.h"
#include "mx_gpio_default.h"
#include "mx_icache.h"
#include "mx_rcc.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/

/* Status LED aliases (mapped on the gpio_default PG3 output) */
#define MX_STATUS_LED_GPIO_PORT      PG3_PORT
#define MX_STATUS_LED_PIN            PG3_PIN
#define MX_STATUS_LED_ACTIVE_STATE   PG3_ACTIVE_STATE
#define MX_STATUS_LED_INACTIVE_STATE PG3_INACTIVE_STATE

/* ########### Aliases to initialization functions ########### */

  /* ***********************************************************
      GPDMA1_CH0: aliases for the data handling initialization functions
    *********************************************************** */

/* Right aligned zero padding */
#define mx_example_dma_init_right_align_zero_padding   mx_gpdma1_ch0_cfg1_init
/* Right aligned sign extended data */
#define mx_example_dma_init_right_align_sign_extend    mx_gpdma1_ch0_cfg2_init
/* Right aligned left truncated data */
#define mx_example_dma_init_right_align_left_trunc     mx_gpdma1_ch0_cfg3_init
/* Left aligned right truncated data */
#define mx_example_dma_init_left_align_right_trunc     mx_gpdma1_ch0_cfg4_init
/* Source byte exchange */
#define mx_example_dma_init_src_byte_exchange          mx_gpdma1_ch0_cfg5_init
/* Destination byte exchange */
#define mx_example_dma_init_dest_byte_exchange         mx_gpdma1_ch0_cfg6_init
/* Destination half-word exchange */
#define mx_example_dma_init_dest_halfword_exchange     mx_gpdma1_ch0_cfg7_init

  /* ***********************************************************
      gpio_default: aliases for initialization functions
    *********************************************************** */
#define mx_status_led_init                             mx_gpio_default_init

/* ########################################################### */

/* ########### Aliases to De-Initialization functions ########### */

#define mx_example_dma_deinit                          mx_gpdma1_ch0_deinit
#define mx_status_led_deinit                           mx_gpio_default_deinit

/* ########################################################### */

/* ########### Aliases to get HAL handle functions ########### */

#define mx_example_dma_gethandle                       mx_gpdma1_ch0_gethandle

/* ########################################################### */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_HAL_DEF_H */
