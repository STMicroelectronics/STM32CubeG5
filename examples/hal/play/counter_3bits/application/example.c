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
volatile uint8_t counter_overflow; /* Tracks the counter overflow event */
/* Private functions prototype -----------------------------------------------*/
static void lut_output_falling_callback(hal_play_handle_t *hplay, uint32_t edge_trig_mask_status);

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

  /* Set edge trigger on LUT 2 */
  edge_trigger_config.lut_out_falling_mask = HAL_PLAY_LUT2_OUT_REGISTERED;
  edge_trigger_config.lut_out_rising_mask = 0U;

  /* Start PLAY module */
  if (HAL_PLAY_Start(pPLAY, &edge_trigger_config) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_PLAY_LUT_EnableIT(pPLAY, HAL_PLAY_LUT2_OUT_REGISTERED) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Register the PLAY LUT output falling edge callbacks */
  if (HAL_PLAY_RegisterLUTOutputFallingCallback(pPLAY, lut_output_falling_callback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * The user can change the PLAYi_INx input signal to see how the outputs changes accordingly.
  * As the PLAY peripheral handles the counting, the application does not need to do anything in this step.
  */
app_status_t app_process(void)
{
  /* Wait for the counter overflow event */
  while (counter_overflow == 0U)
  {
    /* The PLAY peripheral will count by itself, so the application does not need to do anything here.
       The counter_overflow variable will be set by the lut_output_falling_callback function
       when the counter resets to 0 (i.e., when the most significant bit signal falls). */
  }

  /* Reset the counter_overflow variable to 0 for the next cycle */
  counter_overflow = 0U;

  return EXEC_STATUS_OK;
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


/** Callback for LUT Output Falling Edge
  * This function is called when the LUT2 output signal falls (i.e., when the counter resets to 0).
  */
static void lut_output_falling_callback(hal_play_handle_t *hplay, uint32_t edge_trig_mask_status)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hplay);

  if ((edge_trig_mask_status & HAL_PLAY_LUT2_OUT_REGISTERED) == HAL_PLAY_LUT2_OUT_REGISTERED)
  {
    /** Set the counter_overflow variable each  time the counter resets to 0
      * (falling edge of most significant bit signal).
      */
    counter_overflow = 1U;
    PRINTF("[INFO] Step 2: Edge trigger: counter reset to 0 (LUT2 output falling edge).\n");
  }
}
