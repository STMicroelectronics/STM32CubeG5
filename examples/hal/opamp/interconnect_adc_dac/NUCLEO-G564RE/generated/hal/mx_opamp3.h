/**
  ******************************************************************************
  * @file           : mx_opamp3.h
  * @brief          : Header for mx_opamp3.c file.
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
#ifndef MX_OPAMP3_H
#define MX_OPAMP3_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for OPAMP in HAL layer */
/******************************************************************************/
/**
  * @brief mx_opamp3 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */

hal_opamp_handle_t *mx_opamp3_init(void);

/**
  * @brief  De-initialize opamp3 instance and return it.
  */
void mx_opamp3_deinit(void);

/**
  * @brief  Get the OPAMP3 object.
  * @retval Pointer on the OPAMP3 Handle
  */
hal_opamp_handle_t *mx_opamp3_gethandle(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_OPAMP3_H */
