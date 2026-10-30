/**
  ******************************************************************************
  * file           : example.c
  * brief          : example program body
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_play_handle_t *pPLAY; /* Pointer referencing the PLAY handle from the generated code */
/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the PLAY instance.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_play_edge_trigger_config_t edge_trigger_config;

  pPLAY = mx_example_play_init();
  if (pPLAY == NULL)
  {
    goto _app_init_exit;
  }

  /* In this example, edge triggers are not used */
  edge_trigger_config.lut_out_falling_mask = 0U;
  edge_trigger_config.lut_out_rising_mask = 0U;

  /* Start PLAY module */
  if (HAL_PLAY_Start(pPLAY, &edge_trigger_config) == HAL_OK)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  else
  {
    PRINTF("[ERROR] Step 1: Device initialization ERROR.\n");
    goto _app_init_exit;
  }

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * This step toggles the PLAY Software Trigger input, which acts as the second input of the LUT.
  * The first LUT input is the PLAY external GPIO input configured in the app_init().
  * The LUT output is connected to the PLAY output GPIO pin.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Toggle the second input with a software trigger */
  if (HAL_PLAY_ToggleSWTrigger(pPLAY, HAL_PLAY_SWTRIG0, 100U) == HAL_OK)
  {
    PRINTF("[INFO] Step 2: Software trigger toggled.\n");
    return_status = EXEC_STATUS_OK;
  }

  return return_status;
} /* end app_process */


/** Deinitializes the PLAY instance. This function is not used in this example (infinite loop).
  * It is provided as a reference if the application needs to de-initialize the PLAY instance.
  */
app_status_t app_deinit(void)
{
  PRINTF("[INFO] de-init \n");

  /** De-initialize the PLAY instance.
    * This does not reset the PLAY registers.
    * If the application needs to reset the PLAY registers,
    * it must call the HAL RCC dedicated reset functions.
    */
  mx_example_play_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
