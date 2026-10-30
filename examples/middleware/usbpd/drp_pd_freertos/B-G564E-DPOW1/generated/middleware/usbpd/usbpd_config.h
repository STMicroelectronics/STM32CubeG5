
/**
  ******************************************************************************
  * @file    usbpd_config.h
  * @brief  Configuration for STM32 USBPD private aliases
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_usbpd_license.md file
  * in the root directory of this software component.
  * If no mx_usbpd_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USBPD_CONFIG_H
#define USBPD_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mx_hal_def.h"
#include "mx_tcpp0203.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define USBPD_VID  1155 /*!< Vendor ID (assigned by the USB-IF)               */
#define USBPD_PID  2 /*!< Product ID (assigned by the manufacturer)       */
#define USBPD_XID  4026531843 /*!< Value provided by the USB-IF assigned to the product   */
/******************************************************************************/
/*   CAD ALIASES   (private aliases)                                     */
/******************************************************************************/
/* port 0 */
#define usbpd_ucpd_0_gethandle        mx_ucpd1_gethandle
#define usbpd_it_0_enable()           HAL_CORTEX_NVIC_EnableIRQ(UCPD1_IRQn);
/******************************************************************************/
/*   PWR ALIASES   (private aliases)                                     */
/******************************************************************************/
/* port 0 */
#define usbpd_tcpp0203_0_getobject    mx_tcpp0203_0_getobject
#define usbpd_adc_gethandle           TCPP0203_0_ADC_GETHANDLE

/******************************************************************************/
/*   TIMERSERVER ALIASES   (private aliases)                                     */
/******************************************************************************/
/* timings in us */
#define usbpd_tim_us_gethandle        mx_tim1_gethandle
#define usbpd_tim_us_enable_clk       HAL_RCC_TIM1_EnableClock
#define usbpd_us_tim_disable_clk      HAL_RCC_TIM1_DisableClock
/******************************************************************************/
/*   TRACE ALIASES   (private aliases)                                     */
/******************************************************************************/
#define usbpd_trace_gethandle         mx_uart5_uart_gethandle
#define usbpd_trace_disable_clk       HAL_RCC_UART5_DisableClock
#define usbpd_trace_enable_clk        HAL_RCC_UART5_EnableClock
/* Exported functions ------------------------------------------------------- */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* USBPD_CONFIG_H */
