/**
  ******************************************************************************
  * @file           : mx_ucpd1.c
  * @brief          : UCPD1 Peripheral initialization
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
#include "mx_ucpd1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
/* Handle for UCPD */
static hal_ucpd_handle_t hUCPD1;

static hal_dma_handle_t hGPDMA1_CH1;
static hal_dma_handle_t hGPDMA1_CH2;

/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for  in HAL layer */
/******************************************************************************/
hal_ucpd_handle_t *mx_ucpd1_init(void)
{
  hal_ucpd_config_t ucpd_config;

  /* Basic configuration */
  if (HAL_UCPD_Init(&hUCPD1, HAL_UCPD1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_UCPD1_EnableClock();

  if (HAL_RCC_UCPD1_SetKernelClkSource(HAL_RCC_UCPD1_CLK_SRC_HSI) != HAL_OK)
  {
    return NULL;
  }

  /* Bit rate set to 308 kbps */
  ucpd_config.clk_psc        = HAL_UCPD_PSC_DIV8;
  ucpd_config.hbitclk_div    = HAL_UCPD_HBITCLK_DIV13;
  ucpd_config.transwin_div   = HAL_UCPD_TRANSWIN_DIV10;
  ucpd_config.ifrgap_div     = HAL_UCPD_IFRGAP_DIV16;

  ucpd_config.rx_ordered_set = HAL_UCPD_RX_ORDERED_SET_SOP |
                               HAL_UCPD_RX_ORDERED_SET_HARD_RST |
                               HAL_UCPD_RX_ORDERED_SET_CABLE_RST;

  if (HAL_UCPD_SetConfig(&hUCPD1, &ucpd_config) != HAL_OK)
  {
    return NULL;
  }

  /* ### UCPD1 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOA_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name]

       PA8     ------>   UCPD1_CC1
       PA9     ------>   UCPD1_CC2
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ANALOG;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_8 | HAL_GPIO_PIN_9, &gpio_config);

  /* Enable interrupt */
  HAL_CORTEX_NVIC_SetPriority(UCPD1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_7, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(UCPD1_IRQn);

/* Configure the DMA TX */

  /* Configure the GPDMA1_CH1 DMA channel for UCPD1 TX */
  if (HAL_DMA_Init(&hGPDMA1_CH1, HAL_GPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPDMA1_EnableClock();

  hal_dma_direct_xfer_config_t xfer_cfg_ucpd1_tx_dma;
  xfer_cfg_ucpd1_tx_dma.request         = HAL_GPDMA1_REQUEST_UCPD1_TX;
  xfer_cfg_ucpd1_tx_dma.direction       = HAL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  xfer_cfg_ucpd1_tx_dma.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
  xfer_cfg_ucpd1_tx_dma.dest_inc        = HAL_DMA_DEST_ADDR_FIXED;
  xfer_cfg_ucpd1_tx_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  xfer_cfg_ucpd1_tx_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  xfer_cfg_ucpd1_tx_dma.priority        = HAL_DMA_PRIORITY_HIGH;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hGPDMA1_CH1, &xfer_cfg_ucpd1_tx_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for GPDMA1_CH1 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_6, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH1_IRQn);

  /* Link the Transmit DMA handle to the UCPD handle */
  if (HAL_UCPD_SetTxDMA(&hUCPD1, &hGPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

/* Configure the DMA RX */

  /* Configure the GPDMA1_CH2 DMA channel for UCPD1 RX */
  if (HAL_DMA_Init(&hGPDMA1_CH2, HAL_GPDMA1_CH2) != HAL_OK)
  {
    return NULL;
  }

  hal_dma_direct_xfer_config_t xfer_cfg_ucpd1_rx_dma;
  xfer_cfg_ucpd1_rx_dma.request         = HAL_GPDMA1_REQUEST_UCPD1_RX;
  xfer_cfg_ucpd1_rx_dma.direction       = HAL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  xfer_cfg_ucpd1_rx_dma.src_inc         = HAL_DMA_SRC_ADDR_FIXED;
  xfer_cfg_ucpd1_rx_dma.dest_inc        = HAL_DMA_DEST_ADDR_INCREMENTED;
  xfer_cfg_ucpd1_rx_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  xfer_cfg_ucpd1_rx_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  xfer_cfg_ucpd1_rx_dma.priority        = HAL_DMA_PRIORITY_HIGH;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hGPDMA1_CH2, &xfer_cfg_ucpd1_rx_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for GPDMA1_CH2 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH2_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_5, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH2_IRQn);

  /* Link the Receive DMA handle to the UCPD handle */
  if (HAL_UCPD_SetRxDMA(&hUCPD1, &hGPDMA1_CH2) != HAL_OK)
  {
    return NULL;
  }

  return &hUCPD1;
}

void mx_ucpd1_deinit(void)
{
  /* Disable interrupt */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH2_IRQn);
(void)HAL_UCPD_DeInit(&hUCPD1);

  HAL_RCC_UCPD1_Reset();

  HAL_RCC_UCPD1_DisableClock();

  /* De-initialize all GPIOA pins associated with UCPD1 */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_8 | HAL_GPIO_PIN_9);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hGPDMA1_CH1);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH1_IRQn);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hGPDMA1_CH2);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH2_IRQn);
}

hal_ucpd_handle_t *mx_ucpd1_gethandle(void)
{
  return &hUCPD1;
}

/******************************************************************************/
/*                           UCPD1 global interrupt                           */
/******************************************************************************/
void UCPD1_IRQHandler(void)
{
  HAL_UCPD_IRQHandler(&hUCPD1);
}

/******************************************************************************/
/*                     GPDMA1 channel 1 global interrupt                      */
/******************************************************************************/
void GPDMA1_CH1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hGPDMA1_CH1);
}

/******************************************************************************/
/*                     GPDMA1 channel 2 global interrupt                      */
/******************************************************************************/
void GPDMA1_CH2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hGPDMA1_CH2);
}
