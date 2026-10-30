/**
  **********************************************************************************************************************
  * @file  : mx_gpdma1_ch0.c
  * @brief : Provides GPDMA1_CH0 services
  **********************************************************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in the root directory of this software
  * component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **********************************************************************************************************************
  */
/* Includes ----------------------------------------------------------------------------------------------------------*/
#include "mx_gpdma1_ch0.h"

/* Exported variables by reference------------------------------------------------------------------------------------*/
static hal_dma_handle_t hGPDMA1_CH0;

/**********************************************************************************************************************/
/* Exported functions for GPDMA1_CH0 in HAL layer */
/**********************************************************************************************************************/
/**
  * @brief  Initialize and configure the gpdma1_ch0 (configuration 1) through HAL layer.
  * @retval Pointer to hal_dma_handle_t structure.
  */
hal_dma_handle_t *mx_gpdma1_ch0_hal_init(void)
{

  HAL_RCC_GPDMA1_EnableClock();

  if (HAL_DMA_Init(&hGPDMA1_CH0, HAL_GPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  /* Set DMA channel linked list xfer configuration */
  {
    hal_dma_linkedlist_xfer_config_t linkedlist_xfer_cfg;

    linkedlist_xfer_cfg.priority        = HAL_DMA_PRIORITY_HIGH;
    linkedlist_xfer_cfg.fetch_port      = HAL_DMA_PORT0;
    linkedlist_xfer_cfg.xfer_event_mode = HAL_DMA_LINKEDLIST_XFER_EVENT_Q;
    if (HAL_DMA_SetConfigLinkedListXfer(&hGPDMA1_CH0, &linkedlist_xfer_cfg) != HAL_OK)
    {
      return NULL;
    }
  }

  /* Enable the interruption for GPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH0_IRQn);

  return &hGPDMA1_CH0;
}

/**
  * @brief  DeInitialize the gpdma1_ch0 (configuration 1) through HAL layer.
  */
void mx_gpdma1_ch0_hal_deinit(void)
{
  /* Disable the interruption for GPDMA1_CH0 */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH0_IRQn);

  (void)HAL_DMA_DeInit(&hGPDMA1_CH0);
}

/**
  * @brief Retrieve the gpdma1_channel0 handle.
  */
hal_dma_handle_t *mx_gpdma1_ch0_hal_gethandle(void)
{
  return &hGPDMA1_CH0;
}
/******************************************************************************/
/*                     GPDMA1 channel0 global interrupt                    */
/******************************************************************************/
void GPDMA1_CH0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hGPDMA1_CH0);
}
