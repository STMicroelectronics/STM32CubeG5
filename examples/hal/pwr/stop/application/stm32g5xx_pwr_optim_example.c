/**
  ******************************************************************************
  * File            : stm32g5xx_pwr_optim_example.c
  * Brief           : System configuration for STM32G5xx Stop mode.
  ******************************************************************************
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "pwr_optim_example.h"

/* Exported functions ------------------------------------------------------- */

/**
  * brief:  Configure the system clock and power baseline used by the example.
  * retval: system status
  */
system_status_t PWR_SystemOptimization(void)
{
  if (mx_rccx_init() != SYSTEM_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  if (mx_pwrx_init() != SYSTEM_OK)
  {
    return SYSTEM_POWER_ERROR;
  }

  /* Disable HSI in Stop mode */
  HAL_RCC_HSI_DisableInStopMode();

  /* SRAM1 is not required by this Stop example. */
  HAL_RCC_SRAM1_DisableClock();

  /* SRAM3 and DCACHE clocks are enabled after reset but unused by this example. */
#if defined(SRAM3_BASE)
  HAL_RCC_SRAM3_DisableClock();
#endif /* SRAM3_BASE */
#if defined(DCACHE)
  HAL_RCC_DCACHE_DisableClock();
#endif /* DCACHE */

  /* Disable all peripheral clocks while the core is Stopping. */
  /* Memory, accelerators and analog peripherals. */
  /* These gates affect Stop mode only and are restored after wake-up. */
  HAL_RCC_LP_SRAM1_DisableClockInStopMode();
  HAL_RCC_LP_SRAM2_DisableClockInStopMode();
#if defined(SRAM3_BASE)
  HAL_RCC_LP_SRAM3_DisableClockInStopMode();
#endif /* SRAM3_BASE */
  HAL_RCC_LP_DAC1_DisableClockInStopMode();
  HAL_RCC_LP_DAC2_DisableClockInStopMode();
  HAL_RCC_LP_DAC5_DisableClockInStopMode();
  HAL_RCC_LP_DAC6_DisableClockInStopMode();
  HAL_RCC_LP_DAC7_DisableClockInStopMode();
  HAL_RCC_LP_DAC8_DisableClockInStopMode();
  HAL_RCC_LP_DAC9_DisableClockInStopMode();
  HAL_RCC_LP_DAC10_DisableClockInStopMode();

  /* External memory and connectivity peripherals. */
  /* No peripheral in this group is active during the Stop interval. */
  HAL_RCC_LP_SPI2_DisableClockInStopMode();
  HAL_RCC_LP_SPI3_DisableClockInStopMode();
  HAL_RCC_LP_USART2_DisableClockInStopMode();
  HAL_RCC_LP_USART3_DisableClockInStopMode();
  HAL_RCC_LP_UART4_DisableClockInStopMode();
  HAL_RCC_LP_UART5_DisableClockInStopMode();
  HAL_RCC_LP_I2C1_DisableClockInStopMode();
  HAL_RCC_LP_I2C2_DisableClockInStopMode();
  HAL_RCC_LP_I3C1_DisableClockInStopMode();
  HAL_RCC_LP_UART6_DisableClockInStopMode();
  HAL_RCC_LP_DTS_DisableClockInStopMode();
  HAL_RCC_LP_UCPD1_DisableClockInStopMode();

  /* Timers, serial interfaces and low-power peripherals. */
  /* The wake-up source is EXTI13, not a timer or serial peripheral. */
#if defined(PLAY1)
  HAL_RCC_LP_PLAY1_DisableClockInStopMode();
#endif /* PLAY1 */
#if defined(USB_DRD_FS)
  HAL_RCC_LP_USB_DisableClockInStopMode();
#endif /* USB_DRD_FS */
  HAL_RCC_LP_LPUART1_DisableClockInStopMode();
#if defined(I2C3)
  HAL_RCC_LP_I2C3_DisableClockInStopMode();
#endif /* I2C3 */
#if defined(I2C4)
  HAL_RCC_LP_I2C4_DisableClockInStopMode();
#endif /* I2C4 */
  HAL_RCC_LP_LPTIM1_DisableClockInStopMode();
#if defined(OPAMP1)
  HAL_RCC_LP_OPAMP1_DisableClockInStopMode();
#endif /* OPAMP1 */
#if defined(OPAMP2)
  HAL_RCC_LP_OPAMP2_DisableClockInStopMode();
#endif /* OPAMP2 */
#if defined(OPAMP3)
  HAL_RCC_LP_OPAMP3_DisableClockInStopMode();
#endif /* OPAMP3 */
  HAL_RCC_LP_VREFBUF_DisableClockInStopMode();
  HAL_RCC_LP_RTCAPB_DisableClockInStopMode();

  /* Select kernel clocks for peripherals that remain configured after wakeup. */
  /* USART, UART and LPUART kernel clocks. */
  /* PLL2Q provides the selected serial kernel clock source. */
  HAL_RCC_USART1_SetKernelClkSource(HAL_RCC_USART1_CLK_SRC_PLL2Q);
  HAL_RCC_USART2_SetKernelClkSource(HAL_RCC_USART2_CLK_SRC_PLL2Q);
#if defined(USART3)
  HAL_RCC_USART3_SetKernelClkSource(HAL_RCC_USART3_CLK_SRC_PLL2Q);
#endif /* USART3 */
  HAL_RCC_UART4_SetKernelClkSource(HAL_RCC_UART4_CLK_SRC_PLL2Q);
  HAL_RCC_UART5_SetKernelClkSource(HAL_RCC_UART5_CLK_SRC_PLL2Q);
  HAL_RCC_UART6_SetKernelClkSource(HAL_RCC_UART6_CLK_SRC_PLL2Q);
  HAL_RCC_LPUART1_SetKernelClkSource(HAL_RCC_LPUART1_CLK_SRC_PLL2Q);


  HAL_RCC_DAC1_SetKernelClkSource(HAL_RCC_DAC1_CLK_SRC_PLL2Q);
  HAL_RCC_DAC2_SetKernelClkSource(HAL_RCC_DAC2_CLK_SRC_PLL2Q);
  HAL_RCC_DAC5_SetKernelClkSource(HAL_RCC_DAC5_CLK_SRC_PLL2Q);
  HAL_RCC_DAC6_SetKernelClkSource(HAL_RCC_DAC6_CLK_SRC_PLL2Q);
  HAL_RCC_DAC7_SetKernelClkSource(HAL_RCC_DAC7_CLK_SRC_PLL2Q);
  HAL_RCC_DAC8_SetKernelClkSource(HAL_RCC_DAC8_CLK_SRC_PLL2Q);
  HAL_RCC_DAC9_SetKernelClkSource(HAL_RCC_DAC9_CLK_SRC_PLL2Q);
  HAL_RCC_DAC10_SetKernelClkSource(HAL_RCC_DAC10_CLK_SRC_PLL2Q);

  /* SPI, PLAY1 and LPTIM kernel clocks. */
  /* PLL2P provides the selected synchronous peripheral clock source. */
  HAL_RCC_SPI1_SetKernelClkSource(HAL_RCC_SPI1_CLK_SRC_PLL2P);
  HAL_RCC_SPI2_SetKernelClkSource(HAL_RCC_SPI2_CLK_SRC_PLL2P);
  HAL_RCC_SPI3_SetKernelClkSource(HAL_RCC_SPI3_CLK_SRC_PLL2P);
  HAL_RCC_PLAY1_SetKernelClkSource(HAL_RCC_PLAY1_CLK_SRC_PLL2P);
  HAL_RCC_LPTIM1_SetKernelClkSource(HAL_RCC_LPTIM1_CLK_SRC_PLL2P);

  /* Memory, power-management and interconnect kernel clocks. */
  /* These selections match the board clock plan generated for the example. */
  HAL_RCC_XSPI1_SetKernelClkSource(HAL_RCC_XSPI1_CLK_SRC_HSE);
  HAL_RCC_UCPD1_SetKernelClkSource(HAL_RCC_UCPD1_CLK_SRC_PLL2Q);
  HAL_RCC_USB_SetKernelClkSource(HAL_RCC_USB_CLK_SRC_PLL2Q);
  HAL_RCC_I3C1_SetKernelClkSource(HAL_RCC_I3C1_CLK_SRC_HSE);
  HAL_RCC_I2C1_SetKernelClkSource(HAL_RCC_I2C1_CLK_SRC_HSE);
  HAL_RCC_I2C2_SetKernelClkSource(HAL_RCC_I2C2_CLK_SRC_HSE);
  HAL_RCC_I2C3_SetKernelClkSource(HAL_RCC_I2C3_CLK_SRC_HSE);
  HAL_RCC_I2C4_SetKernelClkSource(HAL_RCC_I2C4_CLK_SRC_HSE);

  /* ADC, FDCAN and position encoder kernel clocks. */
  /* Configure kernel sources before isolating the RCC peripheral buses. */
  HAL_RCC_ADC_SetKernelClkSource(HAL_RCC_ADC_CLK_SRC_PLL2P);
  HAL_RCC_FDCAN_SetKernelClkSource(HAL_RCC_FDCAN_CLK_SRC_PLL2Q);

#if defined(POSENC1)
  HAL_RCC_POSENC1_SetKernelClkSource(HAL_RCC_POSENC1_CLK_SRC_PLL2P);
#endif /* POSENC1 */
#if defined(POSENC2)
  HAL_RCC_POSENC2_SetKernelClkSource(HAL_RCC_POSENC2_CLK_SRC_PLL2P);
#endif /* POSENC2 */

  /* Isolate unused RCC peripheral buses after configuration. */
  HAL_RCC_AHB1_DisableBusClock();
  HAL_RCC_AHB2_DisableBusClock();
  HAL_RCC_APB1_DisableBusClock();
  HAL_RCC_APB2_DisableBusClock();
  HAL_RCC_APB3_DisableBusClock();

  return SYSTEM_OK;
}
