/**
  ******************************************************************************
  * @file           : mx_filex_app.h
  * @brief          : Header for mx_filex_app.c file.
  *                  This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_filex_license.md file
  * in the same directory as the generated code.
  * If no mx_filex_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __MX_FILEX_APP_H__
#define __MX_FILEX_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "fx_api.h"
#include "mx_filex_interfaces.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
UINT app_filex_init(VOID);
UINT app_filex_process(VOID);
/* Private defines -----------------------------------------------------------*/
/** logging macro - just redirects to printf()
  * libc's stdout is redirected to UART thanks to the Basic stdio utility.
  *
  * @user You can enable logs by defining USE_TRACE=1 in the build options.
  */
#if defined(USE_TRACE) && USE_TRACE != 0
#define PRINTF(...)    printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif /* defined(USE_TRACE) && USE_TRACE != 0 */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* MX_APP_FILEX_H */
