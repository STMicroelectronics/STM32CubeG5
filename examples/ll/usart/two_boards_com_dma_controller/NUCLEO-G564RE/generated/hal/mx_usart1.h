/**
  ******************************************************************************
  * @file           : mx_usart1.h
  * @brief          : Header for mx_usart1.c file.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_USART1_H
#define MX_USART1_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_USART1

/* USART1 TX / GPDMA1_CH0 DMA channel aliases */
#define MX_LPDMAX_CH1Y_CH                     LL_GPDMA1_CH0

/* USART1 RX / GPDMA1_CH1 DMA channel aliases */
#define MX_LPDMAX_CH2Y_CH                     LL_GPDMA1_CH1

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/******************************************************************************/
/* Exported functions for USART in LL layer */
/******************************************************************************/
/**
  * @brief mx_usart1_usart init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
USART_TypeDef *mx_usart1_usart_init(void);

/**
  * @brief  De-initialize mx_usart1_usart instance and return it.
  * @retval None
  */
void mx_usart1_usart_deinit(void);

/******************************************************************************/
/*         USART1 global interrupt is managed directly in user code.          */
/******************************************************************************/
/* void USART1_IRQHandler(void); */

/******************************************************************************/
/*    GPDMA1 channel 0 global interrupt is managed directly in user code.     */
/******************************************************************************/
/* void GPDMA1_CH0_IRQHandler(void); */

/******************************************************************************/
/*    GPDMA1 channel 1 global interrupt is managed directly in user code.     */
/******************************************************************************/
/* void GPDMA1_CH1_IRQHandler(void); */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_USART1_H */
