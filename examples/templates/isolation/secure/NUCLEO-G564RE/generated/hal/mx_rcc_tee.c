/**
  ******************************************************************************
  * @file           : mx_rcc_tee.c
  * @brief          : STM32 RCC TEE program body
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
#include "mx_rcc_tee.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

system_status_t mx_rcc_tee_setup(void)
{
  /*
   * Full access security path:
   * - Configure RCC clocks security/privilege/lock
   * - other RCC systems security/privilege configuration (GPIO/EXTI)
   */
  /* Configure the RCC as NSecure and NPrivilege : sharing clock between Secure and NSecure : workaround */
  /* Configure security attributes for RCC peripheral */
  if (HAL_RCC_SetSecAttr(HAL_RCC_SEC_ITEM_ALL, HAL_RCC_NSEC) != HAL_OK)
  {
    return SYSTEM_RESOURCES_ISOLATION_ERROR;
  }

  /* Configure privilege attributes for RCC peripheral */
  if (HAL_RCC_SetPrivAttr(HAL_RCC_PRIV_ITEM_ALL, HAL_RCC_NPRIV) != HAL_OK)
  {
    return SYSTEM_RESOURCES_ISOLATION_ERROR;
  }

  return SYSTEM_OK;
}