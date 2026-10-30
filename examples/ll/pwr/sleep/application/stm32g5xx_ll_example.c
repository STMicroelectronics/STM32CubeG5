/**
  ******************************************************************************
  * file           : stm32g5xx_ll_example.c
  * brief          : Enter and exit the Sleep mode through an EXTI interrupt using the LL API
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
#include "ll_example.h"

/* Private functions prototype -----------------------------------------------*/
void mx_extix_irqhandler(void);

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
  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_SRAM2);

  /* SRAM3 and DCACHE clocks are enabled after reset but unused by this example. */
#if defined(SRAM3_BASE)
  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_SRAM3);
#endif /* SRAM3_BASE */
#if defined(DCACHE)
  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_DCACHE);
#endif /* DCACHE */

  /* Disable all peripheral clocks while the core is sleeping. */
  /* Memory, accelerators and analog peripherals. */
  /* These gates affect Sleep mode only and are restored after wake-up. */
  LL_AHB1_GRP1_DisableClockLowPower(LL_AHB1_GRP1_PERIPH_GPDMA1);
  LL_AHB1_GRP1_DisableClockLowPower(LL_AHB1_GRP1_PERIPH_GPDMA2);
  LL_AHB1_GRP1_DisableClockLowPower(LL_AHB1_GRP1_PERIPH_FLASH);
  LL_AHB1_GRP1_DisableClockLowPower(LL_AHB1_GRP1_PERIPH_CRC);
  LL_AHB1_GRP1_DisableClockLowPower(LL_AHB1_GRP1_PERIPH_CORDIC1);
  LL_AHB1_GRP1_DisableClockLowPower(LL_AHB1_GRP1_PERIPH_FMAC1);
  LL_AHB1_GRP1_DisableClockLowPower(LL_AHB1_GRP1_PERIPH_RAMCFG);
  LL_AHB1_GRP1_DisableClockLowPower(LL_AHB1_GRP1_PERIPH_TZSC1);
  LL_AHB1_GRP1_DisableClockLowPower(LL_AHB1_GRP1_PERIPH_SRAM2);
#if defined(SRAM3_BASE)
  LL_AHB1_GRP1_DisableClockLowPower(LL_AHB1_GRP1_PERIPH_SRAM3);
#endif /* SRAM3_BASE */
  LL_AHB1_GRP1_DisableClockLowPower(LL_AHB1_GRP1_PERIPH_ICACHE);
#if defined(DCACHE)
  LL_AHB1_GRP1_DisableClockLowPower(LL_AHB1_GRP1_PERIPH_DCACHE);
#endif /* DCACHE */
  LL_AHB1_GRP1_DisableClockLowPower(LL_AHB1_GRP1_PERIPH_SRAM1);
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_DAC1);
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_DAC2);
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_DAC5);
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_DAC6);
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_DAC7);
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_DAC8);
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_DAC9);
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_DAC10);
#if defined(FMAC2)
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_FMAC2);
#endif /* FMAC2 */
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_ADC12);
#if defined(ADC3)
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_ADC3);
#endif /* ADC3 */

  /* Security and cryptographic accelerators. */
  /* No security peripheral is used by the sleep scenario. */
  LL_AHB3_GRP1_DisableClockLowPower(LL_AHB3_GRP1_PERIPH_DBG);
  LL_AHB4_GRP1_DisableClockLowPower(LL_AHB4_GRP1_PERIPH_AES);
  LL_AHB4_GRP1_DisableClockLowPower(LL_AHB4_GRP1_PERIPH_HASH);
  LL_AHB4_GRP1_DisableClockLowPower(LL_AHB4_GRP1_PERIPH_RNG);
  LL_AHB4_GRP1_DisableClockLowPower(LL_AHB4_GRP1_PERIPH_PKA);
  LL_AHB4_GRP1_DisableClockLowPower(LL_AHB4_GRP1_PERIPH_SAES);
  LL_AHB4_GRP1_DisableClockLowPower(LL_AHB4_GRP1_PERIPH_CCB);
  LL_AHB4_GRP1_DisableClockLowPower(LL_AHB4_GRP1_PERIPH_OTFDEC1);

  /* GPIO ports. */
  /* GPIOC and GPIOG stay enabled for the PC13 wake input and PG3 LED. */
  /* All other GPIO ports can be gated while the core is sleeping. */
  LL_AHB4_GRP1_DisableClockLowPower(LL_AHB4_GRP1_PERIPH_GPIOA);
  LL_AHB4_GRP1_DisableClockLowPower(LL_AHB4_GRP1_PERIPH_GPIOB);
  LL_AHB4_GRP1_DisableClockLowPower(LL_AHB4_GRP1_PERIPH_GPIOD);
  LL_AHB4_GRP1_DisableClockLowPower(LL_AHB4_GRP1_PERIPH_GPIOE);
  LL_AHB4_GRP1_DisableClockLowPower(LL_AHB4_GRP1_PERIPH_GPIOF);
  LL_AHB4_GRP1_DisableClockLowPower(LL_AHB4_GRP1_PERIPH_GPIOG);
  LL_AHB4_GRP1_DisableClockLowPower(LL_AHB4_GRP1_PERIPH_GPIOH);

  /* External memory and connectivity peripherals. */
  /* No peripheral in this group is active during the sleep interval. */
  LL_AHB4_GRP1_DisableClockLowPower(LL_AHB4_GRP1_PERIPH_XSPI1);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_TIM2);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_TIM3);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_TIM6);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_TIM7);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_TIM9);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_TIM10);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_TIM11);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_TIM12);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_TIM13);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_TIM14);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_WWDG);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_SPI2);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_SPI3);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_USART2);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_USART3);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_UART4);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_UART5);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_I2C1);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_I2C2);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_I3C1);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_CRS);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_UART6);
  LL_APB1_GRP2_DisableClockLowPower(LL_APB1_GRP2_PERIPH_DTS);
  LL_APB1_GRP2_DisableClockLowPower(LL_APB1_GRP2_PERIPH_FDCAN);
  LL_APB1_GRP2_DisableClockLowPower(LL_APB1_GRP2_PERIPH_UCPD1);

  /* Timers, serial interfaces and low-power peripherals. */
  /* The wake-up source is EXTI13, not a timer or serial peripheral. */
