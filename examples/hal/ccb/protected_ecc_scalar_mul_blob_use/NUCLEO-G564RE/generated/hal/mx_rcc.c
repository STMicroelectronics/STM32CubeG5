
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

typedef struct
{
  uint32_t hsi: 4U;
  uint32_t hsi48: 4U;
  uint32_t pll1: 1U;
  uint32_t pll1p: 2U; /* clk src of up to 2 peripherals + sys clk */
  uint32_t pll1q: 4U; /* clk src of up to 15 peripherals */
  uint32_t pll1r: 5U; /* clk src of up to 18 peripherals */
  uint32_t pll2: 1U;
  uint32_t pll2p: 4U; /* clk src of up to 8 peripherals */
  uint32_t pll2q: 5U; /* clk src of up to 20 peripherals */
  uint32_t pll2r: 3U; /* clk src of up to 4 peripherals */
} mx_rcc_status_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define RCC_CONSUME_RESOURCE(block, nb_resources)     (RCC_Status.block += (nb_resources))
#define RCC_RELEASE_RESOURCE(block)                   ((RCC_Status.block > 0U) ? (RCC_Status.block--) : \
                                                       (RCC_Status.block = 0U))
#define RCC_IS_RESOURCE_FREE(block)                   (RCC_Status.block == 0U)
#define RCC_IS_RESOURCE_TAKEN(block)                  (RCC_Status.block > 0U)

/* Private variables ---------------------------------------------------------*/
static mx_rcc_status_t RCC_Status;

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

  RCC_CONSUME_RESOURCE(hsi, 1);

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
  RCC_CONSUME_RESOURCE(pll1, 1);

  if (HAL_RCC_PLL1_EnableOutput(HAL_RCC_PLL1_OUTPUT_P) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  RCC_CONSUME_RESOURCE(pll1p, 1);

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
  RCC_Status = (mx_rcc_status_t){0};

  HAL_RCC_Reset();
}

/**
  * configure and enable clock for USART2
  */
