/**
  ******************************************************************************
  * file           : pwr_optim_example.h
  * brief          : Header for stm32ynxx_pwr_optim_example.c
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
#ifndef PWR_OPTIM_EXAMPLE_H
#define PWR_OPTIM_EXAMPLE_H

/* Includes ------------------------------------------------------------------*/
#include "mx_hal_def.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported functions ------------------------------------------------------- */

/** brief:  System configuration for typical power consumption.
  * retval: system status
  */
system_status_t PWR_SystemOptimization(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PWR_OPTIM_EXAMPLE_H */
