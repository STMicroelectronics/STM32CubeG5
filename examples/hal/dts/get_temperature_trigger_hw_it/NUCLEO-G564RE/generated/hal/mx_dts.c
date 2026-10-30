/**
  ******************************************************************************
  * @file           : mx_dts.c
  * @brief          : DTS Peripheral initialization
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
#include "mx_dts.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_dts_handle_t hDTS;

/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for DTS in HAL layer */
/******************************************************************************/
hal_dts_handle_t *mx_dts_init(void)
{
  hal_dts_config_t        p_config;

  HAL_RCC_DTS_EnableClock();

  if (HAL_DTS_Init(&hDTS, HAL_DTS) != HAL_OK)
  {
    return NULL;
  }

  p_config.reference_clock           = HAL_DTS_REFERENCE_CLOCK_PCLK;
  p_config.quick_measurement         = HAL_DTS_QUICK_MEASUREMENT_DISABLE;
  p_config.high_speed_clock_division = 0x1;

  if (HAL_DTS_SetConfig(&hDTS, &p_config) != HAL_OK)
  {
    return NULL;
  }

  hal_dts_sensor_config_t p_sensor_config;

  p_sensor_config.trigger       = HAL_DTS_TRIGGER_LPTIM1_CH1;
  p_sensor_config.sampling_time = HAL_DTS_SAMPLING_TIME_15_CYCLES;

  if (HAL_DTS_SetConfigSensor(&hDTS, HAL_DTS_SENSOR_1, &p_sensor_config) != HAL_OK)
  {
    return NULL;
  }

  /* DTS NVIC Configuration ****************************************************************************************/
  HAL_CORTEX_NVIC_SetPriority(DTS_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(DTS_IRQn);

  return &hDTS;
}

void mx_dts_deinit(void)
{
  /* De-initialize the DTS */
  HAL_DTS_DeInit(&hDTS);

  /* Disable the DTS interrupt request */
  HAL_CORTEX_NVIC_DisableIRQ(DTS_IRQn);

  HAL_RCC_DTS_DisableClock();

  HAL_RCC_DTS_Reset();
}

hal_dts_handle_t *mx_dts_gethandle(void)
{
  return &hDTS;
}

/******************************************************************************/
/*                 DTS interrupt OR DTS AIT through EXTI line                 */
/******************************************************************************/
void DTS_IRQHandler(void)
{
  HAL_DTS_IRQHandler(&hDTS);
}
