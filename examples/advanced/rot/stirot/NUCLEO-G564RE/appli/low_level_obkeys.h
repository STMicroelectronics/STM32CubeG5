/**
  ******************************************************************************
  * @file    low_level_obkeys.h
  * @author  MCD Application Team
  * @brief   Header for low_level_obkeys.c module
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LOW_LEVEL_OBKEYS_H
#define LOW_LEVEL_OBKEYS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int32_t OBK_ConfigureSaes(hal_aes_handle_t *p_hsaes);
int32_t OBK_Read(uint32_t Offset, void *pData, uint32_t Length);

#ifdef __cplusplus
}
#endif

#endif /* LOW_LEVEL_OBKEYS_H */
