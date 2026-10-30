/**
  ******************************************************************************
  * @file           : mx_cortex_systick.h
  * @brief          : Header for mx_cortex_systick.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_CORTEX_SYSTICK_H
#define MX_CORTEX_SYSTICK_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"
#include "mx_def.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_CORTEX_SYSTICK_H */
