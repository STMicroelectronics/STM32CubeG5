/**
  ******************************************************************************
  * file           : example.c
  * brief          : Enter and exit the Sleep mode through an EXTI interrupt using the HAL API
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
#include "pwr_optim_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_exti_handle_t *pEXTI;  /* pointer referencing the EXTI handle from the generated code */

/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The application configures the system to reach typical product consumption.
  * The application configures the wake-up source to wake-up the MCU from Sleep mode.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Initializes the system to a typical product power-consumption baseline.
     All clocks are disabled at entry and enabled later on demand.
     Resources enabled after this initialization are maintained until the example completes. */
  if (PWR_SystemOptimization() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }

  /* Configures the wake-up source */
  if (mx_extix_init() != SYSTEM_OK)
  {
    goto _app_init_exit;
  }

  pEXTI = mx_extix_gethandle();
  if (pEXTI != NULL)
  {
    if (HAL_EXTI_Enable(pEXTI, HAL_EXTI_MODE_INTERRUPT) == HAL_OK)
    {
      return_status = EXEC_STATUS_INIT_OK;
    }
  }

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * The application requests entry into Sleep mode.
  * The application does not need to reconfigures the system clock after exiting Sleep mode.
  */
app_status_t app_process(void)
{
  /* Suspend SysTick to prevent interrupts during Sleep mode */
  HAL_SuspendTick();

  /* Enter Sleep low-power mode */
  HAL_PWR_EnterSleepMode(HAL_PWR_LOW_PWR_MODE_WFI);

  /* Resume SysTick to continue normal operation after wake-up from Sleep mode */
  HAL_ResumeTick();

  return EXEC_STATUS_OK;
} /* end app_process */


/** This function is never called and is provided as reference only.
  * Deinitializes the wake-up source before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;
} /* end app_deinit */


