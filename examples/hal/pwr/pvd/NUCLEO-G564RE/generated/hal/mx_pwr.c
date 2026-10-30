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
static hal_exti_handle_t hEXTI16;

/******************************************************************************/
/* Exported functions for PWR in HAL layer                                    */
/******************************************************************************/

system_status_t mx_pwr_init(void)
{
  /****************************************************************************/
  /* Configuration of PVD (Programmable Voltage Detector)                     */
  /****************************************************************************/
  /* Configure PVD Level */
  HAL_PWR_SetProgrammableVoltageDetectorLevel(HAL_PWR_PVD_VDD_LEVEL_0);

  /* Enable PVD */
  HAL_PWR_EnableProgrammableVoltageDetector();

  /* Configure PVD EXTI line */
  hal_exti_config_t exti_config;

  /* Initialize the EXTI for line 16 */
  HAL_EXTI_Init(&hEXTI16, HAL_EXTI_LINE_16);

  /* Set the trigger as RISING */
  exti_config.trigger = HAL_EXTI_TRIGGER_RISING;
  HAL_EXTI_SetConfig(&hEXTI16, &exti_config);

  /* Set line 16 Interrupt priority */
  HAL_CORTEX_NVIC_SetPriority(PWR_PVD_AVD_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(PWR_PVD_AVD_IRQn);

  /****************************************************************************/
  /* Configuration of the UCPD (USB Type-C and Power Delivery) PWR features   */
  /****************************************************************************/
  /* Disable Dead Battery mode */
  HAL_PWR_LP_DisableUCPDDeadBattery();

  /* Disable UCPD configuration retention in Standby mode */
  HAL_PWR_LP_DisableUCPDRetention();

  return SYSTEM_OK;
}

/******************************************************************************/
/* EXTI get handle                                                            */
/******************************************************************************/
hal_exti_handle_t *mx_pwr_exti16_gethandle(void)
{
  return &hEXTI16;
}

system_status_t mx_pwr_deinit(void)
{
  /* Deinit PVD EXTI line */
  /* De-initialize the EXTI for line16 */
  HAL_EXTI_DeInit(&hEXTI16);

  /* Set line 16 Interrupt priority */
  HAL_CORTEX_NVIC_DisableIRQ(PWR_PVD_AVD_IRQn);

  return SYSTEM_OK;
}

/******************************************************************************/
/* Power Voltage Monitor/Analog Voltage Monitor(PWR_PVD, PWR_AVD) */
/******************************************************************************/
void PWR_PVD_AVD_IRQHandler(void)
{
  HAL_EXTI_IRQHandler(&hEXTI16);
}
