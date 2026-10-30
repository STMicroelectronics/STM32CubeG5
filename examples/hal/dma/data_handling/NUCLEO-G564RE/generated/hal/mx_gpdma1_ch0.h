/**
  ******************************************************************************
  * @file           : mx_gpdma1_ch0.h
  * @brief          : Header for mx_gpdma1_ch0.c file.
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

/* Define to prevent recursive inclusion ----------------------------------------------------------------------------*/
#ifndef MX_GPDMA1_CH0_H
#define MX_GPDMA1_CH0_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ---------------------------------------------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/******************************************************************************/
/* Exported functions for DMA in HAL layer */
/******************************************************************************/
/**
  * @brief  Initialize GPDMA1_CH0 for the "right aligned zero padding" data handling variant.
  * @retval Pointer to the DMA handle, NULL in case of failure.
  */
hal_dma_handle_t *mx_gpdma1_ch0_cfg1_init(void);

/**
  * @brief  Initialize GPDMA1_CH0 for the "right aligned sign extended" data handling variant.
  * @retval Pointer to the DMA handle, NULL in case of failure.
  */
hal_dma_handle_t *mx_gpdma1_ch0_cfg2_init(void);

/**
  * @brief  Initialize GPDMA1_CH0 for the "right aligned left truncated" data handling variant.
  * @retval Pointer to the DMA handle, NULL in case of failure.
  */
hal_dma_handle_t *mx_gpdma1_ch0_cfg3_init(void);

/**
  * @brief  Initialize GPDMA1_CH0 for the "left aligned right truncated" data handling variant.
  * @retval Pointer to the DMA handle, NULL in case of failure.
  */
hal_dma_handle_t *mx_gpdma1_ch0_cfg4_init(void);

/**
  * @brief  Initialize GPDMA1_CH0 for the "source byte exchange" data handling variant.
  * @retval Pointer to the DMA handle, NULL in case of failure.
  */
hal_dma_handle_t *mx_gpdma1_ch0_cfg5_init(void);

/**
  * @brief  Initialize GPDMA1_CH0 for the "destination byte exchange" data handling variant.
  * @retval Pointer to the DMA handle, NULL in case of failure.
  */
hal_dma_handle_t *mx_gpdma1_ch0_cfg6_init(void);

/**
  * @brief  Initialize GPDMA1_CH0 for the "destination half-word exchange" data handling variant.
  * @retval Pointer to the DMA handle, NULL in case of failure.
  */
hal_dma_handle_t *mx_gpdma1_ch0_cfg7_init(void);

/**
  * @brief  De-initialize GPDMA1_CH0 instance.
  * @retval -1 in case of failure, 0 otherwise.
  */
int32_t mx_gpdma1_ch0_deinit(void);

/**
  * @brief  Get the GPDMA1_CH0 object.
  * @retval Pointer on the GPDMA1_CH0 Handle
  */
hal_dma_handle_t *mx_gpdma1_ch0_gethandle(void);

/******************************************************************************/
/* GPDMA1 channel 0 global interrupt */
/******************************************************************************/
void GPDMA1_CH0_IRQHandler(void);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_GPDMA1_CH0_H */
