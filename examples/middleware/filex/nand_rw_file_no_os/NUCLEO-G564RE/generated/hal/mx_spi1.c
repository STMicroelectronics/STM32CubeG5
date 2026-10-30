/**
  ******************************************************************************
  * @file           : mx_spi1.c
  * @brief          : SPI1 Peripheral initialization
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
#include "mx_spi1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_spi_handle_t hSPI1;

static hal_dma_handle_t hGPDMA1_CH0;
static hal_dma_handle_t hGPDMA1_CH1;

/******************************************************************************/
/* Exported functions for SPI in HAL layer */
/******************************************************************************/
hal_spi_handle_t *mx_spi1_init(void)
{
  hal_spi_config_t spi_config;

  if (HAL_SPI_Init(&hSPI1, HAL_SPI1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_SPI1_EnableClock();

  if (HAL_RCC_SPI1_SetKernelClkSource(HAL_RCC_SPI1_CLK_SRC_PCLK2) != HAL_OK)
  {
    return NULL;
  }

  spi_config.mode = HAL_SPI_MODE_MASTER;
  spi_config.direction = HAL_SPI_DIRECTION_FULL_DUPLEX;
  spi_config.data_width = HAL_SPI_DATA_WIDTH_8_BIT;
  spi_config.clock_polarity = HAL_SPI_CLOCK_POLARITY_LOW;
  spi_config.clock_phase = HAL_SPI_CLOCK_PHASE_1_EDGE;
  spi_config.baud_rate_prescaler = HAL_SPI_BAUD_RATE_PRESCALER_64;
  spi_config.first_bit = HAL_SPI_MSB_FIRST;
  spi_config.nss_pin_management = HAL_SPI_NSS_PIN_MGMT_INTERNAL;

  if (HAL_SPI_SetConfig(&hSPI1, &spi_config) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_SPI_MASTER_EnableKeepIOState(&hSPI1) != HAL_OK)
  {
    return NULL;
  }

  /* ### SPI1 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOG_EnableClock();

  HAL_RCC_GPIOC_EnableClock();

  HAL_RCC_GPIOB_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name]

       PG3     ------>   SPI1_SCK
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_DOWN;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_2;
  HAL_GPIO_Init(HAL_GPIOG, HAL_GPIO_PIN_3, &gpio_config);

  /**
    [GPIO Pin] ------> [Signal Name]

       PC11    ------>   SPI1_MISO
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_DOWN;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_5;
  HAL_GPIO_Init(HAL_GPIOC, HAL_GPIO_PIN_11, &gpio_config);

  /**
    [GPIO Pin] ------> [Signal Name]

       PB2     ------>   SPI1_MOSI
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_DOWN;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_5;
  HAL_GPIO_Init(HAL_GPIOB, HAL_GPIO_PIN_2, &gpio_config);

  /* Configure the GPDMA1_CH0 DMA channel for SPI1 TX */
  if (HAL_DMA_Init(&hGPDMA1_CH0, HAL_GPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPDMA1_EnableClock();

  hal_dma_direct_xfer_config_t xfer_cfg_spi1_tx_dma;
  xfer_cfg_spi1_tx_dma.request         = HAL_GPDMA1_REQUEST_SPI1_TX;
  xfer_cfg_spi1_tx_dma.direction       = HAL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  xfer_cfg_spi1_tx_dma.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
  xfer_cfg_spi1_tx_dma.dest_inc        = HAL_DMA_DEST_ADDR_FIXED;
  xfer_cfg_spi1_tx_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  xfer_cfg_spi1_tx_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  xfer_cfg_spi1_tx_dma.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hGPDMA1_CH0, &xfer_cfg_spi1_tx_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for GPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH0_IRQn);

  /* Link the Transmit DMA handle to the SPI handle */
  if (HAL_SPI_SetTxDMA(&hSPI1, &hGPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  /* Configure the GPDMA1_CH1 DMA channel for SPI1 RX */
  if (HAL_DMA_Init(&hGPDMA1_CH1, HAL_GPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  hal_dma_direct_xfer_config_t xfer_cfg_spi1_rx_dma;
  xfer_cfg_spi1_rx_dma.request         = HAL_GPDMA1_REQUEST_SPI1_RX;
  xfer_cfg_spi1_rx_dma.direction       = HAL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  xfer_cfg_spi1_rx_dma.src_inc         = HAL_DMA_SRC_ADDR_FIXED;
  xfer_cfg_spi1_rx_dma.dest_inc        = HAL_DMA_DEST_ADDR_INCREMENTED;
  xfer_cfg_spi1_rx_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  xfer_cfg_spi1_rx_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  xfer_cfg_spi1_rx_dma.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hGPDMA1_CH1, &xfer_cfg_spi1_rx_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for GPDMA1_CH1 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH1_IRQn);

  /* Link the Receive DMA handle to the SPI handle */
  if (HAL_SPI_SetRxDMA(&hSPI1, &hGPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interrupt for SPI */
  HAL_CORTEX_NVIC_SetPriority(SPI1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(SPI1_IRQn);

  return &hSPI1;
}

void mx_spi1_deinit(void)
{
  /* Disable the interrupt for SPI */
  HAL_CORTEX_NVIC_DisableIRQ(SPI1_IRQn);

  (void)HAL_SPI_DeInit(&hSPI1);

  HAL_RCC_SPI1_Reset();

  HAL_RCC_SPI1_DisableClock();

  /* De-initialize all GPIOG pins associated with SPI1 */
  HAL_GPIO_DeInit(HAL_GPIOG, HAL_GPIO_PIN_3);

  /* De-initialize all GPIOC pins associated with SPI1 */
  HAL_GPIO_DeInit(HAL_GPIOC, HAL_GPIO_PIN_11);

  /* De-initialize all GPIOB pins associated with SPI1 */
  HAL_GPIO_DeInit(HAL_GPIOB, HAL_GPIO_PIN_2);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hGPDMA1_CH0);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH0_IRQn);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hGPDMA1_CH1);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH1_IRQn);
}

hal_spi_handle_t *mx_spi1_gethandle(void)
{
  return &hSPI1;
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
/*                           SPI1 global interrupt                            */
/******************************************************************************/
void SPI1_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&hSPI1);
}
