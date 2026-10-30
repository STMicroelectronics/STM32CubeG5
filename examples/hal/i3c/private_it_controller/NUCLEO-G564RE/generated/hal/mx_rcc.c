
/**
  ******************************************************************************
  * @file           : mx_rcc.c
  * @brief          : STM32 RCC program body
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
#include "mx_rcc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/******************************************************************************/
/* Exported functions for RCC in HAL layer */
/******************************************************************************/

/**
  * Configure the system core clock only and activate it using the HAL RCC unitary APIs (footprint optimization)
  *         The system Clock is configured as follow :
  *            System Clock source            = PLLCLK
  *            SYSCLK(Hz)                     = 280000000
  *            HCLK(Hz)                       = 280000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 2
  *            APB3 Prescaler                 = 2
  *            PLL1M                          = 32
  *            PLL1N                          = 280
  *            PLL1P                          = 2
  *            PLL1Q                          = 3
  *            PLL1R                          = 1
  *            PLL1FracN                      = 0
  *            Flash Latency(WS)              = 6
  */
system_status_t mx_rcc_init(void)
{
  /* Configure the Voltage scaling level */
  if (HAL_PWR_SetMainRegulatorVoltageScalingRange(HAL_PWR_VOLT_SCALE_0) != HAL_OK)
  {
    return SYSTEM_POWER_ERROR;
  }

  if (HAL_RCC_HSI_Enable(HAL_RCC_HSI_DIV1) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  hal_rcc_pll_config_t config_pll1;
  config_pll1.pll_source      = HAL_RCC_PLL_SRC_HSI;
  config_pll1.pll_m           = 32U;
  config_pll1.pll_n           = 280U;
  config_pll1.pll_p           = 2U;
  config_pll1.pll_q           = 3U;
  config_pll1.pll_r           = 1U;
  config_pll1.pll_fracn       = 0;
  config_pll1.pll_input_range = HAL_RCC_PLL_INPUT_RANGE_2_4;
  config_pll1.pll_vco_range   = HAL_RCC_PLL_VCO_RANGE_128_560;
  if (HAL_RCC_PLL1_SetConfig(&config_pll1) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  if (HAL_RCC_PLL1_Enable() != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  if (HAL_RCC_PLL1_EnableOutput(HAL_RCC_PLL1_OUTPUT_P) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  /* Frequency will be increased */
  if (HAL_FLASH_ITF_SetLatency(HAL_FLASH, HAL_FLASH_ITF_LATENCY_6) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }
  if (HAL_FLASH_ITF_SetProgrammingDelay(HAL_FLASH, HAL_FLASH_ITF_PROGRAM_DELAY_3) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  /** Initializes the CPU, AHB and APB busses clocks */
  hal_rcc_bus_clk_config_t config_bus;
  config_bus.hclk_prescaler  = HAL_RCC_HCLK_PRESCALER1;
  config_bus.pclk1_prescaler = HAL_RCC_PCLK_PRESCALER2;
  config_bus.pclk2_prescaler = HAL_RCC_PCLK_PRESCALER2;
  config_bus.pclk3_prescaler = HAL_RCC_PCLK_PRESCALER2;
  if (HAL_RCC_SetBusClockConfig(&config_bus) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  if (HAL_RCC_SetSYSCLKSource(HAL_RCC_SYSCLK_SRC_PLLCLK) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  if (HAL_UpdateCoreClock() != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  /* No GPIO configuration required for RCC */

  return SYSTEM_OK;
}

void mx_rcc_deinit(void)
{
  HAL_RCC_Reset();
}

/**
  * configures and activate the clocks used by all the peripherals selected within the project
  */
system_status_t mx_rcc_peripherals_clock_config(void)
{
  /* Resources using PCLK1 (140 MHz):
    USART2
    I3C1
  */

  return SYSTEM_OK;
}
