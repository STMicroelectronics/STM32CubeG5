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
#endif /* __cplusplus */

/* Exported macros -----------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint32_t IsPasswordCorrect(const uint8_t *password_received, uint32_t password_size_byte);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SECURE_NSCLIB_H */
