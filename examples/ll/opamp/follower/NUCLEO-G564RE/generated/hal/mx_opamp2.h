/**
  ******************************************************************************
  * @file           : mx_opamp2.h
  * @brief          : Header for mx_opamp2.c file.
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
#ifndef MX_OPAMP2_H
#define MX_OPAMP2_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_OPAMP2

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for OPAMP in LL layer */
/******************************************************************************/
/**
  * @brief mx_opamp2 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */

OPAMP_TypeDef *mx_opamp2_init(void);

/**
  * @brief  De-initialize opamp2 instance and return it.
  */
void mx_opamp2_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_OPAMP2_H */
