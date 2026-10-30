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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_GTZC_TEE_H
#define MX_GTZC_TEE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"
#include "mx_def.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for GTZC in HAL layer */
/******************************************************************************/
/**
  * @brief mx_gtzc tee setup function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
system_status_t mx_gtzc_tee_setup(void);

void GTZC1_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_GTZC_TEE_H */