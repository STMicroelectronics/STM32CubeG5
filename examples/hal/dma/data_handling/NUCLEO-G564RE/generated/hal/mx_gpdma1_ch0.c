/**
  ******************************************************************************
  * @file           : mx_gpdma1_ch0.c
  * @brief          : Provides GPDMA1_CH0 services
  *                   One initialization function per data handling variant.
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

/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "mx_gpdma1_ch0.h"

/* Exported variables by reference------------------------------------------------------------------------------------*/
static hal_dma_handle_t hGPDMA1_CH0;

/* Private functions -------------------------------------------------------------------------------------------------*/
/**
  * @brief  Common GPDMA1_CH0 memory-to-memory configuration shared by all data handling variants.
  * @param  src_dw        Source data width.
  * @param  dest_dw       Destination data width.
  * @param  data_handling Data handling configuration.
  * @retval Pointer to the DMA handle, NULL in case of failure.
  */
static hal_dma_handle_t *gpdma1_ch0_configure(hal_dma_src_data_width_t src_dw,
                                              hal_dma_dest_data_width_t dest_dw,
                                              const hal_dma_data_handling_config_t *data_handling)
{
  /* Configure the GPDMA1_CH0 DMA channel */
  if (HAL_DMA_Init(&hGPDMA1_CH0, HAL_GPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPDMA1_EnableClock();

  hal_dma_direct_xfer_config_t xfer_cfg;

  xfer_cfg.request         = HAL_DMA_REQUEST_SW;
  xfer_cfg.direction       = HAL_DMA_DIRECTION_MEMORY_TO_MEMORY;
  xfer_cfg.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
  xfer_cfg.dest_inc        = HAL_DMA_DEST_ADDR_INCREMENTED;
  xfer_cfg.src_data_width  = src_dw;
  xfer_cfg.dest_data_width = dest_dw;
  xfer_cfg.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigDirectXfer(&hGPDMA1_CH0, &xfer_cfg) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_DMA_SetConfigDirectXferDataHandling(&hGPDMA1_CH0, data_handling) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for GPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH0_IRQn);

  return &hGPDMA1_CH0;
}

/**********************************************************************************************************************/
/* Exported functions for GPDMA1_CH0 in HAL layer */
/**********************************************************************************************************************/
/**
  * @brief  Right aligned zero padding: byte source is converted to a right aligned half-word, zero padded.
  */
hal_dma_handle_t *mx_gpdma1_ch0_cfg1_init(void)
{
  hal_dma_data_handling_config_t data_handling_config;

  data_handling_config.src_byte_exchange      = HAL_DMA_SRC_BYTE_PRESERVED;
  data_handling_config.dest_byte_exchange     = HAL_DMA_DEST_BYTE_PRESERVED;
  data_handling_config.dest_halfword_exchange = HAL_DMA_DEST_HALFWORD_PRESERVED;
  data_handling_config.trunc_padd             = HAL_DMA_DEST_DATA_TRUNC_LEFT_PADD_ZERO;
  data_handling_config.pack                   = HAL_DMA_DEST_DATA_PRESERVED;

  return gpdma1_ch0_configure(HAL_DMA_SRC_DATA_WIDTH_BYTE, HAL_DMA_DEST_DATA_WIDTH_HALFWORD, &data_handling_config);
}

/**
  * @brief  Right aligned sign extended: byte source is converted to a right aligned half-word, sign extended.
  */
hal_dma_handle_t *mx_gpdma1_ch0_cfg2_init(void)
{
  hal_dma_data_handling_config_t data_handling_config;

  data_handling_config.src_byte_exchange      = HAL_DMA_SRC_BYTE_PRESERVED;
  data_handling_config.dest_byte_exchange     = HAL_DMA_DEST_BYTE_PRESERVED;
  data_handling_config.dest_halfword_exchange = HAL_DMA_DEST_HALFWORD_PRESERVED;
  data_handling_config.trunc_padd             = HAL_DMA_DEST_DATA_TRUNC_RIGHT_PADD_SIGN;
  data_handling_config.pack                   = HAL_DMA_DEST_DATA_PRESERVED;

  return gpdma1_ch0_configure(HAL_DMA_SRC_DATA_WIDTH_BYTE, HAL_DMA_DEST_DATA_WIDTH_HALFWORD, &data_handling_config);
}

/**
  * @brief  Right aligned left truncated: half-word source is converted to a right aligned, left truncated byte.
  */
hal_dma_handle_t *mx_gpdma1_ch0_cfg3_init(void)
{
  hal_dma_data_handling_config_t data_handling_config;

  data_handling_config.src_byte_exchange      = HAL_DMA_SRC_BYTE_PRESERVED;
  data_handling_config.dest_byte_exchange     = HAL_DMA_DEST_BYTE_PRESERVED;
  data_handling_config.dest_halfword_exchange = HAL_DMA_DEST_HALFWORD_PRESERVED;
  data_handling_config.trunc_padd             = HAL_DMA_DEST_DATA_TRUNC_LEFT_PADD_ZERO;
  data_handling_config.pack                   = HAL_DMA_DEST_DATA_PRESERVED;

  return gpdma1_ch0_configure(HAL_DMA_SRC_DATA_WIDTH_HALFWORD, HAL_DMA_DEST_DATA_WIDTH_BYTE, &data_handling_config);
}

/**
  * @brief  Left aligned right truncated: half-word source is converted to a left aligned, right truncated byte.
  */
hal_dma_handle_t *mx_gpdma1_ch0_cfg4_init(void)
{
  hal_dma_data_handling_config_t data_handling_config;

  data_handling_config.src_byte_exchange      = HAL_DMA_SRC_BYTE_PRESERVED;
  data_handling_config.dest_byte_exchange     = HAL_DMA_DEST_BYTE_PRESERVED;
  data_handling_config.dest_halfword_exchange = HAL_DMA_DEST_HALFWORD_PRESERVED;
  data_handling_config.trunc_padd             = HAL_DMA_DEST_DATA_TRUNC_RIGHT_PADD_SIGN;
  data_handling_config.pack                   = HAL_DMA_DEST_DATA_PRESERVED;

  return gpdma1_ch0_configure(HAL_DMA_SRC_DATA_WIDTH_HALFWORD, HAL_DMA_DEST_DATA_WIDTH_BYTE, &data_handling_config);
}

/**
  * @brief  Source byte exchange: the two consecutive bytes within the unaligned half-word of each source word
  *         are exchanged.
  */
hal_dma_handle_t *mx_gpdma1_ch0_cfg5_init(void)
{
  hal_dma_data_handling_config_t data_handling_config;

  data_handling_config.src_byte_exchange      = HAL_DMA_SRC_BYTE_EXCHANGED;
  data_handling_config.dest_byte_exchange     = HAL_DMA_DEST_BYTE_PRESERVED;
  data_handling_config.dest_halfword_exchange = HAL_DMA_DEST_HALFWORD_PRESERVED;
  data_handling_config.trunc_padd             = HAL_DMA_DEST_DATA_TRUNC_RIGHT_PADD_SIGN;
  data_handling_config.pack                   = HAL_DMA_DEST_DATA_PRESERVED;

  return gpdma1_ch0_configure(HAL_DMA_SRC_DATA_WIDTH_WORD, HAL_DMA_DEST_DATA_WIDTH_WORD, &data_handling_config);
}

/**
  * @brief  Destination byte exchange: the two consecutive bytes are exchanged in each destination half-word.
  */
hal_dma_handle_t *mx_gpdma1_ch0_cfg6_init(void)
{
  hal_dma_data_handling_config_t data_handling_config;

  data_handling_config.src_byte_exchange      = HAL_DMA_SRC_BYTE_PRESERVED;
  data_handling_config.dest_byte_exchange     = HAL_DMA_DEST_BYTE_EXCHANGED;
  data_handling_config.dest_halfword_exchange = HAL_DMA_DEST_HALFWORD_PRESERVED;
  data_handling_config.trunc_padd             = HAL_DMA_DEST_DATA_TRUNC_RIGHT_PADD_SIGN;
  data_handling_config.pack                   = HAL_DMA_DEST_DATA_PRESERVED;

  return gpdma1_ch0_configure(HAL_DMA_SRC_DATA_WIDTH_WORD, HAL_DMA_DEST_DATA_WIDTH_WORD, &data_handling_config);
}

/**
  * @brief  Destination half-word exchange: the two consecutive half-words are exchanged in each destination word.
  */
hal_dma_handle_t *mx_gpdma1_ch0_cfg7_init(void)
{
  hal_dma_data_handling_config_t data_handling_config;

  data_handling_config.src_byte_exchange      = HAL_DMA_SRC_BYTE_PRESERVED;
  data_handling_config.dest_byte_exchange     = HAL_DMA_DEST_BYTE_PRESERVED;
  data_handling_config.dest_halfword_exchange = HAL_DMA_DEST_HALFWORD_EXCHANGED;
  data_handling_config.trunc_padd             = HAL_DMA_DEST_DATA_TRUNC_RIGHT_PADD_SIGN;
  data_handling_config.pack                   = HAL_DMA_DEST_DATA_PRESERVED;

  return gpdma1_ch0_configure(HAL_DMA_SRC_DATA_WIDTH_WORD, HAL_DMA_DEST_DATA_WIDTH_WORD, &data_handling_config);
}

/**
  * @brief  DeInitialize the gpdma1_ch0 through HAL layer.
  * @retval -1 in case of failure, 0 otherwise.
  */
int32_t mx_gpdma1_ch0_deinit(void)
{
  /* Disable the interruption for GPDMA1_CH0 */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH0_IRQn);

  HAL_DMA_DeInit(&hGPDMA1_CH0);

  return 0;
}

/**
  * @brief  Get the GPDMA1_CH0 object.
  */
hal_dma_handle_t *mx_gpdma1_ch0_gethandle(void)
{
  return &hGPDMA1_CH0;
}

/******************************************************************************/
/* GPDMA1 channel 0 global interrupt */
/******************************************************************************/
void GPDMA1_CH0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hGPDMA1_CH0);
}
