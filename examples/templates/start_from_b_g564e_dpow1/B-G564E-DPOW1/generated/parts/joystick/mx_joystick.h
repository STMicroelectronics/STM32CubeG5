/**
  ******************************************************************************
  * file           : mx_joystick.h
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
#ifndef MX_JOYSTICK_H
#define MX_JOYSTICK_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------ */
#include "joystick_adc.h"
#include "mx_hal_def.h"

/* Exported macros ----------------------------------------------------------- */
/* The reference voltage is equal to the voltage at which the mcu is operating (usually 3.3V) */
#define JOYSTICK_ADC_VOLTAGE_REFERENCE                 3.3

/* Part Config ID */
#define JOYSTICK_ADC_0                   0 /* Main label */
#define MX_JOY1                                  0 /* Extra Label */

/* Alias for the getter function based on the extra label */
#define mx_joy1_getobject()      mx_joystick_adc_0_getobject()

/* Resource bindings */
/* Part interface Properties */
#define JOYSTICK_ADC_0_ADC_GETHANDLE        mx_adc1_gethandle

/* Adc Resolution */
#define JOYSTICK_ADC_0_RESOLUTION           256

/* Input voltage (in volts) applied to the ADC pin for each key */
#define JOYSTICK_ADC_0_KEY_CENTER_VOLTAGE   0
#define JOYSTICK_ADC_0_KEY_LEFT_VOLTAGE     0.66
#define JOYSTICK_ADC_0_KEY_DOWN_VOLTAGE     1.32
#define JOYSTICK_ADC_0_KEY_UP_VOLTAGE       2.013
#define JOYSTICK_ADC_0_KEY_RIGHT_VOLTAGE    2.64

/* Input ADC value for each key */
#define JOYSTICK_ADC_0_KEY_UP_VAL       ((JOYSTICK_ADC_0_KEY_UP_VOLTAGE / JOYSTICK_ADC_VOLTAGE_REFERENCE) * JOYSTICK_ADC_0_RESOLUTION)
#define JOYSTICK_ADC_0_KEY_LEFT_VAL     ((JOYSTICK_ADC_0_KEY_LEFT_VOLTAGE / JOYSTICK_ADC_VOLTAGE_REFERENCE) * JOYSTICK_ADC_0_RESOLUTION)
#define JOYSTICK_ADC_0_KEY_DOWN_VAL     ((JOYSTICK_ADC_0_KEY_DOWN_VOLTAGE / JOYSTICK_ADC_VOLTAGE_REFERENCE) * JOYSTICK_ADC_0_RESOLUTION)
#define JOYSTICK_ADC_0_KEY_RIGHT_VAL    ((JOYSTICK_ADC_0_KEY_RIGHT_VOLTAGE / JOYSTICK_ADC_VOLTAGE_REFERENCE) * JOYSTICK_ADC_0_RESOLUTION)
#define JOYSTICK_ADC_0_KEY_CENTER_VAL   ((JOYSTICK_ADC_0_KEY_CENTER_VOLTAGE / JOYSTICK_ADC_VOLTAGE_REFERENCE) * JOYSTICK_ADC_0_RESOLUTION)

/* Margin error between max and min key compared value in 15% +1 */
#define JOYSTICK_ADC_0_KEY_TOLERANCE(val)   (((val * 15)/100) + 1)

/* Functions to retrieve the part object */
joy_adc_t *mx_joystick_adc_0_getobject(void);

#ifdef __cplusplus
}
#endif

#endif /* MX_JOYSTICK_H */
