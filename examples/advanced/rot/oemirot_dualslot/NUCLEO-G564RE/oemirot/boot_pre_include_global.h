/**
  ******************************************************************************
  * @file    boot_pre_include_global.h
  * @brief   OEMiRoT global pre-include header
  ******************************************************************************
  * @attention
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

/*
 * This file is force-included at project scope (before every .c and .h file).
 * It is passed as --preinclude (IAR/AC6) or -include (GCC) to the compiler,
 * so every translation unit in the OEMiRoT project sees these
 * definitions without requiring an explicit #include directive.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Enable OEMiRoT developer mode (logging, error reporting) */
#define OEMIROT_DEV_MODE

/* Disable assert() side-effects in release builds */
#define NDEBUG

#ifdef __cplusplus
}
#endif
