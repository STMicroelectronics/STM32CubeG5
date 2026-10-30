/**
  ******************************************************************************
  * @file           : mx_fmac1.c
  * @brief          : FMAC1 Peripheral initialization
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

/* Includes ------------------------------------------------------------------*/
#include "mx_fmac1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_fmac_handle_t hFMAC1;

/******************************************************************************/
/* Exported functions for FMAC1 in HAL layer */
/******************************************************************************/

hal_fmac_handle_t *mx_fmac1_init(void)
{
  hal_fmac_config_t fmac_config;

  if (HAL_FMAC_Init(&hFMAC1, HAL_FMAC1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_FMAC1_EnableClock();

  fmac_config.x1_buf.base_half_word = 13;
  fmac_config.x1_buf.size_half_word = 121;
  fmac_config.x2_buf.base_half_word = 0;
  fmac_config.x2_buf.size_half_word = 13;
  fmac_config.y_buf.base_half_word = 134;
  fmac_config.y_buf.size_half_word = 121;

  HAL_FMAC_SetConfig(&hFMAC1, &fmac_config);

  /* Enable the interruption for FMAC1 */
  HAL_CORTEX_NVIC_SetPriority(FMAC1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(FMAC1_IRQn);

  return &hFMAC1;
}

void mx_fmac1_deinit(void)
{
  /* Disable the interruption for FMAC */
  HAL_CORTEX_NVIC_DisableIRQ(FMAC1_IRQn);
  (void)HAL_FMAC_DeInit(&hFMAC1);

  HAL_RCC_FMAC1_Reset();

  HAL_RCC_FMAC1_DisableClock();
}

hal_fmac_handle_t *mx_fmac1_gethandle(void)
{
  return &hFMAC1;
}

/******************************************************************************/
/*                              FMAC1 interrupt                               */
/******************************************************************************/
void FMAC1_IRQHandler(void)
{
  HAL_FMAC_IRQHandler(&hFMAC1);
}
