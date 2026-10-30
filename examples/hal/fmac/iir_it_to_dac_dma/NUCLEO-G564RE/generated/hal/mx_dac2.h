/**
  ******************************************************************************
  * @file           : mx_dac2.h
  * @brief          : Header for mx_dac2.c file.
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
#ifndef MX_DAC2_H
#define MX_DAC2_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/******************************************************************************/
/* Exported functions for DAC in HAL layer */
/******************************************************************************/
/**
  * @brief mx_dac2 init function
  * This function configures the hardware resources used in this example
  * @retval pointer to handle or NULL in case of failure
  */
hal_dac_handle_t *mx_dac2_init(void);

/**
  * @brief  De-initialize dac2 instance and return it.
  */
void mx_dac2_deinit(void);

/**
  * @brief  Get the DAC2 object.
  * @retval Pointer on the DAC2 Handle
  */
hal_dac_handle_t *mx_dac2_gethandle(void);

/******************************************************************************/
/*                     GPDMA1 channel 0 global interrupt                      */
/******************************************************************************/
void GPDMA1_CH0_IRQHandler(void);

/******************************************************************************/
/*                           DAC2 global interrupt                            */
/******************************************************************************/
void DAC2_IRQHandler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_DAC2_H */