system_status_t mx_rcc_usart2_clock_config(void)
{
  /* Resources using PCLK1 (140 MHz):
    USART2
  */

  if (HAL_RCC_USART2_SetKernelClkSource(HAL_RCC_USART2_CLK_SRC_PCLK1) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  return SYSTEM_OK;
}

/**
  * Deactivate clock for USART2
  */
system_status_t mx_rcc_usart2_clock_deactivate(void)
{
  /* Nothing to do */

  return SYSTEM_OK;
}

/**
  * configure and enable clock for CCB
  */
system_status_t mx_rcc_ccb_clock_config(void)
{
  /* Resources using HSI48 (48 MHz):
    CCB
  */
  if (mx_rcc_hsi48_clock_config(1) != SYSTEM_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  return SYSTEM_OK;
}

/**
  * Deactivate clock for CCB
  */
system_status_t mx_rcc_ccb_clock_deactivate(void)
{
  if (mx_rcc_hsi48_clock_deactivate() != SYSTEM_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }
  return SYSTEM_OK;
}

/**
  * configure and enable HSI oscillator
  * @param nb_resources Number of resources which requests to configure the clock
  */
system_status_t mx_rcc_hsi_clock_config(uint32_t nb_resources)
{
  if (RCC_IS_RESOURCE_FREE(hsi))
  {
    if (HAL_RCC_HSI_Enable(HAL_RCC_HSI_DIV1) != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }
  RCC_CONSUME_RESOURCE(hsi, nb_resources);

  return SYSTEM_OK;
}

/**
  * Disable HSI oscillator
  */
system_status_t mx_rcc_hsi_clock_deactivate(void)
{
  RCC_RELEASE_RESOURCE(hsi);

  if (RCC_IS_RESOURCE_FREE(hsi))
  {
    if (HAL_RCC_HSI_Disable() != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }
  return SYSTEM_OK;
}

/**
  * configure and enable HSI48 oscillator
  * @param nb_resources Number of resources which requests to configure the clock
  */
system_status_t mx_rcc_hsi48_clock_config(uint32_t nb_resources)
{
  if (RCC_IS_RESOURCE_FREE(hsi48))
  {
    if (HAL_RCC_HSI48_Enable() != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }
  RCC_CONSUME_RESOURCE(hsi48, nb_resources);

  return SYSTEM_OK;
}

/**
  * Disable HSI48 oscillator
  */
system_status_t mx_rcc_hsi48_clock_deactivate(void)
{
  RCC_RELEASE_RESOURCE(hsi48);

  if (RCC_IS_RESOURCE_FREE(hsi48))
  {
    if (HAL_RCC_HSI48_Disable() != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }
  return SYSTEM_OK;
}

/**
  * Configure PLL1 (all factors , , , , ) and activate it.
  * Only PLL1 is enabled, not the PLL1 outputs.
  */
system_status_t mx_rcc_pll1_config(void)
{
  if (RCC_IS_RESOURCE_FREE(pll1))
  {
    /* Enable the oscillator used as PLL1 input */
    if (mx_rcc_hsi_clock_config(1) != SYSTEM_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }

    /* Configure and enable PLL1 */

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

    RCC_CONSUME_RESOURCE(pll1, 1);
  }

  return SYSTEM_OK;
}

/**
  * Deactivate PLL1
  */
system_status_t mx_rcc_pll1_deactivate(void)
{
  /* Deactivation is possible only if the outputs are free */
  if (RCC_IS_RESOURCE_FREE(pll1p)
      && RCC_IS_RESOURCE_FREE(pll1q)
      && RCC_IS_RESOURCE_FREE(pll1r))
  {
    RCC_RELEASE_RESOURCE(pll1);
    if (HAL_RCC_PLL1_Disable() != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }

    /* Deactivate the oscillator used as input of the PLL1 */
    if (mx_rcc_hsi_clock_deactivate() != SYSTEM_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }

  return SYSTEM_OK;
}
/**
  * Activate PLL1P output
  * @param nb_resources Number of resources which requests to configure the output
  */
system_status_t mx_rcc_pll1_output_p_config(uint32_t nb_resources)
{
  if (RCC_IS_RESOURCE_FREE(pll1p))
  {
    if (HAL_RCC_PLL1_EnableOutput(HAL_RCC_PLL1_OUTPUT_P) != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }
  RCC_CONSUME_RESOURCE(pll1p, nb_resources);

  return SYSTEM_OK;
}

/**
  * Deactivate PLL1P output
  */
system_status_t mx_rcc_pll1_output_p_deactivate(void)
{
  RCC_RELEASE_RESOURCE(pll1p);
  if (RCC_IS_RESOURCE_FREE(pll1p))
  {
    if (HAL_RCC_PLL1_DisableOutput(HAL_RCC_PLL1_OUTPUT_P) != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }
  return SYSTEM_OK;
}

/**
  * Activate PLL1Q output
  * @param nb_resources Number of resources which requests to configure the output
  */
system_status_t mx_rcc_pll1_output_q_config(uint32_t nb_resources)
{
  if (RCC_IS_RESOURCE_FREE(pll1q))
  {
    if (HAL_RCC_PLL1_EnableOutput(HAL_RCC_PLL1_OUTPUT_Q) != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }
  RCC_CONSUME_RESOURCE(pll1q, nb_resources);

  return SYSTEM_OK;
}

/**
  * Deactivate PLL1Q output
  */
system_status_t mx_rcc_pll1_output_q_deactivate(void)
{
  RCC_RELEASE_RESOURCE(pll1q);
  if (RCC_IS_RESOURCE_FREE(pll1q))
  {
    if (HAL_RCC_PLL1_DisableOutput(HAL_RCC_PLL1_OUTPUT_Q) != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }
  return SYSTEM_OK;
}

/**
  * Activate PLL1R output
  * @param nb_resources Number of resources which requests to configure the output
  */
system_status_t mx_rcc_pll1_output_r_config(uint32_t nb_resources)
{
  if (RCC_IS_RESOURCE_FREE(pll1r))
  {
    if (HAL_RCC_PLL1_EnableOutput(HAL_RCC_PLL1_OUTPUT_R) != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }
  RCC_CONSUME_RESOURCE(pll1r, nb_resources);

  return SYSTEM_OK;
}

/**
  * Deactivate PLL1R output
  */
system_status_t mx_rcc_pll1_output_r_deactivate(void)
{
  RCC_RELEASE_RESOURCE(pll1r);
  if (RCC_IS_RESOURCE_FREE(pll1r))
  {
    if (HAL_RCC_PLL1_DisableOutput(HAL_RCC_PLL1_OUTPUT_R) != HAL_OK)
    {
      return SYSTEM_CLOCK_ERROR;
    }
  }
  return SYSTEM_OK;
}
