/**
  ******************************************************************************
  * @file           : mx_gtzc_tee.h
  * @brief          : Header for mx_gtzc_tee.c file.
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

#ifndef MX_GTZC_TEE_H
#define MX_GTZC_TEE_H

#include "mx_def.h"
#include "stm32_hal.h"

/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for GTZC in HAL layer */
/******************************************************************************/
/**
  * @brief mx_gtzc_tee init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
system_status_t mx_gtzc_tee_setup(void);

/******************************************************************************/
/*                      Illegal access global interrupt                       */
/******************************************************************************/
void GTZC1_IRQHandler(void);

#endif /* MX_GTZC_TEE_H */