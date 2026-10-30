/**
  ******************************************************************************
  * file           : secure_nsc_lib.h
  * brief          : Header for secure non-secure callable APIs list.
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
#ifndef SECURE_NSCLIB_H
#define SECURE_NSCLIB_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Exported macros -----------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint32_t func_template(uint32_t param);

#ifdef __cplusplus
}
#endif

#endif /* SECURE_NSCLIB_H */
