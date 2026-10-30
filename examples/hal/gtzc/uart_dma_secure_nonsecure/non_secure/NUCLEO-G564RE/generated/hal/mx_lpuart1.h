/**
  ******************************************************************************
  * @file           : mx_lpuart1.h
  * @brief          : Header for mx_lpuart1.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_stm32g5xx_hal_drivers_license.md file
  * in the same directory as the generated code.
  * If no mx_lpuart1.c comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_LPUART1_H
#define MX_LPUART1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** Primary aliases for LPUART1_RX pin */
#define DBG_VCP_RX_PORT                       HAL_GPIOA
#define DBG_VCP_RX_PIN                        HAL_GPIO_PIN_3

/** Primary aliases for LPUART1_TX pin */
#define DBG_VCP_TX_PORT                       HAL_GPIOA
#define DBG_VCP_TX_PIN                        HAL_GPIO_PIN_2

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for UART in HAL layer */
/******************************************************************************/
hal_uart_handle_t *mx_lpuart1_uart_init(void);
void mx_lpuart1_uart_deinit(void);
hal_uart_handle_t *mx_lpuart1_uart_gethandle(void);

/******************************************************************************/
/*                          LPUART1 global interrupt                          */
/******************************************************************************/
void LPUART1_IRQHandler(void);

/******************************************************************************/
/*                     GPDMA1 channel 1 global interrupt                      */
/******************************************************************************/
void GPDMA1_CH1_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_LPUART1_H */