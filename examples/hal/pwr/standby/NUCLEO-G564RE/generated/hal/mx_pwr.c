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
  hal_pwr_wakeup_pin_config_t wkup_config;

  /****************************************************************************/
  /* Configuration of the UCPD (USB Type-C and Power Delivery) PWR features   */
  /****************************************************************************/
  /* Disable Dead Battery mode */
  HAL_PWR_LP_DisableUCPDDeadBattery();

  /* Disable UCPD configuration retention in Standby mode */
  HAL_PWR_LP_DisableUCPDRetention();

  /****************************************************************************/
  /* Wake-up pins configuration                                               */
  /****************************************************************************/
  /* Configuration of selected wake-up pins with pull DOWN and HIGH polarity */
  wkup_config.polarity = HAL_PWR_WAKEUP_PIN_POLARITY_HIGH;
  wkup_config.pull     = HAL_PWR_WAKEUP_PIN_PULL_DOWN;

  if (HAL_PWR_LP_SetConfigWakeupPin(HAL_PWR_WAKEUP_PIN_4, &wkup_config) != HAL_OK)
  {
    return SYSTEM_POWER_ERROR;
  }

  /****************************************************************************/
  /* Enable all configured wake-up pins                                       */
  /****************************************************************************/
  HAL_PWR_LP_EnableWakeupPin(HAL_PWR_WAKEUP_PIN_4);

  return SYSTEM_OK;
}

system_status_t mx_pwr_deinit(void)
{
  /****************************************************************************/
  /* Disable all configured wake-up pins                                      */
  /****************************************************************************/
  HAL_PWR_LP_DisableWakeupPin(HAL_PWR_WAKEUP_PIN_4);

  return SYSTEM_OK;
}
