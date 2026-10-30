/**
  ******************************************************************************
  * file           : example.c
  * brief          : PLAY XOR gate source with LL API
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

  if (mx_playx_init() == NULL)
  {
    goto _app_init_exit;
  }

  /* Lock the MX_PLAYx module configuration */
  PLAY_Lock();

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * This step toggles the MX_PLAYx Software Trigger input, which acts as the second input of the LUT.
  * The first LUT input is the MX_PLAYx external GPIO input configured in the app_init().
  * The LUT output is connected to the MX_PLAYx output GPIO pin.
  */
app_status_t app_process(void)
{
  /* Toggle the second input with a software trigger */
  PLAY_ToggleSWTrigger();

  return EXEC_STATUS_OK;
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
