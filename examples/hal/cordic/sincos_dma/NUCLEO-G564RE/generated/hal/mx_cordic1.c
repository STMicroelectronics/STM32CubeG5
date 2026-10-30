/**
  ******************************************************************************
  * @file           : mx_cordic1.c
  * @brief          : CORDIC1 Peripheral initialization
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
#include "mx_cordic1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototypes ----------------------------------------------*/
/* Exported variables by reference -------------------------------------------*/
static hal_cordic_handle_t hCORDIC1;

static hal_dma_handle_t hGPDMA1_CH0;
static hal_dma_handle_t hGPDMA1_CH1;

/******************************************************************************/
/* Exported functions for CORDIC in HAL layer                                 */
/******************************************************************************/
hal_cordic_handle_t *mx_cordic1_init(void)
{
  hal_cordic_config_t cordic_config;

  if (HAL_CORDIC_Init(&hCORDIC1, HAL_CORDIC1) != HAL_OK)
  {
    return NULL;
  }

  /* configure RCC clock activation*/
  HAL_RCC_CORDIC1_EnableClock();

  /* Basic Configuration ***************************************************/
  cordic_config.function       = HAL_CORDIC_FUNCTION_SINE;
  cordic_config.in_width       = HAL_CORDIC_IN_WIDTH_32_BIT;
  cordic_config.nb_arg         = HAL_CORDIC_NB_ARG_1;
  cordic_config.out_width      = HAL_CORDIC_OUT_WIDTH_32_BIT;
  cordic_config.nb_result      = HAL_CORDIC_NB_RESULT_2;
  cordic_config.scaling_factor = HAL_CORDIC_SCALING_FACTOR_0;
  cordic_config.precision      = HAL_CORDIC_PRECISION_6_CYCLE;
  if (HAL_CORDIC_SetConfig(&hCORDIC1,&cordic_config) != HAL_OK)
  {
    return NULL;
  }
  /* Configure the DMA Write request */

  /* Configure the GPDMA1_CH0 DMA channel for CORDIC1 WR */
  if (HAL_DMA_Init(&hGPDMA1_CH0, HAL_GPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPDMA1_EnableClock();

  hal_dma_direct_xfer_config_t xfer_cfg_cordic1_wr_dma;
  xfer_cfg_cordic1_wr_dma.request         = HAL_GPDMA1_REQUEST_CORDIC1_WR;
  xfer_cfg_cordic1_wr_dma.direction       = HAL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  xfer_cfg_cordic1_wr_dma.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
  xfer_cfg_cordic1_wr_dma.dest_inc        = HAL_DMA_DEST_ADDR_FIXED;
  xfer_cfg_cordic1_wr_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_WORD;
  xfer_cfg_cordic1_wr_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_WORD;
  xfer_cfg_cordic1_wr_dma.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hGPDMA1_CH0, &xfer_cfg_cordic1_wr_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for GPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH0_IRQn);

  /* Link the write DMA handle to the CORDIC handle */
  if (HAL_CORDIC_SetWriteDMA(&hCORDIC1, &hGPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }
  /* Configure the DMA Read request */

  /* Configure the GPDMA1_CH1 DMA channel for CORDIC1 RD */
  if (HAL_DMA_Init(&hGPDMA1_CH1, HAL_GPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  hal_dma_direct_xfer_config_t xfer_cfg_cordic1_rd_dma;
  xfer_cfg_cordic1_rd_dma.request         = HAL_GPDMA1_REQUEST_CORDIC1_RD;
  xfer_cfg_cordic1_rd_dma.direction       = HAL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  xfer_cfg_cordic1_rd_dma.src_inc         = HAL_DMA_SRC_ADDR_FIXED;
  xfer_cfg_cordic1_rd_dma.dest_inc        = HAL_DMA_DEST_ADDR_INCREMENTED;
  xfer_cfg_cordic1_rd_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_WORD;
  xfer_cfg_cordic1_rd_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_WORD;
  xfer_cfg_cordic1_rd_dma.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hGPDMA1_CH1, &xfer_cfg_cordic1_rd_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for GPDMA1_CH1 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH1_IRQn);

  /* Link the Read DMA handle to the CORDIC handle */
  if (HAL_CORDIC_SetReadDMA(&hCORDIC1, &hGPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for CORDIC */
  HAL_CORTEX_NVIC_SetPriority(CORDIC1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(CORDIC1_IRQn);

  return &hCORDIC1;
}
void mx_cordic1_deinit(void)
{
  /* Disable the interruption for CORDIC */
  HAL_CORTEX_NVIC_DisableIRQ(CORDIC1_IRQn);
  /* CORDIC De-intialization */
  (void)HAL_CORDIC_DeInit(&hCORDIC1);
  /* Disable CORDIC Clock */
  HAL_RCC_CORDIC1_Reset();

  HAL_RCC_CORDIC1_DisableClock();

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hGPDMA1_CH0);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH0_IRQn);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hGPDMA1_CH1);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH1_IRQn);
}

hal_cordic_handle_t *mx_cordic1_gethandle(void)
{
  return &hCORDIC1;
}

/******************************************************************************/
/*                     GPDMA1 channel 0 global interrupt                      */
/******************************************************************************/
void GPDMA1_CH0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hGPDMA1_CH0);
}

/******************************************************************************/
/*                     GPDMA1 channel 1 global interrupt                      */
/******************************************************************************/
void GPDMA1_CH1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hGPDMA1_CH1);
}

/******************************************************************************/
/*                             CORDIC1 interrupt                              */
/******************************************************************************/
void CORDIC1_IRQHandler(void)
{
  HAL_CORDIC_IRQHandler(&hCORDIC1);
}
