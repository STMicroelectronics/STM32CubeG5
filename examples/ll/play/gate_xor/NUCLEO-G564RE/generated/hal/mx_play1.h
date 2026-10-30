/**
  ******************************************************************************
  * @file           : mx_play1.h
  * @brief          : Header for mx_play1.c file.
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
#ifndef MX_PLAY1_H
#define MX_PLAY1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_PLAY1

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for PLAY in LL layer */
/******************************************************************************/
/**
  * @brief mx_play1 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
PLAY_TypeDef *mx_play1_init(void);

/**
  * @brief  De-initialize play1 instance and return it.
  */
void mx_play1_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_PLAY1_H */
