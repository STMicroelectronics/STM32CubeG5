/**
  ******************************************************************************
  * @file           : mx_cortex_scb.h
  * @brief          : Header for mx_cortex_scb.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_stm32g5xx_hal_drivers_license.md file
  * in the same directory as the generated code.
  * If no mx_stm32g5xx_hal_drivers_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef MX_CORTEX_SCB_H
#define MX_CORTEX_SCB_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mx_def.h"
#include "stm32_hal.h"

/* Exported functions --------------------------------------------------------*/
system_status_t mx_cortex_scb_init(void);
void mx_cortex_scb_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_CORTEX_SCB_H */