/**
  ******************************************************************************
  * @file           : mx_cordic1.h
  * @brief          : Header for mx_cordic1.c file.
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
#ifndef MX_CORDIC1_H
#define MX_CORDIC1_H

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
/* Exported functions for CORDIC in HAL layer                                 */
/******************************************************************************/

/**
  * @brief mx_cordic1 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_cordic_handle_t *mx_cordic1_init(void);

/**
  * @brief  De-initialize cordic1 instance and return it.
  */
void mx_cordic1_deinit(void);

/**
  * @brief  Get the CORDIC1 object.
  * @retval Pointer on the CORDIC1 Handle
  */
hal_cordic_handle_t *mx_cordic1_gethandle(void);

/******************************************************************************/
/*                             CORDIC1 interrupt                              */
/******************************************************************************/
void CORDIC1_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_CORDIC1_H */
