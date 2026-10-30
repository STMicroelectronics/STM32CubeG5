/**
  ******************************************************************************
  * file           : example.c
  * brief          : The responder's application handling a data transfer over
  *                  I2C in interrupt mode with LL API.
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
#include <string.h> /* importing memcmp and memset functions */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* @user: set the maximum number of attempts to complete a data transfer between the two boards */
#define MAX_COM_ATTEMPTS  (10U)

/* @user: set the payload size in bytes. The I2C message is a string made of this payload and terminated by '\0'. */
#define I2C_PAYLOAD_SIZE  (40U)

/** Size of the TX and RX buffers in bytes.
  * +1 is used to store the null character as the transmitted I2C message is a null-terminated string.
  */
#define BUFFER_SIZE       (I2C_PAYLOAD_SIZE + 1U)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Buffer used for reception (sent back to the controller) */
uint8_t RxBuffer[BUFFER_SIZE] = {0U};

volatile uint8_t TxTransferComplete; /* Set to 1 if the write transfer is correctly completed */
volatile uint8_t RxTransferComplete; /* Set to 1 if the read transfer is correctly completed */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
volatile uint8_t TransferError;      /* Set to 1 if a transmission or a reception error is detected */
#endif /* USE_LL_APP_ERROR */

/* Private functions prototype -----------------------------------------------*/
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
static app_status_t HandleTransferError(void);
#endif /* USE_LL_APP_ERROR */

/** ########## Step 1 ##########
  * The init of the MX_I2Cx instance is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (mx_i2cx_init() != NULL)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  uint32_t com_attempts = 0U; /* memorizes the number of attempts made to exchange a message between the boards */


  /* Clear the Rx buffer before starting a new transfer */
  memset(RxBuffer, 0U, BUFFER_SIZE);

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Clear the MX_I2Cx interrupts flags before starting a new transfer */
    TxTransferComplete = 0U;
    RxTransferComplete = 0U;
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    TransferError = 0U;
#endif /* USE_LL_APP_ERROR */

    /* Start a new communication attempt */
    com_attempts ++;

    /** ########## Step 2 ##########
      * Receives in interrupt mode a buffer containing a null-terminated string using MX_I2Cx.
      */
    I2C_Receive_IT(RxBuffer, BUFFER_SIZE);

    /** ########## Step 3 ##########
      * Waits for one of these MX_I2Cx interrupts: read transfer complete or transfer error.
      */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    while ((RxTransferComplete == 0) && (TransferError == 0))
#else
    while (RxTransferComplete == 0)
#endif /* USE_LL_APP_ERROR */
    {
      /** Put the CPU in Wait For Interrupt state. An MX_I2Cx interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
        */
      __WFI();
    }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    if (TransferError == 1)
    {
      /* Handle MX_I2Cx data transmission error, then restart the communication */
      return_status  = HandleTransferError();
      continue;
    }
#endif /* USE_LL_APP_ERROR */
    /** ########## Step 4 ##########
      * Transmits in interrupt mode the received buffer, containing a null-terminated string, using MX_I2Cx.
      */
    I2C_Transmit_IT(RxBuffer, BUFFER_SIZE);


    /** ########## Step 5 ##########
      * Waits for one of these MX_I2Cx interrupts: write transfer complete or transfer error.
      */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    while ((TxTransferComplete == 0) && (TransferError == 0))
#else
    while (TxTransferComplete == 0)
#endif /* USE_LL_APP_ERROR */
    {
      /** Put the CPU in Wait For Interrupt state. An MX_I2Cx interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
        */
      __WFI();
    }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    if (TransferError == 1)
    {
      /* Handle MX_I2Cx data transmission error, then restart the communication */
      return_status  = HandleTransferError();
      continue;
    }
#endif /* USE_LL_APP_ERROR */

    /* Report the communication status: Data Transfer completed */
    return_status = EXEC_STATUS_OK;

  } /* end while */

  return return_status;
} /* end app_process */


/** De-initializes the MX_I2Cx instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_i2cx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** Reception end of transfer callback
  * @user This implementation of the MX_I2Cx Rx transfer complete callback can be customized.
  * This function is executed if the Rx transfer complete interrupt is generated.
  */
void I2C_RxTransferCpltCallback(void)
{
  /* Asynchronous processing related to step 3 */
  RxTransferComplete = 1U;
}


/** Transmission end of transfer callback
  * @user This implementation of the MX_I2Cx Tx transfer complete callback can be customized.
  * This function is executed if the Tx transfer complete interrupt is generated.
  */
void I2C_TxTransferCpltCallback(void)
{
  /* Asynchronous processing related to step 5 */
  TxTransferComplete = 1U;
}


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** MX_I2Cx transfer error callback
  * @user This implementation of the MX_I2Cx transfer error callback can be customized.
  * This function is executed if the transfer error interrupt is generated.
  */
void I2C_TransferErrorCallback(void)
{
  /* Asynchronous processing related to step 3 and 5 */
  TransferError = 1U;
}


/** brief:  This function is executed in case of a data transfer error.
  *  user:  - This function implementation only illustrates error processing.
  *         - It can be customized to match the application recovery strategy.
  * retval: example status
  */
static app_status_t HandleTransferError(void)
{
  /* Take the time to check the setup before restarting the communication */
  DelayMs(1000U);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
#endif /* USE_LL_APP_ERROR */
