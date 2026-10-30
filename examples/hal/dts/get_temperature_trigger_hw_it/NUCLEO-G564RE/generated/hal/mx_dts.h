/**
  ******************************************************************************
  * @file           : mx_dts.h
  * @brief          : Header for mx_dts.c file.
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
#ifndef MX_DTS_H
#define MX_DTS_H

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
/* Exported functions for DTS in HAL layer */
/******************************************************************************/
/**
  * @brief mx_dts init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_dts_handle_t *mx_dts_init(void);

/**
  * @brief  De-initialize dts instance and return it.
  */
void mx_dts_deinit(void);

/**
  * @brief  Get the DTS object.
  * @retval Pointer on the DTS Handle
  */
hal_dts_handle_t *mx_dts_gethandle(void);

/******************************************************************************/
/*                 DTS interrupt OR DTS AIT through EXTI line                 */
/******************************************************************************/
void DTS_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_DTS_H */
