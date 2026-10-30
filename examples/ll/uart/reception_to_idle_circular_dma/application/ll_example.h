/**
  ******************************************************************************
  * file           : ll_example.h
  * brief          : Header for stm32ynxx_ll_example.c files.
  *                  This file contains example-specific declarations to interface with example.c functions.
  ******************************************************************************
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LL_EXAMPLE_H
#define LL_EXAMPLE_H

/* Includes ------------------------------------------------------------------*/
#include "mx_hal_def.h"       /* aliases to the target-specific generated code */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#ifndef USE_LL_APP_TIMEOUT
#define USE_LL_APP_TIMEOUT 0U
#endif /* USE_LL_APP_TIMEOUT */

#ifndef USE_LL_APP_ERROR
#define USE_LL_APP_ERROR 0U
#endif /* USE_LL_APP_ERROR */

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** brief: Transmit MX_UARTx data in blocking mode.
  * param: p_data     Pointer to data buffer.
  * param: size_byte  Amount of data elements to be transmitted.
  * param: timeout_ms Timeout duration in milliseconds.
  * retval: system status (see system_status_t)
  */
system_status_t UART_Transmit(const void *p_data, uint32_t size_byte, uint32_t timeout_ms);

/** brief: Receive MX_UARTx data in circular DMA mode till either the expected number
  *         of data is received or an IDLE event occurs.
  * param: p_data    Pointer to data buffer.
  * param: size_byte Amount of data elements to be received.
  * retval: system status (see system_status_t)
  */
system_status_t UART_ReceiveToIdleCircular_DMA(void *p_data, uint32_t size_byte);

/** brief: MX_UARTx Rx transfer complete callback.
  * param: size_byte Amount of data available in the user reception buffer.
  * param: rx_event  Event that ended or marked the current reception chunk.
  */
void UART_RxTransferCpltCallback(uint32_t size_byte, uint32_t rx_event);

/** brief: MX_UARTx transfer error callback.
  */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
void UART_TransferErrorCallback(void);
#endif /* USE_LL_APP_ERROR */

/** brief: This function provides delay (in milliseconds) based on SysTick counter flag.
  * param: delay_ms Number of milliseconds to delay.
  */
void DelayMs(uint32_t delay_ms);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
