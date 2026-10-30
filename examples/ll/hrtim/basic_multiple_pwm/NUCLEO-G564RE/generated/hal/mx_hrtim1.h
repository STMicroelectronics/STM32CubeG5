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
#include "stm32_ll.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

#define USE_HRTIM1

/* Aliases of HRTIM1 timer labels */
#define MX_HRTIM_Y_TIMER  LL_HRTIM_TIMC
#define MX_HRTIM_X_TIMER  LL_HRTIM_TIMF

/* Aliases of HRTIM1 output labels */
#define MX_HRTIM_1Y_OUTPUT  LL_HRTIM_OUTPUT_CHC1
#define MX_HRTIM_2Y_OUTPUT  LL_HRTIM_OUTPUT_CHC2
#define MX_HRTIM_1X_OUTPUT  LL_HRTIM_OUTPUT_CHF1
#define MX_HRTIM_2X_OUTPUT  LL_HRTIM_OUTPUT_CHF2

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for HRTIM in LL layer */
/******************************************************************************/
/**
  * @brief mx_hrtim1 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
HRTIM_TypeDef *mx_hrtim1_init(void);

/**
  * @brief  De-initialize mx_hrtim1 instance and return it.
  */
void mx_hrtim1_deinit(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_HRTIM1_H */
