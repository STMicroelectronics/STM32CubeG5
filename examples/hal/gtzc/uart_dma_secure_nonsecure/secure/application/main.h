/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                  This file contains the common defines of the application.
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
#ifndef MAIN_H
#define MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "mx_system.h"      /* target-specific generated code providing system services */
#include "secure_nsclib.h"  /* non-secure-callable code */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void SecureFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MAIN_H */
