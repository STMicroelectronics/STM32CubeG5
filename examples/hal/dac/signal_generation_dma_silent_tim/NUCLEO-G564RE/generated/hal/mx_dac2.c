/**
  ******************************************************************************
  * @file           : mx_dac2.c
  * @brief          : DAC2 Peripheral initialization
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
#include "mx_dac2.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_dac_handle_t hDAC2;

/* DMA nodes must be 8-byte aligned. */
__attribute__((section("non_cacheable_area"), aligned(8)))
static hal_dma_node_t DMA_Node_DAC2_CH1;

static hal_dma_handle_t hGPDMA1_CH0;

/******************************************************************************/
/* Exported functions for DAC2 in HAL layer */
/******************************************************************************/
hal_dac_handle_t *mx_dac2_init(void)
{
  if (HAL_DAC_Init(&hDAC2, HAL_DAC2) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_DAC2_EnableClock();

  if (HAL_RCC_DAC2_SetKernelClkSource(HAL_RCC_DAC2_CLK_SRC_TIM) != HAL_OK)
  {
    return NULL;
  }

  /****************************************************************************/
  /* Initialization of DAC instance                                           */
  /****************************************************************************/

  hal_dac_config_t dac_config;
  dac_config.high_frequency_mode = HAL_DAC_HIGH_FREQ_MODE_ABOVE_160MHZ;
  if (HAL_DAC_SetConfig(&hDAC2, &dac_config) != HAL_OK)
  {
    return NULL;
  }

  /****************************************************************************/
  /* Configuration of DAC channel                                             */
  /****************************************************************************/

  hal_dac_channel_config_t dac_channel_config;

  /* ========== Channel 1 ========== */
  dac_channel_config.alignment         = HAL_DAC_DATA_ALIGN_12_BITS_RIGHT;
  dac_channel_config.trigger           = HAL_DAC_TRIGGER_TIM6_TRGO;
  dac_channel_config.output_buffer     = HAL_DAC_OUTPUT_BUFFER_ENABLED;
  dac_channel_config.output_connection = HAL_DAC_OUTPUT_CONNECTION_EXTERNAL;
  dac_channel_config.data_sign_format  = HAL_DAC_SIGN_FORMAT_UNSIGNED;
  if (HAL_DAC_SetConfigChannel(&hDAC2, HAL_DAC_CHANNEL_1, &dac_channel_config) != HAL_OK)
  {
    return NULL;
  }

  /****************************************************************************/
  /* Configuration of GPIO                                                    */
  /****************************************************************************/

  /* ### DAC2 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOA_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name]

       PA5     ------>   DAC2_OUT1
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ANALOG;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_5, &gpio_config);

  /****************************************************************************/
  /* Configuration of DMA                                                     */
  /****************************************************************************/

  /* ========== Channel 1 ========== */

  /* Configure the GPDMA1_CH0 DMA channel for DAC2_CH1 CH1 */
  if (HAL_DMA_Init(&hGPDMA1_CH0, HAL_GPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPDMA1_EnableClock();

  hal_dma_direct_xfer_config_t xfer_cfg_dac2_ch1_dma;
  xfer_cfg_dac2_ch1_dma.request         = HAL_GPDMA1_REQUEST_DAC2_CH1;
  xfer_cfg_dac2_ch1_dma.direction       = HAL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  xfer_cfg_dac2_ch1_dma.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
  xfer_cfg_dac2_ch1_dma.dest_inc        = HAL_DMA_DEST_ADDR_FIXED;
  xfer_cfg_dac2_ch1_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_HALFWORD;
  xfer_cfg_dac2_ch1_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_WORD;
  xfer_cfg_dac2_ch1_dma.priority        = HAL_DMA_PRIORITY_HIGH;

  if (HAL_DMA_SetConfigPeriphLinkedListCircularXfer(&hGPDMA1_CH0, &DMA_Node_DAC2_CH1, &xfer_cfg_dac2_ch1_dma) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_DMA_SetLinkedListXferFetchNodePort(&hGPDMA1_CH0, HAL_DMA_PORT0) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for GPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH0_IRQn);

  HAL_DAC_SetChannelDMA(&hDAC2, &hGPDMA1_CH0, HAL_DAC_CHANNEL_1);

  return &hDAC2;
}

void mx_dac2_deinit(void)
{
  (void)HAL_DAC_DeInit(&hDAC2);

  HAL_RCC_DAC2_DisableClock();

  HAL_RCC_DAC2_Reset();

  /* De-initialize all GPIOA pins associated with DAC2 */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_5);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hGPDMA1_CH0);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH0_IRQn);
}

hal_dac_handle_t *mx_dac2_gethandle(void)
{
  return &hDAC2;
}

/******************************************************************************/
/*                     GPDMA1 channel 0 global interrupt                      */
/******************************************************************************/
void GPDMA1_CH0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hGPDMA1_CH0);
}
