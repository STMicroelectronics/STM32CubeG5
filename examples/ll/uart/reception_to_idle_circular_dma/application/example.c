/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use the LL UART API for reception to IDLE event in
  *                  circular DMA mode.
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
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* @user: set the maximum number of attempts to complete a data transfer between the two boards */
#define MAX_COM_ATTEMPTS 10U
#define TIMEOUT 1000U /* Timeout period, in milliseconds, of the UART on transmission of a character */

/** The maximum size of the reception buffer in bytes.
  * @user: Set the maximum size of the buffer.
  */
#define BUFFER_SIZE 50U

#define DMA_ALIGNMENT      (8U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Text string printed on the PC Com port for user information */
__attribute__((aligned(DMA_ALIGNMENT)))
static const uint8_t TxStartMessage[] = "\r\nUART Example : Enter characters to fill reception buffers.\r\n";

/** Buffer shared by the CPU and DMA for UART reception.
  * On devices enabling the data cache, this section placement avoids cache coherency issues.
  */
__attribute__((section(".non_cacheable_variables"), aligned(1)))
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

/* Buffer used to store data ready for user loopback */
uint8_t TxBuffer[BUFFER_SIZE] = {0U};

/* Size of the received buffer */
uint32_t NbReceivedChars;

volatile uint8_t RxTransferComplete; /* Set to 1 if the read transfer is correctly completed */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
volatile uint8_t TransferError;      /* Set to 1 if a transfer error is detected              */
#endif /* USE_LL_APP_ERROR */

/* Flag set in RX transfer complete callback to trigger data transmission (loopback) in app_process() */
volatile uint8_t TxUartFlag;

/* Position already consumed by the application in the circular RX buffer. */
uint32_t OldPos;

/* Private functions prototype -----------------------------------------------*/
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
static app_status_t HandleTransferError(void);
#endif /* USE_LL_APP_ERROR */

/** ########## Step 1 ##########
  * The init of the MX_UARTx instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_uartx_init() != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  uint32_t com_attempts = 0U;

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Clear the transfer flags before starting a new communication attempt */
    RxTransferComplete = 0U;
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    TransferError = 0U;
#endif /* USE_LL_APP_ERROR */
    TxUartFlag = 0U;
    NbReceivedChars = 0U;
    OldPos = 0U;

    /* Start a new communication attempt */
    com_attempts++;

    /** ########## Step 2 ##########
      * Sends an information message to the user via the serial communication port (PC Com port).
      */
    if (UART_Transmit(TxStartMessage, sizeof(TxStartMessage) - 1U, TIMEOUT) != SYSTEM_OK)
    {
      /* An error occurs at the startup of the UART transmission. Restart the communication. */
      continue;
    }

    /** ########## Step 3 ##########
      * Starts continuous data reception via UART using circular DMA till either the configured amount
      * of data is received or an IDLE event occurs.
      */
    if (UART_ReceiveToIdleCircular_DMA(RxBuffer, BUFFER_SIZE) != SYSTEM_OK)
    {
      /* An error occurs at the startup of the UART reception. Restart the communication. */
      continue;
    }

    /** ########## Step 4 ##########
      * Waits for one of these UART interrupts: read transfer complete or transfer error.
      */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    while ((RxTransferComplete == 0U) && (TransferError == 0U))
#else
    while (RxTransferComplete == 0U)
#endif /* USE_LL_APP_ERROR */
    {
      /** Put the CPU in Wait For Interrupt state. A UART or DMA interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of interrupts. It can be replaced by your own code.
        */
      __WFI();
    }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    if (TransferError == 1U)
    {
      /* Error while the UART reception is ongoing. Restart the communication. */
      continue;
    }
#endif /* USE_LL_APP_ERROR */

    /** ########## Step 5 ##########
      * The received data are sent back on UART Tx (loopback)
      */
    if (TxUartFlag == 1U)
    {
      if (UART_Transmit(TxBuffer, NbReceivedChars, TIMEOUT) != SYSTEM_OK)
      {
        /* Error while the UART transmission is ongoing. Restart the communication. */
        continue;
      }
    }
    else
    {
      /* It handles a transfer error */
      goto _app_process_exit;
    }

    return_status = EXEC_STATUS_OK;

_app_process_exit:
    return return_status;
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if (TransferError == 1U)
  {
    return_status = HandleTransferError();
  }
#endif /* USE_LL_APP_ERROR */

  return return_status;
} /* end app_process */


/** Deinitializes the MX_UARTx instance.
  */
app_status_t app_deinit(void)
{
  mx_uartx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** This function is executed when a UART Rx event is generated.
  * @user: the Rx transfer callback can be customized according to the application needs.
  */
void UART_RxTransferCpltCallback(uint32_t size_byte, uint32_t rx_event)
{
  uint32_t i;

  (void)rx_event;

  /* Check if the number of received data in the reception buffer has changed. */
  if (size_byte != OldPos)
  {
    /* Set the RxTransferComplete to 1 if the read transfer is correctly completed */
    RxTransferComplete = 1U;
    /* Check if position of index in reception buffer has simply be increased
       or if end of buffer has been reached */
    if (size_byte > OldPos)
    {
      /* Current position is higher than the previous one. */
      NbReceivedChars = size_byte - OldPos;
      /* Copy received data in "User" buffer for evacuation */
      for (i = 0U; i < NbReceivedChars; i++)
      {
        TxBuffer[i] = RxBuffer[OldPos + i];
      }
    }
    else
    {
      /* Current position is lower than previous one : end of buffer has been reached */
      /* First copy data from current position till end of buffer */
      NbReceivedChars = BUFFER_SIZE - OldPos;
      /* Copy received data in "User" buffer for evacuation */
      for (i = 0U; i < NbReceivedChars; i++)
      {
        TxBuffer[i] = RxBuffer[OldPos + i];
      }

      if (size_byte > 0U)
      {
        for (i = 0U; i < size_byte; i++)
        {
          TxBuffer[NbReceivedChars + i] = RxBuffer[i];
        }
        NbReceivedChars += size_byte;
      }
    }

    /* Set the TX flag to 1 once the buffer is ready to be sent back to the user.
       The actual transmission is handled in app_process() to avoid doing it inside the IRQ handler. */
    TxUartFlag = 1U;
  }

  /* Update OldPos as new reference of position in User Rx buffer that
     indicates position to which data have been processed. */
  OldPos = size_byte;
}


/** This function is executed when the transfer error interrupt is generated.
  * @user: the Error transfer callback can be customized according to the application needs.
  */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
void UART_TransferErrorCallback(void)
{
  TransferError = 1U;
}
#endif /* USE_LL_APP_ERROR */


/** brief:  This function is executed in case of a data transfer error.
  *  user:  - This function implementation only illustrates error processing.
  *         - It can be customized to match the application recovery strategy.
  * retval: example status
  */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
static app_status_t HandleTransferError(void)
{
  /* Take the time to check the setup before restarting the communication. */
  DelayMs(1000U);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
#endif /* USE_LL_APP_ERROR */
