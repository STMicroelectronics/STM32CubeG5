/**
  ******************************************************************************
  * @file           : mx_usart1.c
  * @brief          : USART1 Peripheral initialization
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
#include "mx_usart1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
/* Handle for USART */
static hal_usart_handle_t hUSART1;

static hal_dma_handle_t hGPDMA1_CH0;
static hal_dma_handle_t hGPDMA1_CH1;
/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for USART in HAL layer */
/******************************************************************************/
hal_usart_handle_t *mx_usart1_usart_init(void)
{
  hal_usart_config_t usart_config;

  if (HAL_USART_Init(&hUSART1, HAL_USART1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_USART1_EnableClock();

  if (HAL_RCC_USART1_SetKernelClkSource(HAL_RCC_USART1_CLK_SRC_PCLK2) != HAL_OK)
  {
    return NULL;
  }

  usart_config.clock_prescaler = HAL_USART_PRESCALER_DIV1;
  usart_config.word_length = HAL_USART_WORD_LENGTH_8_BIT;
  usart_config.stop_bits = HAL_USART_STOP_BIT_1;
  usart_config.parity = HAL_USART_PARITY_NONE;
  usart_config.direction = HAL_USART_DIRECTION_TX_RX;
  usart_config.clock_polarity = HAL_USART_CLOCK_POLARITY_LOW;
  usart_config.clock_phase = HAL_USART_CLOCK_PHASE_1_EDGE;
  usart_config.clock_last_bit = HAL_USART_CLOCK_LAST_BIT_ENABLED;
  usart_config.mode = HAL_USART_MODE_MASTER;
  usart_config.baud_rate = 115200U;

  if (HAL_USART_SetConfig(&hUSART1, &usart_config) != HAL_OK)
  {
    return NULL;
  }

  /* ### USART1 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOA_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name]

       PA7     ------>   USART1_CK
       PA10    ------>   USART1_RX
       PA9     ------>   USART1_TX
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_7;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_7 | HAL_GPIO_PIN_10 | HAL_GPIO_PIN_9, &gpio_config);

  /* Enable USART instance interrupt */
  HAL_CORTEX_NVIC_SetPriority(USART1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(USART1_IRQn);

/* Configure the DMA TX */

  /* Configure the GPDMA1_CH0 DMA channel for USART1 TX */
  if (HAL_DMA_Init(&hGPDMA1_CH0, HAL_GPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPDMA1_EnableClock();

  hal_dma_direct_xfer_config_t xfer_cfg_usart1_tx_dma;
  xfer_cfg_usart1_tx_dma.request         = HAL_GPDMA1_REQUEST_USART1_TX;
  xfer_cfg_usart1_tx_dma.direction       = HAL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  xfer_cfg_usart1_tx_dma.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
  xfer_cfg_usart1_tx_dma.dest_inc        = HAL_DMA_DEST_ADDR_FIXED;
  xfer_cfg_usart1_tx_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  xfer_cfg_usart1_tx_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  xfer_cfg_usart1_tx_dma.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hGPDMA1_CH0, &xfer_cfg_usart1_tx_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for GPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH0_IRQn);

  /* Link the Transmit DMA handle to the USART handle */
  if (HAL_USART_SetTxDMA(&hUSART1, &hGPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

/* Configure the DMA RX */

  /* Configure the GPDMA1_CH1 DMA channel for USART1 RX */
  if (HAL_DMA_Init(&hGPDMA1_CH1, HAL_GPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  hal_dma_direct_xfer_config_t xfer_cfg_usart1_rx_dma;
  xfer_cfg_usart1_rx_dma.request         = HAL_GPDMA1_REQUEST_USART1_RX;
  xfer_cfg_usart1_rx_dma.direction       = HAL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  xfer_cfg_usart1_rx_dma.src_inc         = HAL_DMA_SRC_ADDR_FIXED;
  xfer_cfg_usart1_rx_dma.dest_inc        = HAL_DMA_DEST_ADDR_INCREMENTED;
  xfer_cfg_usart1_rx_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  xfer_cfg_usart1_rx_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  xfer_cfg_usart1_rx_dma.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hGPDMA1_CH1, &xfer_cfg_usart1_rx_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for GPDMA1_CH1 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH1_IRQn);

  /* Link the Receive DMA handle to the USART handle */
  if (HAL_USART_SetRxDMA(&hUSART1, &hGPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  return &hUSART1;
}

void mx_usart1_usart_deinit(void)
{
  /* Disable USART instance interrupt */
  HAL_CORTEX_NVIC_DisableIRQ(USART1_IRQn);

  /* De-initialize the USART instance */
  (void)HAL_USART_DeInit(&hUSART1);

  HAL_RCC_USART1_Reset();

  HAL_RCC_USART1_DisableClock();

  /* De-initialize all GPIOA pins associated with USART1 */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_7 | HAL_GPIO_PIN_9 | HAL_GPIO_PIN_10);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hGPDMA1_CH0);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH0_IRQn);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hGPDMA1_CH1);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH1_IRQn);
}

hal_usart_handle_t *mx_usart1_usart_gethandle(void)
{
  return &hUSART1;
}

/******************************************************************************/
/*                          USART1 global interrupt                           */
/******************************************************************************/
void USART1_IRQHandler(void)
{
  HAL_USART_IRQHandler(&hUSART1);
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
