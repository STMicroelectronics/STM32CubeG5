
/**
  ******************************************************************************
  * @file           : mx_gtzc_tee.c
  * @brief          : GTZC Peripheral initialization
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define GTZC_MAX_SUPER_BLOCK_2    8U  /*!< Number of super blocks for MPCBB2 */
#define GTZC_MAX_SUPER_BLOCK_3    2U  /*!< Number of super blocks for MPCBB3 */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/* Exported functions for GTZC in HAL layer (SW instance MyGTZC_1) */
/******************************************************************************/
system_status_t mx_gtzc_tee_setup(void)
{
  /* Configuration of the secure attributes for SRAM2 */
  if (HAL_GTZC_MPCBB_SetSecAttrByBlock(HAL_GTZC_MPCBB_SRAM2, 0U, GTZC_MAX_SUPER_BLOCK_2, HAL_GTZC_NSEC) != HAL_OK)
  {
    return SYSTEM_RESOURCES_ISOLATION_ERROR;
  }

  /* Configuration of the secure attributes for SRAM3 */
  if (HAL_GTZC_MPCBB_SetSecAttrByBlock(HAL_GTZC_MPCBB_SRAM3, 0U, GTZC_MAX_SUPER_BLOCK_3, HAL_GTZC_NSEC) != HAL_OK)
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
