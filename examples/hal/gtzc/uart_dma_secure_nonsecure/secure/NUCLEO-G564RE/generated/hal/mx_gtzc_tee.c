/**
  ******************************************************************************
  * @file           : mx_gtzc_tee.c
  * @brief          : GTZC trusted execution environment configuration
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
#include "mx_gtzc_tee.h"

/******************************************************************************/
/* Exported functions for GTZC in HAL layer (SW instance MyGTZC_1) */
/******************************************************************************/
system_status_t mx_gtzc_tee_setup(void)
{
  if ((HAL_GTZC_MPCBB_SetPrivAttrByAddr(HAL_GTZC_MPCBB_SRAM1, 0U, SRAM1_SIZE, HAL_GTZC_NPRIV) != HAL_OK)
      || (HAL_GTZC_MPCBB_SetPrivAttrByAddr(HAL_GTZC_MPCBB_SRAM2, 0U, SRAM2_SIZE, HAL_GTZC_NPRIV) != HAL_OK)
      || (HAL_GTZC_MPCBB_SetPrivAttrByAddr(HAL_GTZC_MPCBB_SRAM3, 0U, SRAM3_SIZE, HAL_GTZC_NPRIV) != HAL_OK)
      || (HAL_GTZC_MPCBB_SetSecAttrByAddr(HAL_GTZC_MPCBB_SRAM2, 0U, SRAM2_SIZE, HAL_GTZC_NSEC) != HAL_OK)
      || (HAL_GTZC_MPCBB_SetSecAttrByAddr(HAL_GTZC_MPCBB_SRAM3, 0U, SRAM3_SIZE, HAL_GTZC_NSEC) != HAL_OK))
  {
    return SYSTEM_RESOURCES_ISOLATION_ERROR;
  }

  /* Enable all illegal access */
  if (HAL_GTZC_TZIC_EnableIT(HAL_GTZC_TZIC_PERIPH_ALL) != HAL_OK)
  {
    return SYSTEM_RESOURCES_ISOLATION_ERROR;
  }
  /* Disable the Flash memory access : flashing workaround */
  if (HAL_GTZC_TZIC_DisableIT(HAL_GTZC_TZIC_FLASH_MEM) != HAL_OK)
  {
    return SYSTEM_RESOURCES_ISOLATION_ERROR;
  }

  HAL_CORTEX_NVIC_SetPriority(GTZC1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(GTZC1_IRQn);

  return SYSTEM_OK;
}

/******************************************************************************/
/*                      Illegal access global interrupt                       */
/******************************************************************************/
void GTZC1_IRQHandler(void)
{
  HAL_GTZC_TZIC_IRQHandler();
}