#if defined(HRTIM1)
  LL_APB2_GRP1_DisableClockLowPower(LL_APB2_GRP1_PERIPH_HRTIM1);
#endif /* HRTIM1 */
#if defined(PLAY1)
  LL_APB2_GRP1_DisableClockLowPower(LL_APB2_GRP1_PERIPH_PLAY1);
#endif /* PLAY1 */
  LL_APB2_GRP1_DisableClockLowPower(LL_APB2_GRP1_PERIPH_TIM1);
  LL_APB2_GRP1_DisableClockLowPower(LL_APB2_GRP1_PERIPH_SPI1);
  LL_APB2_GRP1_DisableClockLowPower(LL_APB2_GRP1_PERIPH_TIM8);
  LL_APB2_GRP1_DisableClockLowPower(LL_APB2_GRP1_PERIPH_USART1);
  LL_APB2_GRP1_DisableClockLowPower(LL_APB2_GRP1_PERIPH_TIM15);
  LL_APB2_GRP1_DisableClockLowPower(LL_APB2_GRP1_PERIPH_TIM16);
  LL_APB2_GRP1_DisableClockLowPower(LL_APB2_GRP1_PERIPH_TIM17);
#if defined(USB_DRD_FS)
  LL_APB2_GRP1_DisableClockLowPower(LL_APB2_GRP1_PERIPH_USB);
#endif /* USB_DRD_FS */
  LL_APB2_GRP1_DisableClockLowPower(LL_APB2_GRP1_PERIPH_TIM4);
  LL_APB2_GRP1_DisableClockLowPower(LL_APB2_GRP1_PERIPH_TIM5);
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_SBS);
#if defined(TIM20)
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_TIM20);
#endif /* TIM20 */
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_TIM26);
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_TIM27);
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_LPUART1);
#if defined(I2C3)
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_I2C3);
#endif /* I2C3 */
#if defined(I2C4)
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_I2C4);
#endif /* I2C4 */
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_LPTIM1);
#if defined(POSENC1)
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_POSENC1);
#endif /* POSENC1 */
#if defined(POSENC2)
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_POSENC2);
#endif /* POSENC2 */
#if defined(OPAMP1)
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_OPAMP1);
#endif /* OPAMP1 */
#if defined(OPAMP2)
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_OPAMP2);
#endif /* OPAMP2 */
#if defined(OPAMP3)
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_OPAMP3);
#endif /* OPAMP3 */
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_VREFBUF);
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_RTCAPB);

  /* Select kernel clocks for peripherals that remain configured after wakeup. */
  /* USART, UART and LPUART kernel clocks. */
  /* PLL2Q provides the selected serial kernel clock source. */
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PLL2Q);
  LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PLL2Q);
#if defined(USART3)
  LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PLL2Q);
