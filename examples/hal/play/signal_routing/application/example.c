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
hal_tim_handle_t *pTIM; /* Pointer referencing the TIM handle from the generated code */
/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * Initializes the PLAY and the TIM peripherals.
  * This step is scheduled as follows:
  * - Initializes the PLAY peripheral.
  * - Initializes the TIM peripheral.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_play_edge_trigger_config_t edge_trigger_config;

  /* Initializes and configures the PLAY peripheral */
  pPLAY = mx_example_play_init();
  if (pPLAY == NULL)
  {
    goto _app_init_exit;
  }

  /* In this example, edge triggers are not used */
  edge_trigger_config.lut_out_falling_mask = 0U;
  edge_trigger_config.lut_out_rising_mask = 0U;

  /* Start PLAY module */
  if (HAL_PLAY_Start(pPLAY, &edge_trigger_config) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Initializes the TIM peripheral */
  pTIM = mx_example_tim_init();
  if (pTIM == NULL)
  {
    goto _app_init_exit;
  }

  /* Start the TIM channel */
  if (HAL_TIM_OC_StartChannel(pTIM, HAL_TIM_CHANNEL_1) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Map OC1 to TRGO */
  if (HAL_TIM_SetTriggerOutput(pTIM, HAL_TIM_TRGO_OC1) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Initialization and configuration of the PLAY & TIM peripherals COMPLETED.\n");

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * This step toggles the TIM TRGO output.
  * The TRGO output is connected to the PLAY which acts as a signal router.
  * The PLAY LUT output is routed to the PLAY output GPIO pin,
  * allowing the internal TRGO signal to be output externally.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  hal_tim_oc_compare_unit_config_t oc_compare_unit_config;
  oc_compare_unit_config.pulse = 0;

  /* Get the current configuration of the compare unit */
  HAL_TIM_OC_GetConfigCompareUnit(pTIM, HAL_TIM_OC_COMPARE_UNIT_1, &oc_compare_unit_config);

  /* Toggle the compare unit */
  if (oc_compare_unit_config.mode == HAL_TIM_OC_FORCED_ACTIVE)
  {
    oc_compare_unit_config.mode = HAL_TIM_OC_FORCED_INACTIVE;
  }
  else
  {
    oc_compare_unit_config.mode = HAL_TIM_OC_FORCED_ACTIVE;
  }
  if (HAL_TIM_OC_SetConfigCompareUnit(pTIM, HAL_TIM_OC_COMPARE_UNIT_1, &oc_compare_unit_config) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2: TIM TRGO toggled.\n");
  return_status = EXEC_STATUS_OK;

_app_process_exit:
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
