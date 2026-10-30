/**
  ******************************************************************************
  * file           : example.c
  * brief          : LSE configuration with HAL_API
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

/* Time during the LSE will be disabled */
#define LSE_DISABLE_DELAY_MS 2000U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/


/** ########## Step 1 ##########
  * In RCC examples, the init has already been triggered by mx_system_init() from main.
  * The MCO pin is configured in the generated code to output the LSE clock.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  PRINTF("[INFO] Step 1.1: MCO pin initialized to output the LSE clock.\n");

  if (HAL_RCC_LSE_IsEnabled() != HAL_RCC_OSC_ENABLED)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 1.2: LSE enabled (%" PRIu32 " Hz) and routed to MCO Pin (MCO freq: %" PRIu32 " Hz).\n",
         (uint32_t)LSE_VALUE,
         (uint32_t)LSE_VALUE
        );

  return_status = EXEC_STATUS_INIT_OK;

_app_process_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * The LSE is disabled during few seconds and then re-enabled.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Disable RTC Domain Write Protection */
  if (HAL_RCC_LSE_Disable() != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2: LSE disabled.\n");

  HAL_Delay(LSE_DISABLE_DELAY_MS);

  /** ########## Step 3 ##########
    * The LSE status is restored and output on the MCO pin.
    */

  if (HAL_RCC_LSE_Enable(HAL_RCC_LSE_ON, LSE_CONFIG_VALUE) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3: LSE status restored and output on MCO pin.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## End of example ##########
  * This de-initialization step has nothing to do.
  * RCC cannot be deinitialized in this example, so this function is kept only to inform the user.
  */
app_status_t app_deinit(void)
{

  return EXEC_STATUS_OK;
} /* end app_deinit */
