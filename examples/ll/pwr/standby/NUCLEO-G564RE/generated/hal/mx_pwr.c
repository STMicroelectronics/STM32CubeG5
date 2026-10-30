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
/* Exported functions for PWR in LL layer                                    */
/******************************************************************************/

system_status_t mx_pwr_init(void)
{
  /****************************************************************************/
  /* Configuration of the UCPD (USB Type-C and Power Delivery) PWR features   */
  /****************************************************************************/
  /* Disable Dead Battery mode */
  LL_PWR_DisableUCPDDeadBattery();

  /* Disable UCPD configuration retention in Standby mode */
  /* LL_PWR_DisableUCPDRetentionStandbyMode(); */ /* Configuration matches register reset state at startup. */

  /****************************************************************************/
  /* Wake-up pins configuration                                               */
  /****************************************************************************/
  /* Configuration of selected wake-up pins with pull DOWN and HIGH polarity */
  /* LL_PWR_SetWakeUpPinPolarity(LL_PWR_WAKEUP_PIN_4, LL_PWR_WAKEUP_PIN_POLARITY_HIGH); */ /* Configuration matches register reset state at startup. */
  LL_PWR_SetWakeUpPinPull(LL_PWR_WAKEUP_PIN_4, LL_PWR_WAKEUP_PIN_PULL_DOWN);

  /****************************************************************************/
  /* Enable all configured wake-up pins                                       */
  /****************************************************************************/
  LL_PWR_EnableWakeUpPin(LL_PWR_WAKEUP_PIN_4);

  return SYSTEM_OK;
}

system_status_t mx_pwr_deinit(void)
{
  /****************************************************************************/
  /* Disable all configured wake-up pins                                      */
  /****************************************************************************/
  LL_PWR_DisableWakeUpPin(LL_PWR_WAKEUP_PIN_4);

  return SYSTEM_OK;
}
