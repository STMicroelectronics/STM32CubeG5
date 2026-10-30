/**
  ******************************************************************************
  * file           : example.c
  * brief          : LPUART DMA password validation scenario.
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

/* Includes ------------------------------------------------------------------*/
#include "example.h"
#include "secure_nsclib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* @user: set the maximum number of attempts to complete a data transfer between the two boards */
#define BUFFER_SIZE   (50U)

/* @user: The maximum data bus width used by DMA in STM32 devices is 64 bits.
   Therefore, 8-byte alignment is the minimum recommended alignment for DMA buffers across STM32 devices. */
#define DMA_ALIGNMENT (8U)

/* @user: The maximum uart transmission timeout in milli-second. */
#define UART_POLL_TIMEOUT_MS (10000U)

/* Private variables ---------------------------------------------------------*/
hal_uart_handle_t *pUART; /* pointer referencing the UART handle from the generated code */

/* DMA reception buffer and a contiguous copy used by the secure service. */
__attribute__((section("non_cacheable_area"), aligned(DMA_ALIGNMENT)))
static uint8_t tx_buffer[BUFFER_SIZE];
__attribute__((section("non_cacheable_area"), aligned(DMA_ALIGNMENT)))
static uint8_t rx_buffer[BUFFER_SIZE];

/* Messages buffers */
static const uint8_t EnterPasswordMsg[]   = "Enter the Password:\r\n";
static const uint8_t CorrectPasswordMsg[] = "Correct password, end of the example\r\n";
static const uint8_t WrongPasswordMsg[]   = "Wrong Password, try again:\r\n";

/* Transmission completion state updated by the UART receive callback. */
static volatile uint32_t received_char_count;
static volatile uint8_t  rx_transfer_complete;

/* Private functions prototype -----------------------------------------------*/
/* UART Rx complete callback */
static void RxReceiverCpltCallback(hal_uart_handle_t *huart, uint32_t size_byte, hal_uart_rx_event_types_t rx_event);

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * The init of the UART and DMA instances in circular mode are triggered by the applicative code.
    * The user callbacks for I2C master TX/RX transfer completed and transfer error are registered.
    */
  pUART = mx_example_uart_init();
  if (pUART == NULL)
  {
    goto _app_init_exit;
  }

  if (HAL_UART_RegisterRxCpltCallback(pUART, RxReceiverCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /** ########## Step 2 ##########
    * Start the reception of user password in DMA circular mode.
    */
  if (HAL_UART_ReceiveUntilCM_DMA(pUART, rx_buffer, sizeof(rx_buffer), '\r') != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_UART_Transmit(pUART, EnterPasswordMsg, sizeof(EnterPasswordMsg) - 1U, UART_POLL_TIMEOUT_MS) != HAL_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
}

app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  uint8_t password_ok = 0U;

  /* Process completed DMA transfers until the secure service accepts input. */
  while (password_ok == 0U)
  {
    /** ########## Step 3 ##########
      * Receive next user password in DMA circular mode.
      */
    while (rx_transfer_complete == 0U)
    {
      __WFI();
    }

    /* Echo exactly the bytes completed by the circular DMA reception. */
    if (HAL_UART_Transmit(pUART, tx_buffer, received_char_count, UART_POLL_TIMEOUT_MS) != HAL_OK)
    {
      goto _app_process_exit;
    }
    if (HAL_UART_Transmit(pUART, (const uint8_t *)"\r\n", sizeof("\r\n") - 1U, UART_POLL_TIMEOUT_MS) != HAL_OK)
    {
      goto _app_process_exit;
    }

    /** ########## Step 4 ##########
      * Check the correctness of the received user password through non-secure callable.
      * If the user password is correct, end the example.
      * If the user password is wrong, return to the step 3.
      */
    password_ok = (uint8_t)IsPasswordCorrect(tx_buffer, received_char_count);
    if (password_ok != 0U)
    {
      if (HAL_UART_Transmit(pUART, CorrectPasswordMsg, sizeof(CorrectPasswordMsg) - 1U, UART_POLL_TIMEOUT_MS) != HAL_OK)
      {
        goto _app_process_exit;
      }

      return_status = EXEC_STATUS_OK;
    }
    else
    {
      if (HAL_UART_Transmit(pUART, WrongPasswordMsg, sizeof(WrongPasswordMsg) - 1U, UART_POLL_TIMEOUT_MS) != HAL_OK)
      {
        goto _app_process_exit;
      }

      rx_transfer_complete = 0U;
    }
  }

_app_process_exit:
  return return_status;
}


/** De-initializes the UART and DMA instances before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_uart_deinit();

  return EXEC_STATUS_OK;
}


/** UART Rx transfer complete callback
  * @user This implementation of the UART Rx transfer complete callback can be customized.
  * This function is executed if the UART Rx transfer complete interrupt is generated.
  */
static void RxReceiverCpltCallback(hal_uart_handle_t *huart, uint32_t size_byte, hal_uart_rx_event_types_t rx_event)
{
  static uint32_t old_position;
  uint32_t index;

  STM32_UNUSED(huart);
  STM32_UNUSED(rx_event);

  /* Convert the circular-DMA position into a contiguous password buffer. */
  if (size_byte != old_position)
  {
    if (size_byte > old_position)
    {
      received_char_count = size_byte - old_position;
      for (index = 0U; index < received_char_count; index++)
      {
        tx_buffer[index] = rx_buffer[old_position + index];
      }
    }
    else
    {
      /* Reception wrapped around the end of the circular DMA buffer. */
      received_char_count = BUFFER_SIZE - old_position;
      for (index = 0U; index < received_char_count; index++)
      {
        tx_buffer[index] = rx_buffer[old_position + index];
      }

      for (index = 0U; index < size_byte; index++)
      {
        tx_buffer[received_char_count + index] = rx_buffer[index];
      }

      received_char_count += size_byte;
    }

    rx_transfer_complete = 1U;
  }

  /* Preserve this position to calculate the next completed transfer span. */
  old_position = size_byte;
}
