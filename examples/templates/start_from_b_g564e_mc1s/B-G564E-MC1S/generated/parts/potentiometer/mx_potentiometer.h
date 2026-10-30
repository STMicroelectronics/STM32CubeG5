/**
  ******************************************************************************
  * file           : mx_potentiometer.h
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
#ifndef MX_POTENTIOMETER_H
#define MX_POTENTIOMETER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------ */
#include "potentiometer.h"
#include "mx_hal_def.h"

/* Exported macros ----------------------------------------------------------- */
/* Part Config ID */
#define POTENTIOMETER_0                              0

/* No label has been defined for this part driver instance,
 * so no alias is generated for POTENTIOMETER_0 or mx_potentiometer_0_getobject(). */

/* Resource bindings */
/* Part interface Properties */
#define POTENTIOMETER_0_ADC_GETHANDLE      mx_adc3_gethandle


/* Functions to retrieve the part object */
pot_obj_t *mx_potentiometer_0_getobject(void);
 
 

#ifdef __cplusplus
}
#endif

#endif /* MX_POTENTIOMETER_H */
