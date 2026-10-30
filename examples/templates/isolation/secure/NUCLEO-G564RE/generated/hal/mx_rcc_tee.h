/**
  ******************************************************************************
  * @file           : mx_rcc_tee.h
  * @brief          : Exporting functions for mx_rcc_tee.c
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_RCC_TEE_H
#define MX_RCC_TEE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"
#include "mx_def.h"

/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/**
  * @brief Peripheral security and privilege configuration function
  * @retval system_status_t
  */
system_status_t mx_rcc_tee_setup(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_RCC_TEE_H */