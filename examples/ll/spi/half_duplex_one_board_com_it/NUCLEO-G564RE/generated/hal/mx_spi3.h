/**
  ******************************************************************************
  * @file           : mx_spi3.h
  * @brief          : Header for mx_spi3.c file.
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
#ifndef MX_SPI3_H
#define MX_SPI3_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_ll.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define USE_SPI3

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for SPI in LL layer */
/******************************************************************************/
/**
  * @brief mx_spi3 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
SPI_TypeDef *mx_spi3_init(void);

/**
  * @brief  De-initialize spi3 instance and return it.
  */
void mx_spi3_deinit(void);

/******************************************************************************/
/*          SPI3 global interrupt is managed directly in user code.           */
/******************************************************************************/
/* void SPI3_IRQHandler(void); */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_SPI3_H */
