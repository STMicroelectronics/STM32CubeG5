/**
  ******************************************************************************
  * file           : mx_rgbled_no_control.h
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

/* Define to prevent recursive inclusion ----------------------------------- */
#ifndef MX_RGBLED_NO_CONTROL_H
#define MX_RGBLED_NO_CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ---------------------------------------------------------------- */
#include "rgbled.h"
#include "mx_hal_def.h"

/* Exported macros --------------------------------------------------------- */
/* Part Config ID */
#define MX_RGBLED_0                                  0U

/* User-defined aliases for the RGBLED instance IDs and getter functions */
#define MX_STATUS_RGBLED                              MX_RGBLED_0
#define mx_status_rgbled_getobject()       mx_rgbled_0_getobject()

/* Resource bindings */
#define MX_RGBLED_0_RED_TIM_GETHANDLE()          mx_tim3_gethandle()
#define MX_RGBLED_0_RED_TIM_CHANNEL              HAL_TIM_CHANNEL_1
#define MX_RGBLED_0_GREEN_TIM_GETHANDLE()        mx_tim4_gethandle()
#define MX_RGBLED_0_GREEN_TIM_CHANNEL            HAL_TIM_CHANNEL_2
#define MX_RGBLED_0_BLUE_TIM_GETHANDLE()         mx_tim2_gethandle()
#define MX_RGBLED_0_BLUE_TIM_CHANNEL             HAL_TIM_CHANNEL_1
#define MX_RGBLED_0_PATTERN_TIM_GETHANDLE()      NULL
/* Exported functions ------------------------------------------------------ */
rgbled_obj_t *mx_rgbled_0_getobject(void);
#ifdef __cplusplus
}
#endif

#endif /* MX_RGBLED_NO_CONTROL_H */
