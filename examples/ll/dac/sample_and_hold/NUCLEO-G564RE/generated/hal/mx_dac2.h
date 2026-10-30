/**
  ******************************************************************************
  * @file           : mx_dac2.h
  * @brief          : Header for mx_dac2.c file.
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
#ifndef MX_DAC2_H
#define MX_DAC2_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_DAC2

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for DAC in LL layer */
/******************************************************************************/
/**
  * @brief mx_dac2 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
DAC_TypeDef *mx_dac2_init(void);

/**
  * @brief  De-initialize dac2 instance and return it.
  */
void mx_dac2_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_DAC2_H */
