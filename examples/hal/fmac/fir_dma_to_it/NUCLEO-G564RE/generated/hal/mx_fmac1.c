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

static hal_dma_handle_t hGPDMA1_CH0;

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

  fmac_config.x1_buf.base_half_word = 5;
  fmac_config.x1_buf.size_half_word = 127;
  fmac_config.x2_buf.base_half_word = 0;
  fmac_config.x2_buf.size_half_word = 5;
  fmac_config.y_buf.base_half_word = 132;
  fmac_config.y_buf.size_half_word = 123;

  HAL_FMAC_SetConfig(&hFMAC1, &fmac_config);

  /* Configure the DMA Write */

  /* Configure the GPDMA1_CH0 DMA channel for FMAC1 wr */
  if (HAL_DMA_Init(&hGPDMA1_CH0, HAL_GPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPDMA1_EnableClock();

  hal_dma_direct_xfer_config_t xfer_cfg_fmac1_wr_dma;
  xfer_cfg_fmac1_wr_dma.request         = HAL_GPDMA1_REQUEST_FMAC1_WR;
  xfer_cfg_fmac1_wr_dma.direction       = HAL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  xfer_cfg_fmac1_wr_dma.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
  xfer_cfg_fmac1_wr_dma.dest_inc        = HAL_DMA_DEST_ADDR_FIXED;
  xfer_cfg_fmac1_wr_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_HALFWORD;
  xfer_cfg_fmac1_wr_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_HALFWORD;
  xfer_cfg_fmac1_wr_dma.priority        = HAL_DMA_PRIORITY_HIGH;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hGPDMA1_CH0, &xfer_cfg_fmac1_wr_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for GPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH0_IRQn);

  /* Link the Write X1 DMA handle to the FMAC1 handle */
  if (HAL_FMAC_SetWriteX1DMA(&hFMAC1, &hGPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

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

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hGPDMA1_CH0);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH0_IRQn);
}

hal_fmac_handle_t *mx_fmac1_gethandle(void)
{
  return &hFMAC1;
}

/******************************************************************************/
/*                     GPDMA1 channel 0 global interrupt                      */
/******************************************************************************/
void GPDMA1_CH0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hGPDMA1_CH0);
}

/******************************************************************************/
/*                              FMAC1 interrupt                               */
/******************************************************************************/
void FMAC1_IRQHandler(void)
{
  HAL_FMAC_IRQHandler(&hFMAC1);
}
