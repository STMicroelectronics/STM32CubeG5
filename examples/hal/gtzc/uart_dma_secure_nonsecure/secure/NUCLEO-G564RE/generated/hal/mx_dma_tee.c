/**
  ******************************************************************************
  * @file           : mx_dma_tee.c
  * @brief          : DMA trusted execution environment configuration
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
#include "mx_dma_tee.h"
#include "stm32g5xx_ll_dma.h"

/* Exported functions --------------------------------------------------------*/
system_status_t mx_dma_tee_setup(void)
{
  /* Configure GPDMA1 channel 1 as non-secure and privileged. */
  HAL_RCC_GPDMA1_EnableClock();
  HAL_DMA_SetSecAttr(HAL_GPDMA1_CH1, HAL_DMA_NSEC);
  HAL_DMA_SetPrivAttr(HAL_GPDMA1_CH1, HAL_DMA_PRIV);

  return SYSTEM_OK;
}