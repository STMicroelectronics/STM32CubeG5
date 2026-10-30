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
hal_exti_handle_t *pEXTI; /* Pointer referencing the EXTI handle from the generated code */
/* Private functions prototype -----------------------------------------------*/

/** ########## Step 1 ##########
  * The applicative code initializes the PLAY instance.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_play_edge_trigger_config_t edge_trigger_config;

  pPLAY = mx_play_alarm_init();
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

  /* Retrieve the EXTI handle */
  pEXTI = mx_play_exti_gethandle();
  if (pEXTI == NULL)
  {
    goto _app_init_exit;
  }

  if (HAL_EXTI_Enable(pEXTI, HAL_EXTI_MODE_INTERRUPT) != HAL_OK)
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
  app_status_t return_status = EXEC_STATUS_OK;

  /** ########## Step 2 ##########
    * The system enters STOP mode to demonstrate that the alarm signal is fully handled by the PLAY hardware.
    */
  PRINTF("[INFO] Step 2: Enter STOP Mode.\n");

  /** Suspend Tick increment to prevent WakeUp by Systick interrupt.
    * Otherwise the Systick interrupt wakes the device up within 1ms (HAL time base).
    */
  HAL_SuspendTick();

  /** Enter STOP Mode with WaitForInterrupt parameter, any interrupt enabled in NVIC would wake the system up
    * During STOP mode, the PLAY LUT output flags are not functional.
    * To wakeup the system, this example configures the PLAY EXTI output so that the falling edge of LUT 15
    * will trigger the EXTI, waking up the system.
    */
  HAL_PWR_EnterStopMode(HAL_PWR_LOW_PWR_MODE_WFI, HAL_PWR_STOP_MODE);

  /**
    * The user can change the PLAYi_INx input signal to trigger the alarm signal.
    * As the PLAY peripheral handles the alarm signal, the application does not need to do anything in this step.
    * When the user changes the input signal a second time, the PLAY peripheral will stop the alarm signal.
    * The alarm stop will trigger an EXTI interrupt, which will wake the system.
    * The application will then turn on the LED to indicate that the alarm has been stopped, wait for one second,
    * and go back again in STOP mode.
    */

  /* Reach this point when the MCU wakes up */

  /** ########## Step 3 ##########
    * The PLAY interrupt wakes the device up.
    */
  HAL_ResumeTick();
  mx_clock_init();

  PRINTF("[INFO] Step 3: Alarm stopped by user. WakeUp detection and clock re-configuration.\n");

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
  mx_play_alarm_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
