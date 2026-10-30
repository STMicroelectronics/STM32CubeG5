/**
  ******************************************************************************
  * @file           : mx_hrtim1.h
  * @brief          : Header for mx_hrtim1.c file.
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
#ifndef MX_HRTIM1_H
#define MX_HRTIM1_H

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
/* Exported functions for HRTIM in HAL layer */
/******************************************************************************/
/**
  * @brief mx_hrtim1 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_hrtim_handle_t *mx_hrtim1_init(void);

/**
  * @brief  De-initialize mx_hrtim1 instance and return it.
  */
void mx_hrtim1_deinit(void);

/**
  * @brief  Get the mx_hrtim1 object.
  * @retval Pointer on the mx_hrtim1 Handle
  */
hal_hrtim_handle_t *mx_hrtim1_gethandle(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_HRTIM1_H */
