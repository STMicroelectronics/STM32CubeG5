
/**
  ******************************************************************************
  * @file           : mx_tee.c
  * @brief          : STM32 tee (trusted execution environment) body
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
#include "mx_def.h"
#include "mx_hal_def.h"
#include "mx_tee.h"
#include "mx_gtzc_tee.h"
#include "mx_rcc_tee.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * Peripherals isolation
  */
system_status_t mx_tee_setup(void)
{
  /* RCC peripheral isolation initialization */
  if (mx_rcc_tee_setup() != SYSTEM_OK)
  {
    return SYSTEM_RESOURCES_ISOLATION_ERROR;
  }

  /* GTZC peripheral initialization */
  if (mx_gtzc_tee_setup() != SYSTEM_OK)
  {
    return SYSTEM_RESOURCES_ISOLATION_ERROR;
  }

  return SYSTEM_OK;
}
