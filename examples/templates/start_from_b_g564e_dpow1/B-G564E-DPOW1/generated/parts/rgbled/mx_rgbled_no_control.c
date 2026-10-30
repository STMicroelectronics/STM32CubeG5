/**
  ******************************************************************************
  * file           : mx_rgbled_no_control.c
  * brief          : Code generation for the RGB LED part driver.
  ******************************************************************************
  * attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_rgbled_license.md file
  * in the same directory as the generated code.
  * If no mx_rgbled_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ---------------------------------------------------------------- */
#include "mx_rgbled_no_control.h"

/* Private objects --------------------------------------------------------- */
static rgbled_obj_t rgbled_0_obj;
/**
  * @brief Bind generated TIM resources to the RGBLED object.
  * @param pobj pointer to the RGBLED object.
  * @retval 0 on success, -1 otherwise.
  */
int32_t rgbled_io_init(rgbled_obj_t *pobj)
{
  if (pobj == NULL)
  {
    return -1;
  }

  switch (pobj->id)
  {
case MX_RGBLED_0:
  pobj->red.phtim = MX_RGBLED_0_RED_TIM_GETHANDLE();
  pobj->red.pwm_channel = MX_RGBLED_0_RED_TIM_CHANNEL;
  pobj->green.phtim = MX_RGBLED_0_GREEN_TIM_GETHANDLE();
  pobj->green.pwm_channel = MX_RGBLED_0_GREEN_TIM_CHANNEL;
  pobj->blue.phtim = MX_RGBLED_0_BLUE_TIM_GETHANDLE();
  pobj->blue.pwm_channel = MX_RGBLED_0_BLUE_TIM_CHANNEL;
#if defined(RGBLED_PATTERN_PLAYBACK) && (RGBLED_PATTERN_PLAYBACK == 1)
      pobj->pattern_timer.phtim = NULL;
#endif /* RGBLED_PATTERN_PLAYBACK */
      break;
default:
      return -1;
  }

  return 0;
}

/**
  * @brief Return the generated RGBLED object for instance 0.
  * @retval RGBLED object pointer.
  */
rgbled_obj_t *mx_rgbled_0_getobject(void)
{
  return &rgbled_0_obj;
}
