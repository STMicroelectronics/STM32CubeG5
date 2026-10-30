/**
  ******************************************************************************
  * @file           : mx_tamp.c
  * @brief          : TAMP Peripheral initialization
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
#include "mx_tamp.h"
#include "mx_rcc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/* Exported functions for TAMP in HAL layer */
/******************************************************************************/
system_status_t mx_tamp_init(void)
{
  hal_tamp_active_config_t              p_active_global_config;
  hal_tamp_active_individual_config_t   p_active_indiv_config;
  /* Disable RTC Domain Write Protection */
  HAL_PWR_DisableRTCDomainWriteProtection();

  if (mx_rcc_tamp_clock_config() != SYSTEM_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  HAL_RCC_RTCAPB_EnableClock();

  HAL_RCC_RTC_EnableKernelClock();

  p_active_global_config.output_period = 0;
  p_active_global_config.filter = HAL_TAMP_ACTIVE_FILTER_DISABLE;
  p_active_global_config.asynchronous_prescaler = HAL_TAMP_ACTIVE_PRESCALER_RTCCLK;
  p_active_global_config.shared_output = HAL_TAMP_ACTIVE_TAMPER_OUTPUT_NOT_SHARED;
  if (HAL_TAMP_ACTIVE_SetConfig(&p_active_global_config) != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  p_active_indiv_config.tamper_output = HAL_TAMP_ACTIVE_TAMPER_OUTPUT_2;
  p_active_indiv_config.erase_secrets = HAL_TAMP_ACTIVE_SECRETS_ERASE;
  if (HAL_TAMP_ACTIVE_SetConfigTampers(MX_TAMP_X, &p_active_indiv_config) != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  /* Enable the interruption for TAMP */
  HAL_CORTEX_NVIC_SetPriority(TAMP_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(TAMP_IRQn);

  return SYSTEM_OK;
}

void mx_tamp_deinit(void)
{
  /* Disable the interuption for TAMP */
  HAL_CORTEX_NVIC_DisableIRQ(TAMP_IRQn);
}

/******************************************************************************/
/*                          Tamper global interrupts                          */
/******************************************************************************/
void TAMP_IRQHandler(void)
{
  HAL_TAMP_IRQHandler();
}
