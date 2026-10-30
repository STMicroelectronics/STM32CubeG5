/**
  ******************************************************************************
  * @file           : mx_pwr.c
  * @brief          : PWR Peripheral initialization
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
#include "mx_pwr.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/******************************************************************************/
/* Exported functions for PWR in HAL layer                                    */
/******************************************************************************/

system_status_t mx_pwr_init(void)
{
  /****************************************************************************/
  /* Configuration of RAM retention                                           */
  /****************************************************************************/
  /* For SRAM1 in stop mode, retention is disabled */
  HAL_PWR_LP_DisableMemoryRetention(HAL_PWR_MEMORY_RETENTION_SRAM1_STOP);

  /* For SRAM2 in stop mode, retention is enabled only for pages 1 */
  HAL_PWR_LP_DisableMemoryPageRetention(HAL_PWR_MEMORY_RETENTION_SRAM2_STOP, 2, 1);

  /* For SRAM3 in stop mode, retention is disabled */
  HAL_PWR_LP_DisableMemoryRetention(HAL_PWR_MEMORY_RETENTION_SRAM3_STOP);

  /****************************************************************************/
  /* Configuration of FLASH low power mode                                    */
  /****************************************************************************/
  HAL_PWR_LP_EnableFlashLowPWRMode();

  /****************************************************************************/
  /* Configuration of the UCPD (USB Type-C and Power Delivery) PWR features   */
  /****************************************************************************/
  /* Disable Dead Battery mode */
  HAL_PWR_LP_DisableUCPDDeadBattery();

  /* Disable UCPD configuration retention in Standby mode */
  HAL_PWR_LP_DisableUCPDRetention();

  return SYSTEM_OK;
}

system_status_t mx_pwr_deinit(void)
{
  return SYSTEM_OK;
}
