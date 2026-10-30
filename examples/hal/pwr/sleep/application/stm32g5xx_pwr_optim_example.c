/**
  ******************************************************************************
  * file           : stm32g5xx_pwr_optim_example.c
  * brief          : System configuration for STM32G5xx power consumption optimization
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

/* Private functions prototype -----------------------------------------------*/

/** brief:  System configuration for typical power consumption.
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

  /* SRAM2 is not required by this sleep example. */
  HAL_RCC_SRAM2_DisableClock();

  /* SRAM3 and DCACHE clocks are enabled after reset but unused by this example. */
#if defined(SRAM3_BASE)
  HAL_RCC_SRAM3_DisableClock();
#endif /* SRAM3_BASE */
#if defined(DCACHE)
  HAL_RCC_DCACHE_DisableClock();
#endif /* DCACHE */

  /* Disable all peripheral clocks while the core is sleeping. */
  /* Memory, accelerators and analog peripherals. */
  /* These gates affect Sleep mode only and are restored after wake-up. */
  HAL_RCC_LP_GPDMA1_DisableClockInSleepMode();
  HAL_RCC_LP_GPDMA2_DisableClockInSleepMode();
  HAL_RCC_LP_FLASH_DisableClockInSleepMode();
  HAL_RCC_LP_CRC_DisableClockInSleepMode();
  HAL_RCC_LP_CORDIC1_DisableClockInSleepMode();
  HAL_RCC_LP_FMAC1_DisableClockInSleepMode();
  HAL_RCC_LP_RAMCFG_DisableClockInSleepMode();
  HAL_RCC_LP_TZSC1_DisableClockInSleepMode();
  HAL_RCC_LP_SRAM2_DisableClockInSleepMode();
#if defined(SRAM3_BASE)
  HAL_RCC_LP_SRAM3_DisableClockInSleepMode();
#endif /* SRAM3_BASE */
  HAL_RCC_LP_ICACHE_DisableClockInSleepMode();
#if defined(DCACHE)
  HAL_RCC_LP_DCACHE_DisableClockInSleepMode();
#endif /* DCACHE */
  HAL_RCC_LP_SRAM1_DisableClockInSleepMode();
  HAL_RCC_LP_DAC1_DisableClockInSleepMode();
  HAL_RCC_LP_DAC2_DisableClockInSleepMode();
  HAL_RCC_LP_DAC5_DisableClockInSleepMode();
  HAL_RCC_LP_DAC6_DisableClockInSleepMode();
  HAL_RCC_LP_DAC7_DisableClockInSleepMode();
  HAL_RCC_LP_DAC8_DisableClockInSleepMode();
  HAL_RCC_LP_DAC9_DisableClockInSleepMode();
  HAL_RCC_LP_DAC10_DisableClockInSleepMode();
#if defined(FMAC2)
  HAL_RCC_LP_FMAC2_DisableClockInSleepMode();
#endif /* FMAC2 */
  HAL_RCC_LP_ADC12_DisableClockInSleepMode();
#if defined(ADC3)
  HAL_RCC_LP_ADC3_DisableClockInSleepMode();
