/**
  ******************************************************************************
  * file           : mx_joystick.c
  * brief          : Code generation for the Joystick part driver.
  ******************************************************************************
  * attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_joystick_license.md file
  * in the same directory as the generated code.
  * If no mx_joystick_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "mx_joystick.h"

static joy_adc_t joystick_adc_0_obj;

/**
  * @brief initialize the IO layer.
  * @param  pobj   : Joystick object
  * @retval error status.
  */
int32_t joy_adc_io_init(joy_adc_t *pobj)
{
  /* Part Status */
  int ret = 0;

  switch (pobj->id)
  {
    case JOYSTICK_ADC_0:
      pobj->phadc              = JOYSTICK_ADC_0_ADC_GETHANDLE();
      pobj->key_up_max_val     = (int32_t)(JOYSTICK_ADC_0_KEY_UP_VAL + JOYSTICK_ADC_0_KEY_TOLERANCE(JOYSTICK_ADC_0_KEY_UP_VAL));
      pobj->key_up_min_val     = (int32_t)(JOYSTICK_ADC_0_KEY_UP_VAL - JOYSTICK_ADC_0_KEY_TOLERANCE(JOYSTICK_ADC_0_KEY_UP_VAL));
      pobj->key_down_max_val   = (int32_t)(JOYSTICK_ADC_0_KEY_DOWN_VAL + JOYSTICK_ADC_0_KEY_TOLERANCE(JOYSTICK_ADC_0_KEY_DOWN_VAL));
      pobj->key_down_min_val   = (int32_t)(JOYSTICK_ADC_0_KEY_DOWN_VAL - JOYSTICK_ADC_0_KEY_TOLERANCE(JOYSTICK_ADC_0_KEY_DOWN_VAL));
      pobj->key_left_max_val   = (int32_t)(JOYSTICK_ADC_0_KEY_LEFT_VAL + JOYSTICK_ADC_0_KEY_TOLERANCE(JOYSTICK_ADC_0_KEY_LEFT_VAL));
      pobj->key_left_min_val   = (int32_t)(JOYSTICK_ADC_0_KEY_LEFT_VAL - JOYSTICK_ADC_0_KEY_TOLERANCE(JOYSTICK_ADC_0_KEY_LEFT_VAL));
      pobj->key_right_max_val  = (int32_t)(JOYSTICK_ADC_0_KEY_RIGHT_VAL + JOYSTICK_ADC_0_KEY_TOLERANCE(JOYSTICK_ADC_0_KEY_RIGHT_VAL));
      pobj->key_right_min_val  = (int32_t)(JOYSTICK_ADC_0_KEY_RIGHT_VAL - JOYSTICK_ADC_0_KEY_TOLERANCE(JOYSTICK_ADC_0_KEY_RIGHT_VAL));
      pobj->key_center_max_val = (int32_t)(JOYSTICK_ADC_0_KEY_CENTER_VAL + JOYSTICK_ADC_0_KEY_TOLERANCE(JOYSTICK_ADC_0_KEY_CENTER_VAL));
      pobj->key_center_min_val = (int32_t)(JOYSTICK_ADC_0_KEY_CENTER_VAL - JOYSTICK_ADC_0_KEY_TOLERANCE(JOYSTICK_ADC_0_KEY_CENTER_VAL));
      break;

 
 
    default:
      /* Error -- Unknown ID */
      ret = -1;
      break;
  }

  return ret;
}

/**
  * @brief Function to retrieve the Part object 0.
  * @retval part object.
  */
joy_adc_t *mx_joystick_adc_0_getobject(void)
{
  return &joystick_adc_0_obj;
}
