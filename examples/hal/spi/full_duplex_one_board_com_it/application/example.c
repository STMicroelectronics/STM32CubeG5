/**
  ******************************************************************************
  * file           : example.c
  * brief          : A full-duplex SPI communication in interrupt mode,
                     on one single board, using the HAL API.
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
#include <string.h> /* importing memcmp and memset functions */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* @user: set the maximum number of attempts to complete a data transfer between the two boards */
#define MAX_COM_ATTEMPTS 10U
/* @user: set the payload size in bytes. The SPI message is a string made of this payload and terminated by '\0'. */
#define SPI_PAYLOAD_SIZE 67U
/** Size of the TX and RX buffers in bytes.
  * +1 is used to store the null character as the transmitted SPI message is a null-terminated string.
  */
#define BUFFER_SIZE (SPI_PAYLOAD_SIZE + 1U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

hal_spi_handle_t *pSPI_Controller;  /* pointer referencing the SPI controller handle from the generated code */
hal_spi_handle_t *pSPI_Responder;  /* pointer referencing the SPI responder handle from the generated code */

volatile uint8_t ControllerTxRxTransferComplete;  /* Set to 1 if a transfer is complete for the controller */
volatile uint8_t ResponderTxRxTransferComplete;   /* Set to 1 if a transfer is complete for the responder */
volatile uint8_t ControllerTransferError;         /* Set to 1 if a transfer error is detected for the controller */
volatile uint8_t ResponderTransferError;          /* Set to 1 if a transfer error is detected for the responder */

/** BufferFromController, BufferFromResponder: fixed-size buffers to transfer alternately.
  * @user: it is possible to modify the messages content and length, update SPI_PAYLOAD_SIZE if necessary.
  */
static const uint8_t BufferFromControllerA[BUFFER_SIZE] =
  "SPI Full Duplex One Board Communication From Controller - Message A";
static const uint8_t BufferFromControllerB[BUFFER_SIZE] =
  "SPI Full Duplex One Board Communication From Controller - Message B";
static const uint8_t BufferFromResponderA[BUFFER_SIZE]  =
  "SPI Full Duplex One Board Communication From Responder  - Message A";
static const uint8_t BufferFromResponderB[BUFFER_SIZE]  =
  "SPI Full Duplex One Board Communication From Responder  - Message B";

/* Pointer to the buffer used for transmission */
const uint8_t *pTxDataController;
const uint8_t *pTxDataResponder;

/* Buffer used for reception */
uint8_t RxBufferController[BUFFER_SIZE] = {0U};
uint8_t RxBufferResponder[BUFFER_SIZE] = {0U};

/* Private functions prototype -----------------------------------------------*/
static inline void UpdateBuffers(void);
static app_status_t HandleTransferCplt(void);
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t spi_error_code, hal_spi_handle_t *pSPI);

/* Functions allowing the user to configure dunamically the SPI callbacks instead of weak functions */
static void ControllerTxRxCpltCallback(hal_spi_handle_t *pSPI);
static void ControllerTransferErrorCallback(hal_spi_handle_t *pSPI);
static void ResponderTxRxCpltCallback(hal_spi_handle_t *pSPI);
static void ResponderTransferErrorCallback(hal_spi_handle_t *pSPI);


