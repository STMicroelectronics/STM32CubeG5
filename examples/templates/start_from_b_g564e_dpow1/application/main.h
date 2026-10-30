/**
  ******************************************************************************
  * file           : main.h
  * brief          : Header for main.c file .
  *                  This file contains the common defines of the application.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MAIN_H
#define MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "mx_hal_def.h"
#include "mx_system.h"
#include "mx_basic_stdio_app.h"
#include "mx_led.h"                /* led_part_drivers */
#include "mx_joystick.h"           /* joystick_part_driver */
#include "mx_rgbled_no_control.h"  /* rgbled_part_driver */
#include "mx_tcpp0203.h"           /* tcpp03_part_driver */
#include <stdio.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MAIN_H */
