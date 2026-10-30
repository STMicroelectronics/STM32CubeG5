/**
  ******************************************************************************
  * @file    com.c
  * @brief   COM module.
  *          This file provides set of firmware functions to manage Com
  *          functionalities.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "com.h"

const char YMODEM_IT_MSG[] = "\r\nYmodem stop by printf\r\n";

static volatile uint32_t Ymodem = 0;       /*!< set to 1 when Ymodem uses UART> */

/**
  * @brief Receive Data.
  * @param uDataLength: Data pointer to the Data to receive.
  * @param uTimeout: Timeout duration.
  * @retval Status of the Receive operation.
  */
hal_status_t COM_Receive(uint8_t *Data, uint16_t uDataLength, uint32_t uTimeout)
{
  if (!Ymodem)
  {
    return HAL_UART_Receive(mx_example_usart_gethandle(), (uint8_t *)Data, uDataLength, uTimeout);
  }
  else
    return HAL_ERROR;
}

/**
  * @brief Receive Data function Y Modem.
  * @param uDataLength: Data pointer to the Data to receive.
  * @param uTimeout: Timeout duration.
  * @retval Status of the Receive operation.
  */
hal_status_t COM_Receive_Y(uint8_t *Data, uint16_t uDataLength, uint32_t uTimeout)
{
  if (Ymodem)
  {
    return HAL_UART_Receive(mx_example_usart_gethandle(), (uint8_t *)Data, uDataLength, uTimeout);
  }
  else
    return HAL_BUSY;
}

/**
  * @brief  Flush COM Input.
  * @param None.
  * @retval HAL_Status.
  */
hal_status_t COM_Flush(void)
{
  /* Clean the input path */
  LL_USART_RequestRxDataFlush((USART_TypeDef *)((uint32_t)mx_example_usart_gethandle()->instance));
  LL_USART_RequestTxDataFlush((USART_TypeDef *)((uint32_t)mx_example_usart_gethandle()->instance));
  return HAL_OK;
}

hal_status_t  COM_Y_On(void)
{
  hal_status_t status = HAL_ERROR;
  if (!Ymodem)
  {
    Ymodem = 1;
    status = HAL_OK;
  }
  return status;
}

hal_status_t  COM_Y_Off(void)
{
  hal_status_t status = HAL_ERROR;
  if (Ymodem)
  {
    Ymodem = 0;
    status = HAL_OK;
  }
  return status;
}

/**
  * @brief  Ymodem Header Packet Transfer completed callback.
  * @param  FileSize Dimension of the file that will be received.
  * @retval None
  */
__weak hal_status_t Ymodem_HeaderPktRxCpltCallback(uint32_t uFlashDestination, uint32_t uFileSize)
{

  /* NOTE : This function must not be modified, when the callback is needed,
            the Ymodem_HeaderPktRxCpltCallback could be implemented in the user file
   */
  return HAL_OK;
}

/**
  * @brief  Ymodem Data Packet Transfer completed callback.
  * @param  pData Pointer to the buffer.
  * @param  Size Packet dimension.
  * @retval None
  */
__weak hal_status_t Ymodem_DataPktRxCpltCallback(uint8_t *pData, uint32_t uFlashDestination, uint32_t uSize)
{

  /* NOTE : This function must not be modified, when the callback is needed,
            the Ymodem_DataPktRxCpltCallback could be implemented in the user file
   */
  return HAL_OK;
}

