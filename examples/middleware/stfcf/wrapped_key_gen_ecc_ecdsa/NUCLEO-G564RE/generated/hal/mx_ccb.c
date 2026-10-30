
/**
  ******************************************************************************
  * @file           : mx_ccb.c
  * @brief          : CCB Peripheral initialization
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
#include "mx_ccb.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_ccb_handle_t hCCB;
/******************************************************************************/
/* Exported functions for CCB in HAL layer */
/******************************************************************************/
hal_ccb_handle_t *mx_ccb_init(void)
{
  uint32_t attempt = 0U;
  hal_status_t status = HAL_ERROR;

  if (HAL_CCB_Init(&hCCB, HAL_CCB) != HAL_OK)
  {
    return NULL;
  }
  HAL_RCC_FLASH_EnableClock();

  if (HAL_FLASH_ITF_Unlock(HAL_FLASH) != HAL_OK)
  {
    return NULL;
  }
  while ((attempt < 3U) && (status != HAL_OK))
  {
    status = HAL_FLASH_ITF_LaunchPUFOperation(HAL_FLASH);
    attempt++;
  }
  if (status != HAL_OK)
  {
    return NULL;
  }
  HAL_RCC_RNG_EnableClock();

  HAL_RCC_PKA_EnableClock();

  HAL_RCC_SAES_EnableClock();

  HAL_RCC_CCB_EnableClock();

  return &hCCB;
}

void mx_ccb_deinit(void)
{
  HAL_CCB_DeInit(&hCCB);

  HAL_RCC_CCB_Reset();

  HAL_RCC_CCB_DisableClock();
}

hal_ccb_handle_t *mx_ccb_gethandle(void)
{
  return &hCCB;
}
