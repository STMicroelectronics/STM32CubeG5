/**
  ******************************************************************************
  * file           : example.c
  * brief          : A half-duplex SPI communication in interrupt mode,
  *                  on a single board using the LL API.
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
/* @user: set the maximum number of attempts to complete a data transfer */
#define MAX_COM_ATTEMPTS  (10U)
/* @user: set the payload size in bytes. The SPI message is a string made of this payload and terminated by '\0'. */
#define SPI_PAYLOAD_SIZE  (67U)
/** Size of the TX and RX buffers in bytes.
  * +1 is used to store the null character as the transmitted SPI message is a null-terminated string.
  */
#define BUFFER_SIZE (SPI_PAYLOAD_SIZE + 1U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t ControllerComplete;  /* Set to 1 when the controller transfer is completed */
volatile uint8_t ResponderComplete;   /* Set to 1 when the responder transfer is completed */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
volatile uint8_t ControllerError;     /* Set to 1 if a transfer error is detected for the controller */
volatile uint8_t ResponderError;      /* Set to 1 if a transfer error is detected for the responder */
#endif /* USE_LL_APP_ERROR */

/** Buffers transmitted by the controller and the responder.
  * @user: it is possible to modify the messages content and length, update SPI_PAYLOAD_SIZE if necessary.
  */
static const uint8_t BufferFromControllerA[BUFFER_SIZE] =
  "SPI Half Duplex One Board Communication From Controller - Message A";
static const uint8_t BufferFromControllerB[BUFFER_SIZE] =
  "SPI Half Duplex One Board Communication From Controller - Message B";
static const uint8_t BufferFromResponderA[BUFFER_SIZE]  =
  "SPI Half Duplex One Board Communication From Responder  - Message A";
static const uint8_t BufferFromResponderB[BUFFER_SIZE]  =
  "SPI Half Duplex One Board Communication From Responder  - Message B";

/* Pointer to the buffer currently used for transmission by each role */
const uint8_t *pTxDataController;
const uint8_t *pTxDataResponder;
/* Buffers used for reception */
uint8_t RxBufferController[BUFFER_SIZE] = {0U};
uint8_t RxBufferResponder[BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/
static inline void UpdateBuffers(void);
static app_status_t HandleTransferCplt(void);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
static app_status_t HandleTransferError(void);
#endif /* USE_LL_APP_ERROR */


/** ########## Step 1 ##########
  * The applicative code initializes the two SPI instances
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if ((mx_spix_controller_init() != NULL) && (mx_spix_responder_init() != NULL))
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  uint32_t com_attempts = 0U; /* memorizes the number of attempts made to transfer the message */
  app_status_t return_status = EXEC_STATUS_ERROR;

  UpdateBuffers();

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Clear the SPI interrupts flags before starting a new transfer */
    ControllerComplete = 0U;
    ResponderComplete  = 0U;
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    ControllerError = 0U;
    ResponderError  = 0U;
#endif /* USE_LL_APP_ERROR */

    /* Start a new communication attempt */
    com_attempts++;

    /** ########## Step 2 ##########
      * The responder prepares the communication in half-duplex receive mode, using interrupts,
      * waiting for the controller to initiate the clock and data transmission.
      */
    SPI_HalfDuplexResponderReceive_IT(RxBufferResponder, BUFFER_SIZE);

    /** ########## Step 3 ##########
      * The controller initiates the communication in half-duplex transmit mode, using interrupts,
      * sending bytes to the slave device while the responder receives in parallel.
      * The SPI messages exchanged are null-terminated strings.
      */
    SPI_HalfDuplexControllerTransmit_IT((uint8_t *)pTxDataController, BUFFER_SIZE);

    /** ########## Step 4 ##########
      * Waits for the transfer complete interruption for both SPI instances,
      * or a transfer error interruption on one of them.
      */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    while (((ControllerComplete == 0U) || (ResponderComplete == 0U))
           && ((ControllerError == 0U) && (ResponderError == 0U)))
#else
    while ((ControllerComplete == 0U) || (ResponderComplete == 0U))
#endif /* USE_LL_APP_ERROR */
    {
      /** Put the CPU in Wait For Interrupt state. A SPI interrupt or a SysTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
        */
      __WFI();
    }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    if (ControllerError == 1U)
    {
      /* The controller transfer is not correctly completed. Report the error, then restart the communication */
      return_status = HandleTransferError();
      continue;
    }
    if (ResponderError == 1U)
    {
      /* The responder transfer is not correctly completed. Report the error, then restart the communication */
      return_status = HandleTransferError();
      continue;
    }
#endif /* USE_LL_APP_ERROR */

    /* Clear completion and error flags before the reverse communication phase */
    ControllerComplete = 0U;
    ResponderComplete  = 0U;
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    ControllerError = 0U;
    ResponderError  = 0U;
#endif /* USE_LL_APP_ERROR */

    /** ########## Step 5 ##########
      * The controller prepares the communication in half-duplex receive mode,
          * waiting for the responder to transmit.
          */
    SPI_HalfDuplexControllerReceive_IT(RxBufferController, BUFFER_SIZE);

    /** ########## Step 6 ##########
      * The responder transmits the message in half-duplex transmit mode.
      */
    SPI_HalfDuplexResponderTransmit_IT((uint8_t *)pTxDataResponder, BUFFER_SIZE);

    /** ########## Step 7 ##########
      * Waits for completion of both SPI instances, or a transfer error.
      */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    while (((ControllerComplete == 0U) || (ResponderComplete == 0U))
           && ((ControllerError == 0U) && (ResponderError == 0U)))
#else
    while ((ControllerComplete == 0U) || (ResponderComplete == 0U))
#endif /* USE_LL_APP_ERROR */
    {
      __WFI();
    }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    if (ControllerError == 1U)
    {
      return_status = HandleTransferError();
      continue;
    }
    if (ResponderError == 1U)
    {
      return_status = HandleTransferError();
      continue;
    }
#endif /* USE_LL_APP_ERROR */

    /** ########## Step 8 ##########
      * Checks the correctness of the received buffers after both half-duplex transfers are completed.
      */
    return_status = HandleTransferCplt();

  } /* end while */

  return return_status;
} /* end app_process */


