/**
  ******************************************************************************
  * @file           : mx_lptim1.c
  * @brief          : LPTIM1 Peripheral initialization
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
#include "mx_lptim1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/* Exported variables by reference--------------------------------------------*/
static hal_lptim_handle_t hLPTIM1;

static hal_dma_handle_t hGPDMA1_CH0;
/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for SW instance in HAL layer */
/******************************************************************************/

hal_lptim_handle_t *mx_lptim1_init(void)
{
  /* Init GPIO */
  /* ### LPTIM1 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOB_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name]

       PB2     ------>   LPTIM1_CH1
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_UP;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_MEDIUM;
  gpio_config.alternate   = HAL_GPIO_AF_9;
  HAL_GPIO_Init(HAL_GPIOB, HAL_GPIO_PIN_2, &gpio_config);

  hal_lptim_config_t  config;

  if (HAL_LPTIM_Init(&hLPTIM1, HAL_LPTIM1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_LPTIM1_EnableClock();

  if (HAL_RCC_LPTIM1_SetKernelClkSource(HAL_RCC_LPTIM1_CLK_SRC_LSE) != HAL_OK)
  {
    return NULL;
  }

  /* Timer configuration to reach the output frequency at 0.5 Hz */
  config.mode               = HAL_LPTIM_CONTINUOUS;
  config.clock_source       = HAL_LPTIM_CLK_INTERNAL;
  config.period             = 0xFFFF;
  config.prescaler          = HAL_LPTIM_CLK_SRC_DIV1;
  config.repetition_counter = 0;

  if (HAL_LPTIM_SetConfig(&hLPTIM1, &config) != HAL_OK)
  {
    return NULL;
  }

  hal_lptim_ic_config_t ic_config1;
  ic_config1.source = HAL_LPTIM_INPUT_GPIO;
  ic_config1.polarity = HAL_LPTIM_IC_RISING;
  ic_config1.prescaler = HAL_LPTIM_IC_DIV1;
  ic_config1.filter = HAL_LPTIM_FDIV1;

  if (HAL_LPTIM_IC_SetConfigChannel(&hLPTIM1, HAL_LPTIM_CHANNEL_1, &ic_config1) != HAL_OK)
  {
    return NULL;
  }

  /* Configure the GPDMA1_CH0 DMA channel for LPTIM1 IC1 */
  if (HAL_DMA_Init(&hGPDMA1_CH0, HAL_GPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPDMA1_EnableClock();

  hal_dma_direct_xfer_config_t xfer_cfg_lptim1_ic1_dma;
  xfer_cfg_lptim1_ic1_dma.request         = HAL_GPDMA1_REQUEST_LPTIM1_IC1;
  xfer_cfg_lptim1_ic1_dma.direction       = HAL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  xfer_cfg_lptim1_ic1_dma.src_inc         = HAL_DMA_SRC_ADDR_FIXED;
  xfer_cfg_lptim1_ic1_dma.dest_inc        = HAL_DMA_DEST_ADDR_INCREMENTED;
  xfer_cfg_lptim1_ic1_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_HALFWORD;
  xfer_cfg_lptim1_ic1_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_HALFWORD;
  xfer_cfg_lptim1_ic1_dma.priority        = HAL_DMA_PRIORITY_HIGH;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hGPDMA1_CH0, &xfer_cfg_lptim1_ic1_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for GPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH0_IRQn);

/* Link Channel 1 DMA request handle to the LPTIM handle */
  if (HAL_LPTIM_SetDMA(&hLPTIM1, HAL_LPTIM_DMA_ID_CC1, &hGPDMA1_CH0) != HAL_OK)
  {
     return NULL;
  }

  return &hLPTIM1;
}

void mx_lptim1_deinit(void)
{
  (void)HAL_LPTIM_DeInit(&hLPTIM1);

  HAL_RCC_LPTIM1_Reset();

  HAL_RCC_LPTIM1_DisableClock();

  /* De-initialize all GPIOB pins associated with LPTIM1 */
  HAL_GPIO_DeInit(HAL_GPIOB, HAL_GPIO_PIN_2);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hGPDMA1_CH0);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH0_IRQn);
}

hal_lptim_handle_t *mx_lptim1_gethandle(void)
{
  return &hLPTIM1;
}

/******************************************************************************/
/*                     GPDMA1 channel 0 global interrupt                      */
/******************************************************************************/
void GPDMA1_CH0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hGPDMA1_CH0);
}
