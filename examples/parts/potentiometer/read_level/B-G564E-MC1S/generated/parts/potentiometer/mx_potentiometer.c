/**
  ******************************************************************************
  * file           : mx_potentiometer.c
  * brief          : Code generation for the Potentiometer part driver.
  ******************************************************************************
  * attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_potentiometer_license.md file
  * in the same directory as the generated code.
  * If no mx_potentiometer_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "mx_potentiometer.h"

static pot_obj_t potentiometer_0_obj;

/**
  * @brief initialize the IO layer.
  * @param  pobj   : Potentiometer object
  * @retval error status.
  */
int32_t pot_io_init(pot_obj_t *pobj)
{
  /* Part Status */
  int ret = 0;

  switch (pobj->id)
  {
    case POTENTIOMETER_0:
      pobj->phadc        = POTENTIOMETER_0_ADC_GETHANDLE();
      break;

 
 
    default:
      /* Error -- Unknown ID */
      ret = -1;
      break;
  }

  return ret;
}

/**
  * @brief Function to retrieve the Part object POTENTIOMETER_0.
  * @retval part object.
  */
pot_obj_t *mx_potentiometer_0_getobject(void)
{
  return &potentiometer_0_obj;
}
 
 