#endif /* ADC3 */

  /* Security and cryptographic accelerators. */
  /* No security peripheral is used by the sleep scenario. */
  HAL_RCC_LP_DBG_DisableClockInSleepMode();
  HAL_RCC_LP_AES_DisableClockInSleepMode();
  HAL_RCC_LP_HASH_DisableClockInSleepMode();
  HAL_RCC_LP_RNG_DisableClockInSleepMode();
  HAL_RCC_LP_PKA_DisableClockInSleepMode();
  HAL_RCC_LP_SAES_DisableClockInSleepMode();
  HAL_RCC_LP_CCB_DisableClockInSleepMode();
  HAL_RCC_LP_OTFDEC1_DisableClockInSleepMode();

  /* GPIO ports. */
  /* GPIOC and GPIOG stay enabled for the PC13 wake input and PG3 LED. */
  /* All other GPIO ports can be gated while the core is sleeping. */
  HAL_RCC_LP_GPIOA_DisableClockInSleepMode();
  HAL_RCC_LP_GPIOB_DisableClockInSleepMode();
  HAL_RCC_LP_GPIOD_DisableClockInSleepMode();
  HAL_RCC_LP_GPIOE_DisableClockInSleepMode();
  HAL_RCC_LP_GPIOF_DisableClockInSleepMode();
  HAL_RCC_LP_GPIOG_DisableClockInSleepMode();
  HAL_RCC_LP_GPIOH_DisableClockInSleepMode();

  /* External memory and connectivity peripherals. */
  /* No peripheral in this group is active during the sleep interval. */
  HAL_RCC_LP_XSPI1_DisableClockInSleepMode();
  HAL_RCC_LP_TIM2_DisableClockInSleepMode();
  HAL_RCC_LP_TIM3_DisableClockInSleepMode();
  HAL_RCC_LP_TIM6_DisableClockInSleepMode();
  HAL_RCC_LP_TIM7_DisableClockInSleepMode();
  HAL_RCC_LP_TIM9_DisableClockInSleepMode();
  HAL_RCC_LP_TIM10_DisableClockInSleepMode();
  HAL_RCC_LP_TIM11_DisableClockInSleepMode();
  HAL_RCC_LP_TIM12_DisableClockInSleepMode();
  HAL_RCC_LP_TIM13_DisableClockInSleepMode();
  HAL_RCC_LP_TIM14_DisableClockInSleepMode();
  HAL_RCC_LP_WWDG_DisableClockInSleepMode();
  HAL_RCC_LP_SPI2_DisableClockInSleepMode();
  HAL_RCC_LP_SPI3_DisableClockInSleepMode();
  HAL_RCC_LP_USART2_DisableClockInSleepMode();
  HAL_RCC_LP_USART3_DisableClockInSleepMode();
  HAL_RCC_LP_UART4_DisableClockInSleepMode();
  HAL_RCC_LP_UART5_DisableClockInSleepMode();
  HAL_RCC_LP_I2C1_DisableClockInSleepMode();
  HAL_RCC_LP_I2C2_DisableClockInSleepMode();
  HAL_RCC_LP_I3C1_DisableClockInSleepMode();
  HAL_RCC_LP_CRS_DisableClockInSleepMode();
  HAL_RCC_LP_UART6_DisableClockInSleepMode();
  HAL_RCC_LP_DTS_DisableClockInSleepMode();
  HAL_RCC_LP_FDCAN_DisableClockInSleepMode();
  HAL_RCC_LP_UCPD1_DisableClockInSleepMode();

  /* Timers, serial interfaces and low-power peripherals. */
  /* The wake-up source is EXTI13, not a timer or serial peripheral. */
#if defined(HRTIM1)
  HAL_RCC_LP_HRTIM1_DisableClockInSleepMode();
#endif /* HRTIM1 */
#if defined(PLAY1)
  HAL_RCC_LP_PLAY1_DisableClockInSleepMode();
#endif /* PLAY1 */
  HAL_RCC_LP_TIM1_DisableClockInSleepMode();
  HAL_RCC_LP_SPI1_DisableClockInSleepMode();
  HAL_RCC_LP_TIM8_DisableClockInSleepMode();
  HAL_RCC_LP_USART1_DisableClockInSleepMode();
  HAL_RCC_LP_TIM15_DisableClockInSleepMode();
  HAL_RCC_LP_TIM16_DisableClockInSleepMode();
  HAL_RCC_LP_TIM17_DisableClockInSleepMode();
#if defined(USB_DRD_FS)
  HAL_RCC_LP_USB_DisableClockInSleepMode();
#endif /* USB_DRD_FS */
  HAL_RCC_LP_TIM4_DisableClockInSleepMode();
  HAL_RCC_LP_TIM5_DisableClockInSleepMode();
  HAL_RCC_LP_SBS_DisableClockInSleepMode();
#if defined(TIM20)
  HAL_RCC_LP_TIM20_DisableClockInSleepMode();
#endif /* TIM20 */
  HAL_RCC_LP_TIM26_DisableClockInSleepMode();
  HAL_RCC_LP_TIM27_DisableClockInSleepMode();
  HAL_RCC_LP_LPUART1_DisableClockInSleepMode();
#if defined(I2C3)
  HAL_RCC_LP_I2C3_DisableClockInSleepMode();
#endif /* I2C3 */
#if defined(I2C4)
  HAL_RCC_LP_I2C4_DisableClockInSleepMode();
#endif /* I2C4 */
  HAL_RCC_LP_LPTIM1_DisableClockInSleepMode();
#if defined(POSENC1)
  HAL_RCC_LP_POSENC1_DisableClockInSleepMode();
#endif /* POSENC1 */
#if defined(POSENC2)
  HAL_RCC_LP_POSENC2_DisableClockInSleepMode();
#endif /* POSENC2 */
#if defined(OPAMP1)
  HAL_RCC_LP_OPAMP1_DisableClockInSleepMode();
#endif /* OPAMP1 */
#if defined(OPAMP2)
  HAL_RCC_LP_OPAMP2_DisableClockInSleepMode();
#endif /* OPAMP2 */
#if defined(OPAMP3)
  HAL_RCC_LP_OPAMP3_DisableClockInSleepMode();
#endif /* OPAMP3 */
  HAL_RCC_LP_VREFBUF_DisableClockInSleepMode();
  HAL_RCC_LP_RTCAPB_DisableClockInSleepMode();

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
