/**
  ******************************************************************************
  * file           : ll_example.h
  * brief          : Header for stm32ynxx_ll_example.c files.
  *                  This file contains example-specific declarations to interface with example.c functions.
  ******************************************************************************
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LL_EXAMPLE_H
#define LL_EXAMPLE_H

/* Includes ------------------------------------------------------------------*/
#include "mx_hal_def.h"       /* aliases to the target-specific generated code */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported types ------------------------------------------------------------*/
/* MX_CORDICx data buffer descriptor. */
typedef struct
{
  int32_t  *p_data;   /*  data pointer               */
  uint32_t size_word; /*  data buffer size in words. */
} cordic_buffer_desc_t;

/* Exported constants --------------------------------------------------------*/
#ifndef USE_LL_APP_TIMEOUT
#define USE_LL_APP_TIMEOUT 0U
#endif /* USE_LL_APP_TIMEOUT */

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** brief:  Starts a MX_CORDICx computation in polling mode.
  * param : p_src_buffer    Pointer to the input data buffer and the buffer size.
  * param : p_dst_buffer    Pointer to the output data buffer and the buffer size.
  * param : timeout_ms     Timeout duration in millisecond (This parameter is unused when USE_LL_APP_TIMEOUT == 0).
  * retval: system status (see system_status_t).
  */
system_status_t CORDIC_Calculate(const cordic_buffer_desc_t *p_src_buffer, cordic_buffer_desc_t *p_dst_buffer,
                                 uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
