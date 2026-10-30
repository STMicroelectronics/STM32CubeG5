/**
  ******************************************************************************
  * file           : example.c
  * brief          : PLAY wakeup from low power with LL API
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
/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the MX_PLAYx instance.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /**
    * Configure PLAY input for the user button and route its signal through LUT
    * to PLAY output, which is connected to the EXTI interrupt used for wakeup.
    */
  if (mx_playx_init() == NULL)
  {
    goto _app_init_exit;
  }

  /* Lock the MX_PLAYx module configuration */
  PLAY_Lock();

  /* Enable EXTI interrupt mode */
  EXTI_EnableLineMode_IT();

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * The system enters STOP mode.
    */
  if (mx_pwr_enter_low_power() != SYSTEM_OK)
  {
    goto _app_process_exit;
  }

  /**
    * The user can change the PLAYi_INx input signal to wake-up the system.
    * The PLAY peripheral will route the signal to the EXTI interrupt.
    * The application will then turn on the LED to indicate that it was waken-up, wait for one second,
    * and go back again in STOP mode.
    */

  /* Reach this point when the MCU wakes up */

  /** ########## Step 3 ##########
    * The MX_PLAYx interrupt wakes the device up.
    */
  if (mx_pwr_system_was_low_power() == 0U)
  {
    goto _app_process_exit;
  }

  if (mx_pwr_exit_low_power() != SYSTEM_OK)
  {
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** Deinitializes the MX_PLAYx instance. This function is not used in this example (infinite loop).
  * It is provided as a reference if the application needs to de-initialize the MX_PLAYx instance.
  */
app_status_t app_deinit(void)
{

  /** De-initialize the MX_PLAYx instance.
    * This does not reset the MX_PLAYx registers.
    * If the application needs to reset the MX_PLAYx registers,
    * it must call the LL RCC dedicated reset functions.
    */
  mx_playx_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */

