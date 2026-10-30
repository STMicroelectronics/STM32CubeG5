/**
  ******************************************************************************
  * file           : example.c
  * brief          : HiperfaceDSL position acquisition with LL API services
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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Variables for HiperfaceDSL encoder data computation to physical values */
/* Value of position from HiperfaceDSL encoder */
uint32_t encoder_position_millidegree = 0U;
/* Value of turn from HiperfaceDSL encoder */
uint32_t encoder_turn = 0U;

/* Value of the HiperfaceDSL online status words. Refer to the HiperfaceDSL documentation */
uint32_t online_status_d = 0U; /* Status for the drive interface */
uint32_t online_status_1 = 0U; /* Status for the safe 1 interface */
uint32_t online_status_2 = 0U; /* Status for the safe 2 interface */

/* Set to 1 when new data is received from the encoder, to notify the main loop that it can process the new data */
volatile uint32_t new_data_received = 0U;
/* Private functions prototype -----------------------------------------------*/

/**
  * The applicative code initializes the POSENC instance.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * The init of the MX_POSENCx instance is triggered by the applicative code.
    */
  if (mx_posencx_init() == NULL)
  {
    goto _app_init_exit;
  }

  if (mx_timx_init() == NULL)
  {
    goto _app_init_exit;
  }

  /* Wait for the system to be ready after initialization before starting the application. */
  DelayUs(100000U);

  /** ########## Step 2 ##########
    * Configure HiperfaceDSL communication and enable the fast-position interrupt.
    */
  if (POSENC_HiperfaceDSL_Start() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }

  /** ########## Step 3 ##########
    * Starts the timer
    */
  if (POSENC_TimerStart() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }

  /** ########## Step 4 ##########
    * Wait until the HiperfaceDSL link reports its first fast-position event.
    */
  if (POSENC_HiperfaceDSL_WaitForStartup() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_OK;

  /** ########## Step 5 ##########
    * Waits until a fast-position event is completed.
    * - Retrieves status and position from the HiperfaceDSL encoder.
    */

  /** Awaiting the conversion completion, the CPU can perform other tasks: it is not blocked by a polling loop.
    * For instance, it can enter Sleep mode and wait for the POSENC interruption to wake it up when the conversion is
    * completed.
    */
  while (new_data_received == 0U)
  {
    __WFI();
  }

  /* Reset the new data received flag */
  new_data_received = 0U;

  if (POSENC_HiperfaceDSL_ReadStatus(&online_status_d, &online_status_1, &online_status_2) != SYSTEM_OK)
  {
    return_status = EXEC_STATUS_ERROR;
  }

  if (POSENC_HiperfaceDSL_GetPosition(&encoder_turn, &encoder_position_millidegree) != SYSTEM_OK)
  {
    return_status = EXEC_STATUS_ERROR;
  }

  return return_status;
} /* end app_process */


/**
  * Deinitialize the POSENC instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_timx_deinit();
  mx_posencx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** POSENC new data callback
  * user: This implementation of the POSENC new data callback can be customized.
  * This function is executed when new data is available from the POSENC peripheral.
  */
void POSENC_NewDataCallback(void)
{
  new_data_received = 1U;
}
