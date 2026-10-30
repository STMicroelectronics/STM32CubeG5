
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
  *            System Clock source            = HSI
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 2
  *            APB3 Prescaler                 = 2
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

  if (HAL_RCC_SetSYSCLKSource(HAL_RCC_SYSCLK_SRC_HSI) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  /* Frequency has been decreased */
  if (HAL_FLASH_ITF_SetLatency(HAL_FLASH, HAL_FLASH_ITF_LATENCY_6) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }
  if (HAL_FLASH_ITF_SetProgrammingDelay(HAL_FLASH, HAL_FLASH_ITF_PROGRAM_DELAY_3) != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  if (HAL_UpdateCoreClock() != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  /** Configure MCO (clock source, IO and divider) */

  /* ### RCC GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOB_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name]

       PB9     ------>   RCC_MCO1
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_0;
  HAL_GPIO_Init(HAL_GPIOB, HAL_GPIO_PIN_9, &gpio_config);

  HAL_RCC_SetConfigMCO(HAL_RCC_MCO1_SRC_HSI, HAL_RCC_MCO1_PRESCALER15);

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
  /* Resources using PCLK1 (32 MHz):
    USART2
  */

  /* Resources using HSI48 (48 MHz):
    CRS
  */
  if (HAL_RCC_HSI48_Enable() != HAL_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  /* Resources using LSE (32.768 kHz):
    CRS
  */
  /* Disable RTC Domain Write Protection */
  HAL_PWR_DisableRTCDomainWriteProtection();

  if (HAL_RCC_LSE_Enable(HAL_RCC_LSE_ON, HAL_RCC_LSE_DRIVE_MEDIUMHIGH) != HAL_OK)
  {
    /* In order to simplify the code generation and management for the user, the write protection is not enabled by
       default. In real case application, we advise to enable it once all the necessary configurations are done. */
    /* Enable RTC Domain Write Protection */
    /* HAL_PWR_EnableRTCDomainWriteProtection(); */

    return SYSTEM_CLOCK_ERROR;
  }
  /* In order to simplify the code generation and management for the user, the write protection is not enabled by
     default. In real case application, we advise to enable it once all the necessary configurations are done. */
  /* Enable RTC Domain Write Protection */
  /* HAL_PWR_EnableRTCDomainWriteProtection(); */

  return SYSTEM_OK;
}
