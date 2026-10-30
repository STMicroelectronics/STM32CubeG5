
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
/* Exported functions for RCC in LL layer */
/******************************************************************************/

/**
  * Configure the system core clock only and activate it using the LL RCC unitary APIs (footprint optimization)
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
  LL_PWR_SetRegulatorVoltageScaling(LL_PWR_REGU_VOLT_SCALE_0);

  /* Poll on VOS ready and Active VOS ready flag to reach correct voltage */
  while((LL_PWR_IsActiveFlag_VOS() == 0U) || (LL_PWR_IsActiveFlag_ACTVOS() == 0U));

  /* LL_RCC_HSI_SetDivider(LL_RCC_HSI_DIV_1); */ /* Configuration matches register reset state at startup. */
  /* LL_RCC_HSI_Enable();
  do
  {
  }
  while (LL_RCC_HSI_IsReady() != 1U);
  */

  LL_RCC_PLL1_ConfigDomain_SYS(LL_RCC_PLL1SOURCE_HSI, 32U, 280U, 2U);

  LL_RCC_PLL1_SetQ(3U);
  /* LL_RCC_PLL1_SetR(1U); */ /* Configuration matches register reset state at startup. */
  LL_RCC_PLL1_SetInputRange(LL_RCC_PLLINPUTRANGE_2_4);
  /* LL_RCC_PLL1_SetVCORange(LL_RCC_PLLVCORANGE_128_560); */ /* Configuration matches register reset state at startup. */

  LL_RCC_PLL1_Enable();
  /* Infinite loop */
  do
  {
  }
  while (LL_RCC_PLL1_IsReady() != 1U);

  LL_RCC_PLL1_EnableOutputP();

  /* Frequency will be increased */
  LL_FLASH_SetLatency(FLASH, LL_FLASH_LATENCY_6WS);
  if (LL_FLASH_GetLatency(FLASH) != LL_FLASH_LATENCY_6WS)
  {
    return SYSTEM_CLOCK_ERROR;
  }
  LL_FLASH_SetProgrammingDelay(FLASH, LL_FLASH_PROGRAM_DELAY_3);

  /** Initializes the CPU, AHB and APB busses clocks */
  LL_RCC_ConfigBusClock(LL_RCC_HCLK_PRESCALER_1 | LL_RCC_APB1_PRESCALER_2 |
                        LL_RCC_APB2_PRESCALER_2 | LL_RCC_APB3_PRESCALER_2);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);
  /* Wait till System clock is ready */
  /* Infinite loop */
  do
  {
  }
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1);

  LL_SetSystemCoreClock(280000000U);
  LL_Init1msTick(SystemCoreClock);

  /* No GPIO configuration required for RCC */

  return SYSTEM_OK;
}

void mx_rcc_deinit(void)
{
  LL_RCC_WRITE_REG(IER, RCC_IER_Rst); /* Disable all interrupts */

  /* Reset System clock */
  LL_RCC_HSI_SetDivider(LL_RCC_HSI_DIV_1);
  LL_RCC_HSI_Enable();
  /* Infinite loop */
  do
  {
  }
  while (LL_RCC_HSI_IsReady() != 1U);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
  /* Wait till System clock is ready */
  /* Infinite loop */
  do
  {
  }
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI);

  /* Reset all bus dividers */
  LL_RCC_ConfigBusClock((uint32_t)LL_RCC_HCLK_PRESCALER_1 | (uint32_t)LL_RCC_APB1_PRESCALER_1 |
                        (uint32_t)LL_RCC_APB2_PRESCALER_1 |
                        (uint32_t)LL_RCC_APB3_PRESCALER_1);

  SystemCoreClock = HSI_VALUE;

  LL_RCC_SetSystickClockSource(LL_RCC_SYSTICK_CLKSOURCE_HCLKDIV8);

  /* Adapt Systick interrupt period */

  /* Reset all remaining oscillators not in backup domain */
  LL_RCC_WRITE_REG(CR, RCC_CR_Rst);
  LL_RCC_WRITE_REG(CR, RCC_CR_Rst); /* Called a second time to ensure HSE EXT and BYP disabled only when HSE has been disabled */

  LL_RCC_LSI_Disable();

  /* Reset MCO, wake up system clock (not RTC prescaler) */
  uint32_t read_value = LL_RCC_READ_REG(CFGR1);
  LL_RCC_WRITE_REG(CFGR1, (read_value & RCC_CFGR1_RTCPRE) | (RCC_CFGR1_Rst & ~(RCC_CFGR1_RTCPRE)));

  /* Reset PLLs */
  LL_RCC_WRITE_REG(PLL1CFGR, RCC_PLL1CFGR_Rst);
  LL_RCC_WRITE_REG(PLL1DIVR, RCC_PLL1DIVR_Rst);
  LL_RCC_WRITE_REG(PLL1FRACR, RCC_PLL1FRACR_Rst);

  /* Clear RCC flags */
  LL_RCC_ClearFlag(LL_RCC_IT_LSIRDY | LL_RCC_IT_LSERDY | LL_RCC_IT_HSIRDY | LL_RCC_IT_HSERDY | LL_RCC_IT_HSI48RDY | \
                   LL_RCC_IT_PLL1RDY | LL_RCC_IT_PLL2RDY | LL_RCC_IT_LSECSS | LL_RCC_IT_HSECSS);

  LL_RCC_ClearResetFlags();
}

/**
  * configures and activate the clocks used by all the peripherals selected within the project
  */
system_status_t mx_rcc_peripherals_clock_config(void)
{
  /* Resources using PCLK2 (140 MHz):
    SPI1
  */

  return SYSTEM_OK;
}
