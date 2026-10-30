/**
  ******************************************************************************
  * @file           : mx_tim2.c
  * @brief          : Peripheral initialization
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
#include "mx_tim2.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_tim_handle_t hTIM2;

/* DMA nodes must be 8-byte aligned. */
__attribute__((section("non_cacheable_area"), aligned(8)))
static hal_dma_node_t DMA_Node_TIM2_CC1;

static hal_dma_handle_t hGPDMA1_CH2;

/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for TIM2 in HAL layer */
/******************************************************************************/
hal_tim_handle_t *mx_tim2_init(void)
{
  if (HAL_TIM_Init(&hTIM2, HAL_TIM2) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_TIM2_EnableClock();

  /* Timer configuration to reach the output frequency at 0 Hz */
  hal_tim_config_t config;
  config.clock_sel.clock_source = HAL_TIM_CLK_INTERNAL;
  config.prescaler              = 0;
  config.counter_mode           = HAL_TIM_COUNTER_UP;
  config.period                 = 0xFFFFFFFF;
  config.repetition_counter     = 0;
  if (HAL_TIM_SetConfig(&hTIM2, &config) != HAL_OK)
  {
    return NULL;
  }

  /* Sampling Clock */
  if (HAL_TIM_SetDTSPrescaler(&hTIM2, HAL_TIM_DTS_DIV1) != HAL_OK)
  {
    return NULL;
  }
  if (HAL_TIM_SetDTS2Prescaler(&hTIM2, HAL_TIM_DTS2_DIV1) != HAL_OK)
  {
    return NULL;
  }

  hal_tim_oc_channel_config_t oc_config;

  oc_config.polarity       = HAL_TIM_OC_HIGH;
  if (HAL_TIM_OC_SetConfigChannel(&hTIM2, HAL_TIM_CHANNEL_1, &oc_config) != HAL_OK)
  {
    return NULL;
  }
  hal_tim_oc_compare_unit_config_t oc_compare_unit_config;

  oc_compare_unit_config.mode  = HAL_TIM_OC_PWM1;
  oc_compare_unit_config.pulse = 0x00000000;
  if (HAL_TIM_OC_SetConfigCompareUnit(&hTIM2, HAL_TIM_OC_COMPARE_UNIT_1,
                                      &oc_compare_unit_config) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_TIM_OC_EnableComparePreload(&hTIM2, HAL_TIM_OC_COMPARE_UNIT_1) != HAL_OK)
  {
    return NULL;
  }

  /* Update Event Management */
  if (HAL_TIM_SetUpdateSource(&hTIM2, HAL_TIM_UPDATE_REGULAR) != HAL_OK)
  {
    return NULL;
  }
  if (HAL_TIM_EnableUpdateGeneration(&hTIM2) != HAL_OK)
  {
    return NULL;
  }

  /* Capture/Compare DMA request selection */
  if (HAL_TIM_SetCaptureCompareDMAReqSource(&hTIM2, HAL_TIM_CC_DMAREQ_CC) != HAL_OK)
  {
    return NULL;
  }

  /* ### TIM2 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOB_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name]

       PB7     ------>   TIM2_CH1
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_2;
  HAL_GPIO_Init(HAL_GPIOB, HAL_GPIO_PIN_7, &gpio_config);

  /* Timer Capture/Compare 1 DMA Request Configuration */

  /* Configure the GPDMA1_CH2 DMA channel for TIM2 cc1 */
  if (HAL_DMA_Init(&hGPDMA1_CH2, HAL_GPDMA1_CH2) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPDMA1_EnableClock();

  hal_dma_direct_xfer_config_t xfer_cfg_tim2_cc1_dma;
  xfer_cfg_tim2_cc1_dma.request         = HAL_GPDMA1_REQUEST_TIM2_CC1;
  xfer_cfg_tim2_cc1_dma.direction       = HAL_DMA_DIRECTION_MEMORY_TO_PERIPH;
  xfer_cfg_tim2_cc1_dma.src_inc         = HAL_DMA_SRC_ADDR_INCREMENTED;
  xfer_cfg_tim2_cc1_dma.dest_inc        = HAL_DMA_DEST_ADDR_FIXED;
  xfer_cfg_tim2_cc1_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_HALFWORD;
  xfer_cfg_tim2_cc1_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_HALFWORD;
  xfer_cfg_tim2_cc1_dma.priority        = HAL_DMA_PRIORITY_HIGH;

  if (HAL_DMA_SetConfigPeriphLinkedListCircularXfer(&hGPDMA1_CH2, &DMA_Node_TIM2_CC1, &xfer_cfg_tim2_cc1_dma) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_DMA_SetLinkedListXferFetchNodePort(&hGPDMA1_CH2, HAL_DMA_PORT0) != HAL_OK)
  {
    return NULL;
  }

  /* Set DMA channel trigger xfer configuration */
  {
    hal_dma_trigger_config_t trigger_cfg;

    trigger_cfg.source    = HAL_GPDMA1_TRIGGER_TIM6_TRGO;
    trigger_cfg.polarity  = HAL_DMA_TRIGGER_POLARITY_RISING;
    trigger_cfg.mode      = HAL_DMA_TRIGGER_SINGLE_BURST_TRANSFER;
    if (HAL_DMA_FillNodeTrigger( &DMA_Node_TIM2_CC1, &trigger_cfg) != HAL_OK)
    {
      return NULL;
    }
  }

  /* Enable the interruption for GPDMA1_CH2 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH2_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH2_IRQn);

/* Link Capture/Compare 1 DMA request handle to the TIM handle */
  if (HAL_TIM_SetDMA(&hTIM2, HAL_TIM_DMA_ID_CC1, &hGPDMA1_CH2) != HAL_OK)
  {
    return NULL;
  }

  return &hTIM2;
}

void mx_tim2_deinit(void)
{
  (void)HAL_TIM_DeInit(&hTIM2);

  HAL_RCC_TIM2_DisableClock();

  HAL_RCC_TIM2_Reset();

  /* De-initialize all GPIOB pins associated with TIM2 */
  HAL_GPIO_DeInit(HAL_GPIOB, HAL_GPIO_PIN_7);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hGPDMA1_CH2);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH2_IRQn);
}

hal_tim_handle_t *mx_tim2_gethandle(void)
{
  return &hTIM2;
}

/******************************************************************************/
/*                     GPDMA1 channel 2 global interrupt                      */
/******************************************************************************/
void GPDMA1_CH2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hGPDMA1_CH2);
}
