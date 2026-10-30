/**
  ******************************************************************************
  * file           : example.c
  * brief          : potentiometer read level example
  *                  Called by the main after system initialization
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
pot_obj_t *pPotObj = NULL;
uint32_t Level = 0;

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The init of the potentiometer part is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Retrieve and store the potentiometer object pointer */
  pPotObj = mx_potentiometer_0_getobject();

  /* Part initialization */
  if (pot_init(pPotObj, POTENTIOMETER_0) != POT_ERROR)
  {
    PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
    return_status = EXEC_STATUS_INIT_OK;
  }
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Reads potentiometer level
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  if (pot_get_level(pPotObj, &Level) != POT_OK)
  {
    PRINTF("\n[ERROR] Step 2: Read potentiometer level ERROR\n");
    goto _app_process_exit;
  }

  PRINTF("\n[INFO] Step 2: Read potentiometer level COMPLETED\n");
  PRINTF("\n[INFO] Level = %"PRIu32 "\n", Level);

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** Deinitializes the ADC instance before leaving the scenario.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  /* adc de-init */
  mx_example_adc_deinit();
  return EXEC_STATUS_OK;
} /* end app_deinit */

