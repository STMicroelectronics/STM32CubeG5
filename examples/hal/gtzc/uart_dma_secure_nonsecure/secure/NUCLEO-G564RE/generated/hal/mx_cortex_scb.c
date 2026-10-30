/**
  ******************************************************************************
  * @file           : mx_cortex_scb.c
  * @brief          : CORTEX_SCB peripheral initialization
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

/* Includes ------------------------------------------------------------------*/
#include "mx_cortex_scb.h"

/* Exported functions --------------------------------------------------------*/
system_status_t mx_cortex_scb_init(void)
{
  HAL_CORTEX_SCB_DisableHardFaultEscalation(HAL_CORTEX_SCB_USAGE_FAULT | HAL_CORTEX_SCB_BUS_FAULT | HAL_CORTEX_SCB_SECURE_FAULT);

  HAL_CORTEX_NVIC_SetPriority(MemoryManagement_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_SetPriority(BusFault_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_SetPriority(UsageFault_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_SetPriority(SecureFault_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);

  return SYSTEM_OK;
}

void mx_cortex_scb_deinit(void)
{
  HAL_CORTEX_SCB_EnableHardFaultEscalation(HAL_CORTEX_SCB_USAGE_FAULT | HAL_CORTEX_SCB_BUS_FAULT | HAL_CORTEX_SCB_MEM_MANAGEMENT_FAULT | HAL_CORTEX_SCB_SECURE_FAULT);
}