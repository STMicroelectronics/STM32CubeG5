/**
  ******************************************************************************
  * @file           : mx_fmac1.h
  * @brief          : Header for mx_fmac1.c file.
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
#ifndef MX_FMAC1_H
#define MX_FMAC1_H

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
/* Exported functions for FMAC in HAL layer */
/******************************************************************************/

/**
  * @brief mx_fmac1 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_fmac_handle_t *mx_fmac1_init(void);

/**
  * @brief  De-initialize fmac1 instance and return it.
  */
void mx_fmac1_deinit(void);

/**
  * @brief  Get the FMAC1 object.
  * @retval Pointer on the FMAC1 Handle
  */
hal_fmac_handle_t *mx_fmac1_gethandle(void);

/******************************************************************************/
/*                              FMAC1 interrupt                               */
/******************************************************************************/
void FMAC1_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_FMAC1_H */
