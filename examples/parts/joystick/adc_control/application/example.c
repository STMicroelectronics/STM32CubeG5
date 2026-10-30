/**
  ******************************************************************************
  * file           : example.c
  * brief          : Joystick read key state example
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
#define APP_JOY_DEBOUNCE_TIMEOUT        50

/* Private variables ---------------------------------------------------------*/
joy_adc_t *pJoyObj = NULL;
const char *pKeyPressedLabel[6] = {"NONE", "CENTER", "DOWN", "LEFT", "RIGHT", "UP"};

/** ########## Step 1 ##########
  * Initializes the Joystick instance.
  *
  * Start ADC peripheral with self-calibration.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Retrieve and store the joystick object pointer */
  pJoyObj = mx_joystick_adc_0_getobject();

  /* Part initialization */
  if (joy_adc_init(pJoyObj, JOYSTICK_ADC_0) != JOY_ADC_OK)
  {
    goto _app_init_exit;
  }

  /* Start ADC instance */
  if (HAL_ADC_Start(mx_example_adc_gethandle()) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_ADC_Calibrate(mx_example_adc_gethandle()) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");
  PRINTF("\n[INFO] Step 1: Press any key on the joystick.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Read the state of the Joystick key.
  * The status LED is toggled when any key is pressed.
  */
app_status_t app_process(void)
{
  app_status_t return_status;
  joy_adc_key_t key_pressed_state;

  /* get key state */
  if (joy_adc_get_state(pJoyObj, &key_pressed_state) == JOY_ADC_OK)
  {
    if (key_pressed_state != JOY_ADC_KEY_NONE)
    {
      PRINTF("\n[INFO] Step 2: Key Pressed - %s\n", pKeyPressedLabel[key_pressed_state]);

      /* led toggles */
      led_toggle(MX_STATUS_LED);
    }

    return_status = EXEC_STATUS_OK;
  }
  else
  {
    return_status = EXEC_STATUS_ERROR;
  }

  HAL_Delay(APP_JOY_DEBOUNCE_TIMEOUT);

  return return_status;
} /* end app_process */


/** There is no deinit function in the joystick adc part driver.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;
} /* end app_deinit */