#endif /* USART3 */
  LL_RCC_SetUARTClockSource(LL_RCC_UART4_CLKSOURCE_PLL2Q);
  LL_RCC_SetUARTClockSource(LL_RCC_UART5_CLKSOURCE_PLL2Q);
  LL_RCC_SetUARTClockSource(LL_RCC_UART6_CLKSOURCE_PLL2Q);
  LL_RCC_SetLPUARTClockSource(LL_RCC_LPUART1_CLKSOURCE_PLL2Q);
  LL_RCC_SetDACClockSource(LL_RCC_DAC1_CLKSOURCE_PLL2Q);
  LL_RCC_SetDACClockSource(LL_RCC_DAC2_CLKSOURCE_PLL2Q);
  LL_RCC_SetDACClockSource(LL_RCC_DAC5_CLKSOURCE_PLL2Q);
  LL_RCC_SetDACClockSource(LL_RCC_DAC6_CLKSOURCE_PLL2Q);
  LL_RCC_SetDACClockSource(LL_RCC_DAC7_CLKSOURCE_PLL2Q);
  LL_RCC_SetDACClockSource(LL_RCC_DAC8_CLKSOURCE_PLL2Q);
  LL_RCC_SetDACClockSource(LL_RCC_DAC9_CLKSOURCE_PLL2Q);
  LL_RCC_SetDACClockSource(LL_RCC_DAC10_CLKSOURCE_PLL2Q);

  /* SPI, PLAY1 and LPTIM kernel clocks. */
  /* PLL2P provides the selected synchronous peripheral clock source. */
  LL_RCC_SetSPIClockSource(LL_RCC_SPI1_CLKSOURCE_PLL2P);
  LL_RCC_SetSPIClockSource(LL_RCC_SPI2_CLKSOURCE_PLL2P);
  LL_RCC_SetSPIClockSource(LL_RCC_SPI3_CLKSOURCE_PLL2P);
  LL_RCC_SetPLAYClockSource(LL_RCC_PLAY1_CLKSOURCE_PLL2P);
  LL_RCC_SetLPTIMClockSource(LL_RCC_LPTIM1_CLKSOURCE_PLL2P);

  /* Memory, power-management and interconnect kernel clocks. */
  /* These selections match the board clock plan generated for the example. */
  LL_RCC_SetXSPIClockSource(LL_RCC_XSPI1_CLKSOURCE_HSE);
  LL_RCC_SetUCPDClockSource(LL_RCC_UCPD1_CLKSOURCE_PLL2Q);
  LL_RCC_SetUSBClockSource(LL_RCC_USB_CLKSOURCE_PLL2Q);
  LL_RCC_SetI3CClockSource(LL_RCC_I3C1_CLKSOURCE_HSE);
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSE);
  LL_RCC_SetI2CClockSource(LL_RCC_I2C2_CLKSOURCE_HSE);
  LL_RCC_SetI2CClockSource(LL_RCC_I2C3_CLKSOURCE_HSE);
  LL_RCC_SetI2CClockSource(LL_RCC_I2C4_CLKSOURCE_HSE);

  /* ADC, FDCAN and position encoder kernel clocks. */
  /* Configure kernel sources before isolating the RCC peripheral buses. */
  LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_PLL2P);
  LL_RCC_SetFDCANClockSource(LL_RCC_FDCAN_CLKSOURCE_PLL2Q);
#if defined(POSENC1)
  LL_RCC_SetPOSENCClockSource(LL_RCC_POSENC1_CLKSOURCE_PLL2P);
#endif /* POSENC1 */
#if defined(POSENC2)
  LL_RCC_SetPOSENCClockSource(LL_RCC_POSENC2_CLKSOURCE_PLL2P);
#endif /* POSENC2 */

  /* Isolate unused RCC peripheral buses after configuration. */
  LL_AHB1_DisableBusClock();
  LL_AHB2_DisableBusClock();
  LL_APB1_DisableBusClock();
  LL_APB2_DisableBusClock();
  LL_APB3_DisableBusClock();

  return SYSTEM_OK;
}


/** brief: Enable the EXTI Line 13 interrupt.
  */
inline void EXTI_EnableLineMode_IT(void)
{
  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_13);
}


/** brief: Enter the Sleep mode request.
  */
inline void PWR_EnterSleepMode(void)
{
  /* Clear SLEEPDEEP bit of Cortex System Control Register */
  SCB_DisableDeepSleep();

  /* Wait For Interrupt Request */
  __WFI();
}


/** brief: Handles the EXTI line interrupts.
  */
void mx_extix_irqhandler(void)
{
  if (LL_EXTI_IsActiveRisingFlag_0_31(MX_EXTIX_EXTI_LINE) != 0U)
  {
    LL_EXTI_ClearRisingFlag_0_31(MX_EXTIX_EXTI_LINE);
  }
}
