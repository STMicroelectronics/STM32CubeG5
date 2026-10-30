/**
  ******************************************************************************
  * file           : stm32g5xx_pwr_optim_example.c
  * brief          : System configuration for STM32G5xx power consumption optimization
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
#include "pwr_optim_example.h"

/* Private functions prototype -----------------------------------------------*/

/** brief:  System configuration for typical power consumption.
  * retval: system status
  */
system_status_t PWR_SystemOptimization(void)
{
  if (mx_rccx_init() != SYSTEM_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  if (mx_pwrx_init() != SYSTEM_OK)
  {
    return SYSTEM_POWER_ERROR;
  }

  return SYSTEM_OK;
}