/**
  ******************************************************************************
  * @file           : mx_i2c1.c
  * @brief          : I2C1 Peripheral initialization
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
#include "mx_i2c1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_smbus_handle_t hI2C1;

static hal_dma_handle_t hGPDMA1_CH0;
static hal_dma_handle_t hGPDMA1_CH1;

/******************************************************************************/
/* Exported functions for I2C1 in HAL layer */
/******************************************************************************/
hal_smbus_handle_t *mx_i2c1_smbus_init(void)
{
  hal_smbus_config_t smbus_config;

  if (HAL_SMBUS_Init(&hI2C1, HAL_SMBUS1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_I2C1_EnableClock();

  /*
    Timing automatically calculated with:
     - I2C1 input clock at 140000000 Hz
     - I2C clock speed at 1000000 Hz
  */
  smbus_config.timing           = 0xF02A3D;
  smbus_config.own_address1     = 63 << 1U;
  smbus_config.device_mode      = HAL_SMBUS_PERIPHERAL_MODE_HOST;
  if (HAL_SMBUS_SetConfig(&hI2C1, &smbus_config) != HAL_OK)
  {
    return NULL;
  }

  HAL_SMBUS_EnableAnalogFilter(&hI2C1);
  HAL_SMBUS_EnablePacketErrorCheck(&hI2C1);

  /* ### I2C1 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOA_EnableClock();

  HAL_RCC_GPIOB_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name]

       PA15    ------>   I2C1_SCL
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_OPENDRAIN;
  gpio_config.pull        = HAL_GPIO_PULL_UP;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_4;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_15, &gpio_config);

  /**
    [GPIO Pin] ------> [Signal Name]

       PB9     ------>   I2C1_SDA
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_OPENDRAIN;
  gpio_config.pull        = HAL_GPIO_PULL_UP;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_4;
  HAL_GPIO_Init(HAL_GPIOB, HAL_GPIO_PIN_9, &gpio_config);

  /* Configure the DMA TX */

  /* Configure the GPDMA1_CH0 DMA channel for I2C1 TX */
  if (HAL_DMA_Init(&hGPDMA1_CH0, HAL_GPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPDMA1_EnableClock();

  hal_dma_direct_xfer_config_t xfer_cfg_i2c1_tx_dma;
  xfer_cfg_i2c1_tx_dma.request         = HAL_GPDMA1_REQUEST_I2C1_TX;
  xfer_cfg_i2c1_tx_dma.direction       = HAL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  xfer_cfg_i2c1_tx_dma.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
  xfer_cfg_i2c1_tx_dma.dest_inc        = HAL_DMA_DEST_ADDR_FIXED;
  xfer_cfg_i2c1_tx_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  xfer_cfg_i2c1_tx_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  xfer_cfg_i2c1_tx_dma.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hGPDMA1_CH0, &xfer_cfg_i2c1_tx_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for GPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH0_IRQn);

  /* Link the Transmit DMA handle to the SMBUS handle */
  if (HAL_SMBUS_SetTxDMA(&hI2C1, &hGPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  /* Configure the DMA RX */

  /* Configure the GPDMA1_CH1 DMA channel for I2C1 RX */
  if (HAL_DMA_Init(&hGPDMA1_CH1, HAL_GPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  hal_dma_direct_xfer_config_t xfer_cfg_i2c1_rx_dma;
  xfer_cfg_i2c1_rx_dma.request         = HAL_GPDMA1_REQUEST_I2C1_RX;
  xfer_cfg_i2c1_rx_dma.direction       = HAL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  xfer_cfg_i2c1_rx_dma.src_inc         = HAL_DMA_SRC_ADDR_FIXED;
  xfer_cfg_i2c1_rx_dma.dest_inc        = HAL_DMA_DEST_ADDR_INCREMENTED;
  xfer_cfg_i2c1_rx_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_BYTE;
  xfer_cfg_i2c1_rx_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_BYTE;
  xfer_cfg_i2c1_rx_dma.priority        = HAL_DMA_PRIORITY_LOW_WEIGHT_LOW;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hGPDMA1_CH1, &xfer_cfg_i2c1_rx_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for GPDMA1_CH1 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH1_IRQn);

  /* Link the Receive DMA handle to the SMBUS handle */
  if (HAL_SMBUS_SetRxDMA(&hI2C1, &hGPDMA1_CH1) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_RCC_I2C1_SetKernelClkSource(HAL_RCC_I2C1_CLK_SRC_PCLK1) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the Event interruption for I2C */
  HAL_CORTEX_NVIC_SetPriority(I2C1_EV_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(I2C1_EV_IRQn);

  /* Enable the Error interruption for I2C */
  HAL_CORTEX_NVIC_SetPriority(I2C1_ERR_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(I2C1_ERR_IRQn);

  return &hI2C1;
}

void mx_i2c1_smbus_deinit(void)
{
  /* Disable the Event interruption for I2C */
  HAL_CORTEX_NVIC_DisableIRQ(I2C1_EV_IRQn);

  /* Disable the Error interruption for I2C */
  HAL_CORTEX_NVIC_DisableIRQ(I2C1_ERR_IRQn);

  (void)HAL_SMBUS_DeInit(&hI2C1);

  HAL_RCC_I2C1_Reset();

  HAL_RCC_I2C1_DisableClock();

  /* De-initialize all GPIOA pins associated with I2C1 */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_15);

  /* De-initialize all GPIOB pins associated with I2C1 */
  HAL_GPIO_DeInit(HAL_GPIOB, HAL_GPIO_PIN_9);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hGPDMA1_CH0);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH0_IRQn);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hGPDMA1_CH1);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH1_IRQn);
}

hal_smbus_handle_t *mx_i2c1_smbus_gethandle(void)
{
  return &hI2C1;
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
/*                            I2C1 event interrupt                            */
/******************************************************************************/
void I2C1_EV_IRQHandler(void)
{
  HAL_SMBUS_EV_IRQHandler(&hI2C1);
}

/******************************************************************************/
/*                            I2C1 error interrupt                            */
/******************************************************************************/
void I2C1_ERR_IRQHandler(void)
{
  HAL_SMBUS_ERR_IRQHandler(&hI2C1);
}