/**
  * De-initializes the two SPI instances before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_spix_controller_deinit();
  mx_spix_responder_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** SPI Controller Transfer complete callback
  * user: This implementation of the SPI transfer complete callback can be customized.
  * This function is executed if the SPI Controller transfer complete interrupt is generated.
  */
void SPI_ControllerCpltCallback(void)
{
  /* Asynchronous processing related to step 4 */
  ControllerComplete = 1U;
}


/** SPI Responder reception complete callback
  * user: This implementation of the SPI reception complete callback can be customized.
  * This function is executed if the SPI Responder reception complete interrupt is generated.
  */
void SPI_ResponderCpltCallback(void)
{
  /* Asynchronous processing related to step 4 */
  ResponderComplete = 1U;
}


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** SPI transfer error callback
  * user: This implementation of the SPI transfer error callback can be customized.
  * This function is executed if the SPI transfer error interrupt is generated.
  */
void SPI_ControllerErrorCallback(void)
{
  /* Asynchronous processing related to step 4 */
  ControllerError = 1U;
}


/** SPI Receive error callback
  * user: This implementation of the SPI receive error callback can be customized.
  * This function is executed if the SPI receive error interrupt is generated.
  */
void SPI_ResponderErrorCallback(void)
{
  /* Asynchronous processing related to step 4 */
  ResponderError = 1U;
}
#endif /* USE_LL_APP_ERROR */


/** brief:  Updates the TX buffer pointer and clears the RX buffer before starting a new transfer.
  * param:  none
  * retval: none
  */
static inline void UpdateBuffers(void)
{
  /* Toggle between buffer A and B for both transfers */
  if (pTxDataController == BufferFromControllerA)
  {
    pTxDataController = BufferFromControllerB;
  }
  else
  {
    pTxDataController = BufferFromControllerA;
  }

  if (pTxDataResponder == BufferFromResponderA)
  {
    pTxDataResponder = BufferFromResponderB;
  }
  else
  {
    pTxDataResponder = BufferFromResponderA;
  }

  memset(RxBufferController, 0U, BUFFER_SIZE);
  memset(RxBufferResponder, 0U, BUFFER_SIZE);
} /* end UpdateBuffers */


/** brief:  Checks the correctness of the data when the transfer is completed.
  * param:  none
  * retval: example status
  */
static app_status_t HandleTransferCplt(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if ((memcmp(pTxDataController, RxBufferResponder, BUFFER_SIZE) == 0)
      && (memcmp(pTxDataResponder, RxBufferController, BUFFER_SIZE) == 0))
  {
    return_status = EXEC_STATUS_OK;
  }

  return return_status;
} /* end HandleTransferCplt */


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/** brief: This function is executed in case of a data transfer error.
  * user:  - This function implementation only illustrates error processing.
  *        - It can be customized to match the application recovery strategy.
  * retval: example status
  */
static app_status_t HandleTransferError(void)
{
  /* Take the time to check the setup before restarting the communication */
  DelayMs(1000U);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
#endif /* USE_LL_APP_ERROR */
