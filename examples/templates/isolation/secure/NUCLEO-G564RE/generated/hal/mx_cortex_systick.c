/**
  ******************************************************************************
  * @file           : mx_cortex_systick.c
  * @brief          : STM32 cortex systick body
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
/* Includes ------------------------------------------------------------------*/
#include "mx_cortex_systick.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/


/******************************************************************************/
/*                            Systick Handler                                 */
/******************************************************************************/
/**
  * @brief  This function handles SysTick Handler.
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}