/** ########## Step 1 ##########
  * The applicative code initializes the two SPI instances and registers the user callbacks.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Initialize the SPI instances */
  pSPI_Controller = mx_spi_controller_init();
  pSPI_Responder = mx_spi_responder_init();

  if ((pSPI_Controller == NULL) || (pSPI_Responder == NULL))
  {
    goto _app_init_exit;
  }

  /* Register the SPI callbacks for the controller */
  if (HAL_SPI_RegisterTxRxCpltCallback(pSPI_Controller, ControllerTxRxCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_SPI_RegisterErrorCallback(pSPI_Controller, ControllerTransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Register the SPI callbacks for the responder */
  if (HAL_SPI_RegisterTxRxCpltCallback(pSPI_Responder, ResponderTxRxCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_SPI_RegisterErrorCallback(pSPI_Responder, ResponderTransferErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_status_t hal_status;    /* memorizes the HAL status of the SPI TX/RX operations */
  uint32_t com_attempts = 0U; /* memorizes the number of attempts made to exchange a message between the boards */
  uint32_t spi_error_code;    /* memorizes the SPI error code limited to the last process */

  UpdateBuffers();

  while ((com_attempts < MAX_COM_ATTEMPTS) && (return_status == EXEC_STATUS_ERROR))
  {
    /* Clear the SPI interrupts flags before starting a new transfer */
    ControllerTxRxTransferComplete = 0U;
    ResponderTxRxTransferComplete = 0U;
    ControllerTransferError = 0U;
    ResponderTransferError = 0U;

    /* Start a new communication attempt */
    com_attempts ++;

    /** ########## Step 2 ##########
      * The responder prepares the communication in interrupt mode in full duplex.
      */
    hal_status = HAL_SPI_TransmitReceive_IT(pSPI_Responder, pTxDataResponder, RxBufferResponder, BUFFER_SIZE);
    if (hal_status != HAL_OK)
    {
      /* Failure of HAL API here must be due to a wrong configuration and cannot be recovered */
      PRINTF("[ERROR] Responder -  Unrecoverable configuration error");
      goto _app_process_exit;
    }

    /** ########## Step 3 ##########
      * The controller prepares the communication in interrupt mode in full duplex.
      */
    hal_status = HAL_SPI_TransmitReceive_IT(pSPI_Controller, pTxDataController, RxBufferController, BUFFER_SIZE);
    if (hal_status != HAL_OK)
    {
      /* Failure of HAL API here must be due to a wrong configuration and cannot be recovered */
      PRINTF("[ERROR] Controller -  Unrecoverable configuration error");
      goto _app_process_exit;
    }

    /** ########## Step 4 ##########
      * Waits for one of these SPI interrupts: write-read transfer complete for both controller and responder,
      * or transfer error for one of the SPI instances.
      */
    while (((ControllerTxRxTransferComplete == 0U) || (ResponderTxRxTransferComplete == 0U))
           && ((ControllerTransferError == 0U) && (ResponderTransferError == 0U)))
    {
      /** Put the CPU in Wait For Interrupt state. A SPI interrupt or a SystTick interrupt can wake up the CPU.
        * @user: This process is used to illustrate the interest of the interrupts. It can be replaced by your own code.
        */
      __WFI();
    }

    if (ControllerTransferError == 1U)
    {
      /* The write-read transfer is not correctly completed. Report the error, then restart the communication */
      spi_error_code = HAL_SPI_GetLastErrorCodes(pSPI_Controller);
      return_status  = HandleTransferError(hal_status, spi_error_code, pSPI_Controller);
      continue;
    }

    if (ResponderTransferError == 1U)
    {
      /* The write-read transfer is not correctly completed. Report the error, then restart the communication */
      spi_error_code = HAL_SPI_GetLastErrorCodes(pSPI_Responder);
      return_status  = HandleTransferError(hal_status, spi_error_code, pSPI_Responder);
      continue;
    }

    /** ########## Step 5 ##########
      * Checks the correctness of the data when the transfer is completed.
      */
    return_status = HandleTransferCplt();

  }

_app_process_exit:
  return return_status;
} /* end app_process */


/** SPI Controller TX/RX Transfer complete callback
  * @user: This implementation of the SPI TX/RX transfer complete callback can be customized.
  * This function is executed if the SPI Controller Tx/Rx transfer complete interrupt is generated.
  */
static void ControllerTxRxCpltCallback(hal_spi_handle_t *hspi)
{
  /* Asynchronous processing related to step 4 */
  ControllerTxRxTransferComplete = 1U;
}

/** SPI Controller transfer error callback
  * @user: This implementation of the SPI transfer error callback can be customized.
  * This function is executed if the SPI transfer error interrupt is generated.
  */
static void ControllerTransferErrorCallback(hal_spi_handle_t *hspi)
{
  /* Asynchronous processing related to step 4 */
  ControllerTransferError = 1U;
}

/** SPI Responder TX/RX Transfer complete callback
  * @user: This implementation of the SPI TX/RX transfer complete callback can be customized.
  * This function is executed if the SPI Responder Tx/Rx transfer complete interrupt is generated.
  */
static void ResponderTxRxCpltCallback(hal_spi_handle_t *hspi)
{
  /* Asynchronous processing related to step 4 */
  ResponderTxRxTransferComplete = 1U;
}

/** SPI Responder transfer error callback
  * @user: This implementation of the SPI transfer error callback can be customized.
  * This function is executed if the SPI transfer error interrupt is generated.
  */
static void ResponderTransferErrorCallback(hal_spi_handle_t *hspi)
{
  /* Asynchronous processing related to step 4 */
  ResponderTransferError = 1U;
}


/** ########## Step 6 ##########
  * Deinitializes the SPI instances before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  PRINTF("[INFO] Step 6: de-init \n");

  mx_spi_controller_deinit();
  mx_spi_responder_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** brief:  Updates the TX buffer pointer and clears the RX buffer before starting a new transfer.
  * param:  none
  * retval: none
  */
static inline void UpdateBuffers(void)
{
  /* Toggle between the buffer A and buffer B to select the message to send for both controller and responder */
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

  if (memcmp(pTxDataController, RxBufferResponder, BUFFER_SIZE) == 0)
  {

    if (memcmp(pTxDataResponder, RxBufferController, BUFFER_SIZE) == 0)
    {
      PRINTF("[INFO] Step 5: Data received CORRECT from both controller and responder.\n");
      return_status = EXEC_STATUS_OK;
    }
    else
    {
      PRINTF("[ERROR] Controller - Buffer received DIFFERENT.\n");
    }
  }
  else
  {
    PRINTF("[ERROR] Responder - Buffer received DIFFERENT.\n");
  }

  return return_status;
} /* end HandleTransferCplt */


/** brief: This function is executed in case of a data transfer error.
  * @user: - This function implementation only illustrates error processing.
  *        - It can be customized to match the application recovery strategy.
  * param hal_status:  HAL status of the SPI TX/RX operations.
  * param spi_error_code:  SPI Error Code.
  * retval: example status
  */
static app_status_t HandleTransferError(uint32_t hal_status, uint32_t spi_error_code, hal_spi_handle_t *pSPI)
{
  if (pSPI == pSPI_Controller)
  {
    PRINTF("[ERROR] Controller - Communication ERROR: hal_status = %" PRIu32 ", \
    HAL_SPI_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n", hal_status, spi_error_code);
  }
  else if (pSPI == pSPI_Responder)
  {
    PRINTF("[ERROR] Responder - Communication ERROR: hal_status = %" PRIu32 ", \
    HAL_SPI_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n", hal_status, spi_error_code);
  }
  else
  {
    PRINTF("[ERROR] Unknown SPI instance - Communication ERROR: hal_status = %" PRIu32 ", \
    HAL_SPI_GetLastErrorCodes = %" PRIu32 ". TRYING AGAIN.\n", hal_status, spi_error_code);
  }

  /* Take the time to check the setup before restarting the communication */
  HAL_Delay(1000U);

  return EXEC_STATUS_ERROR;
} /* end HandleTransferError */
