/**
  ******************************************************************************
  * @file           : mx_lpuart1.c
  * @brief          : LPUART1 Peripheral initialization
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_stm32g5xx_hal_drivers_license.md file
  * in the same directory as the generated code.
  * If no mx_lpuart1.c comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx_lpuart1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/* Handle for UART */
static hal_uart_handle_t hLPUART1;

static hal_dma_node_t DMA_Node_LPUART1_RX;
static hal_dma_handle_t hGPDMA1_CH1;

/* Exported function definition ----------------------------------------------*/

/******************************************************************************/
/* Exported functions for UART in HAL layer */
/******************************************************************************/

hal_uart_handle_t *mx_lpuart1_uart_init(void)
{
  hal_uart_config_t uart_config;

  /* Basic configuration */
  if (HAL_UART_Init(&hLPUART1, HAL_LPUART1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_LPUART1_EnableClock();

  if (HAL_RCC_LPUART1_SetKernelClkSource(HAL_RCC_LPUART1_CLK_SRC_PCLK3) != HAL_OK)
  {
    return NULL;
  }

  uart_config.baud_rate = 115200;
  uart_config.clock_prescaler = HAL_UART_PRESCALER_DIV1;
  uart_config.word_length = HAL_UART_WORD_LENGTH_8_BIT;
  uart_config.stop_bits = HAL_UART_STOP_BIT_1;
  uart_config.parity = HAL_UART_PARITY_NONE;
  uart_config.direction = HAL_UART_DIRECTION_TX_RX;
  uart_config.hw_flow_ctl = HAL_UART_HW_CONTROL_NONE;
  uart_config.oversampling = HAL_UART_OVERSAMPLING_16;
  uart_config.one_bit_sampling = HAL_UART_ONE_BIT_SAMPLE_DISABLE;

  if (HAL_UART_SetConfig(&hLPUART1, &uart_config) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPIOA_EnableClock();

  hal_gpio_config_t gpio_config;

  /**
    LPUART1 GPIO Configuration

    [GPIO Pin] ------> [Signal Name]

       PA3     ------>   LPUART1_RX
       PA2     ------>   LPUART1_TX
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_3;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_3 | HAL_GPIO_PIN_2, &gpio_config);

  /* Enable interrupt */
  HAL_CORTEX_NVIC_SetPriority(LPUART1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(LPUART1_IRQn);

  /* Configure the DMA RX */
  if (HAL_DMA_Init(&hGPDMA1_CH1, HAL_GPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPDMA1_EnableClock();

  hal_dma_direct_xfer_config_t xfer_cfg_lpuart1_rx_dma;
  xfer_cfg_lpuart1_rx_dma.request         = HAL_GPDMA1_REQUEST_LPUART1_RX;
  xfer_cfg_lpuart1_rx_dma.direction       = HAL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  xfer_cfg_lpuart1_rx_dma.src_inc         = HAL_DMA_SRC_ADDR_FIXED;
  xfer_cfg_lpuart1_rx_dma.dest_inc        = HAL_DMA_DEST_ADDR_INCREMENTED;
  xfer_cfg_lpuart1_rx_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  xfer_cfg_lpuart1_rx_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  xfer_cfg_lpuart1_rx_dma.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigPeriphLinkedListCircularXfer(&hGPDMA1_CH1,
                                                     &DMA_Node_LPUART1_RX,
                                                     &xfer_cfg_lpuart1_rx_dma) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_DMA_SetLinkedListXferFetchNodePort(&hGPDMA1_CH1, HAL_DMA_PORT0) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for GPDMA1_CH1 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH1_IRQn);

  /* Link the Receive DMA handle to the UART handle */
  if (HAL_UART_SetRxDMA(&hLPUART1, &hGPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  return &hLPUART1;
}

void mx_lpuart1_uart_deinit(void)
{
  /* Disable interrupt */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH1_IRQn);
  (void)HAL_UART_DeInit(&hLPUART1);

  HAL_RCC_LPUART1_Reset();
  HAL_RCC_LPUART1_DisableClock();

  /* De-initialize all GPIO pins associated with LPUART1 */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_2 | HAL_GPIO_PIN_3);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hGPDMA1_CH1);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH1_IRQn);
}

hal_uart_handle_t *mx_lpuart1_uart_gethandle(void)
{
  return &hLPUART1;
}

/******************************************************************************/
/*                          LPUART1 global interrupt                          */
/******************************************************************************/
void LPUART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&hLPUART1);
}

/******************************************************************************/
/*                     GPDMA1 channel 1 global interrupt                      */
/******************************************************************************/
void GPDMA1_CH1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hGPDMA1_CH1);
}