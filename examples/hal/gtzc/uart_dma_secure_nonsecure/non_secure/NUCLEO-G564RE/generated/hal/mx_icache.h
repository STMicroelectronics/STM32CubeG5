/**
  ******************************************************************************
  * @file           : mx_icache.h
  * @brief          : Header for mx_icache.c file.
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

#ifndef MX_ICACHE_H
#define MX_ICACHE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported functions --------------------------------------------------------*/
hal_icache_handle_t *mx_icache_init(void);
void mx_icache_deinit(void);
hal_icache_handle_t *mx_icache_gethandle(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_ICACHE_H */