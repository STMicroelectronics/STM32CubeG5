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

static hal_dma_handle_t hGPDMA1_CH0;

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
  config.prescaler              = 4;
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

  hal_tim_ic_channel_config_t ic_config;

  ic_config.source    = HAL_TIM_INPUT_TIM2_TI1_FDCAN1_RXEOF_EVT;
  ic_config.polarity  = HAL_TIM_IC_RISING;
  ic_config.filter    = HAL_TIM_FDIV1;
  if (HAL_TIM_IC_SetConfigChannel(&hTIM2, HAL_TIM_CHANNEL_1, &ic_config) != HAL_OK)
  {
    return NULL;
  }

  hal_tim_ic_capture_unit_config_t ic_capture_unit_config;

  ic_capture_unit_config.source     = HAL_TIM_IC_DIRECT;
  ic_capture_unit_config.prescaler  = HAL_TIM_IC_DIV1;
  if (HAL_TIM_IC_SetConfigCaptureUnit(&hTIM2, HAL_TIM_IC_CAPTURE_UNIT_1, &ic_capture_unit_config) != HAL_OK)
  {
    return NULL;
  }

  ic_config.source    = HAL_TIM_INPUT_TIM2_TI2_FDCAN1_TXEOF_EVT;
  ic_config.polarity  = HAL_TIM_IC_RISING;
  ic_config.filter    = HAL_TIM_FDIV1;
  if (HAL_TIM_IC_SetConfigChannel(&hTIM2, HAL_TIM_CHANNEL_2, &ic_config) != HAL_OK)
  {
    return NULL;
  }

  ic_capture_unit_config.source     = HAL_TIM_IC_DIRECT;
  ic_capture_unit_config.prescaler  = HAL_TIM_IC_DIV1;
  if (HAL_TIM_IC_SetConfigCaptureUnit(&hTIM2, HAL_TIM_IC_CAPTURE_UNIT_2, &ic_capture_unit_config) != HAL_OK)
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

  /* No GPIO configuration required for TIM2 */
  /* Enable the Timer global interrupt */
  HAL_CORTEX_NVIC_SetPriority(TIM2_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(TIM2_IRQn);

  /* Timer Capture/Compare 2 DMA Request Configuration */

  /* Configure the GPDMA1_CH0 DMA channel for TIM2 cc2 */
  if (HAL_DMA_Init(&hGPDMA1_CH0, HAL_GPDMA1_CH0) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_GPDMA1_EnableClock();

  hal_dma_direct_xfer_config_t xfer_cfg_tim2_cc2_dma;
  xfer_cfg_tim2_cc2_dma.request         = HAL_GPDMA1_REQUEST_TIM2_CC2;
  xfer_cfg_tim2_cc2_dma.direction       = HAL_DMA_DIRECTION_PERIPH_TO_MEMORY;
  xfer_cfg_tim2_cc2_dma.src_inc         = HAL_DMA_SRC_ADDR_FIXED;
  xfer_cfg_tim2_cc2_dma.dest_inc        = HAL_DMA_DEST_ADDR_INCREMENTED;
  xfer_cfg_tim2_cc2_dma.src_data_width  = HAL_DMA_SRC_DATA_WIDTH_WORD;
  xfer_cfg_tim2_cc2_dma.dest_data_width = HAL_DMA_DEST_DATA_WIDTH_WORD;
  xfer_cfg_tim2_cc2_dma.priority        = HAL_DMA_PRIORITY_HIGH;

  if (HAL_DMA_SetConfigPeriphDirectXfer(&hGPDMA1_CH0, &xfer_cfg_tim2_cc2_dma) != HAL_OK)
  {
    return NULL;
  }

  /* Enable the interruption for GPDMA1_CH0 */
  HAL_CORTEX_NVIC_SetPriority(GPDMA1_CH0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GPDMA1_CH0_IRQn);

/* Link Capture/Compare 2 DMA request handle to the TIM handle */
  if (HAL_TIM_SetDMA(&hTIM2, HAL_TIM_DMA_ID_CC2, &hGPDMA1_CH0) != HAL_OK)
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

  /* No GPIO de-initialization required for TIM2 */
  /* Disable Timer global interrupt */
  HAL_CORTEX_NVIC_DisableIRQ(TIM2_IRQn);

  /* De-initialize the DMA channel */
  HAL_DMA_DeInit(&hGPDMA1_CH0);

  /* Disable the interruption for DMA */
  HAL_CORTEX_NVIC_DisableIRQ(GPDMA1_CH0_IRQn);
}

hal_tim_handle_t *mx_tim2_gethandle(void)
{
  return &hTIM2;
}

/******************************************************************************/
/*                           TIM2 global interrupt                            */
/******************************************************************************/
void TIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&hTIM2);
}

/******************************************************************************/
/*                     GPDMA1 channel 0 global interrupt                      */
/******************************************************************************/
void GPDMA1_CH0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hGPDMA1_CH0);
}
