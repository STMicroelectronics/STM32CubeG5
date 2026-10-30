/**
  ******************************************************************************
  * file           : stm32g5xx_ll_example.c
  * brief          : Enter and exit the Stop mode through an EXTI interrupt using the LL API
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
void mx_extix_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

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

  /* Disable HSI in Stop mode */
  LL_RCC_HSI_DisableInStopMode();

  /* SRAM1 is not required by this Stop example. */
  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_SRAM1);

  /* SRAM3 and DCACHE clocks are enabled after reset but unused by this example. */
#if defined(SRAM3_BASE)
  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_SRAM3);
#endif /* SRAM3_BASE */
#if defined(DCACHE)
  LL_AHB1_GRP1_DisableClock(LL_AHB1_GRP1_PERIPH_DCACHE);
#endif /* DCACHE */

  /* Disable all peripheral clocks while the core is Stopping. */
  /* Memory, accelerators and analog peripherals. */
  /* These gates affect Stop mode only and are restored after wake-up. */
  LL_AHB1_GRP1_DisableClockLowPower(LL_AHB1_GRP1_PERIPH_SRAM1);
  LL_AHB1_GRP1_DisableClockLowPower(LL_AHB1_GRP1_PERIPH_SRAM2);
#if defined(SRAM3_BASE)
  LL_AHB1_GRP1_DisableClockLowPower(LL_AHB1_GRP1_PERIPH_SRAM3);
#endif /* SRAM3_BASE */

  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_DAC1);
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_DAC2);
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_DAC5);
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_DAC6);
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_DAC7);
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_DAC8);
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_DAC9);
  LL_AHB2_GRP1_DisableClockLowPower(LL_AHB2_GRP1_PERIPH_DAC10);

  /* External memory and connectivity peripherals. */
  /* No peripheral in this group is active during the Stop interval. */
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_SPI2);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_SPI3);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_USART2);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_USART3);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_UART4);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_UART5);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_I2C1);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_I2C2);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_I3C1);
  LL_APB1_GRP1_DisableClockLowPower(LL_APB1_GRP1_PERIPH_UART6);
  LL_APB1_GRP2_DisableClockLowPower(LL_APB1_GRP2_PERIPH_DTS);
  LL_APB1_GRP2_DisableClockLowPower(LL_APB1_GRP2_PERIPH_UCPD1);

  /* Timers, serial interfaces and low-power peripherals. */
  /* The wake-up source is EXTI13, not a timer or serial peripheral. */
#if defined(PLAY1)
  LL_APB2_GRP1_DisableClockLowPower(LL_APB2_GRP1_PERIPH_PLAY1);
#endif /* PLAY1 */
#if defined(USB_DRD_FS)
  LL_APB2_GRP1_DisableClockLowPower(LL_APB2_GRP1_PERIPH_USB);
#endif /* USB_DRD_FS */
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_LPUART1);
#if defined(I2C3)
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_I2C3);
#endif /* I2C3 */
#if defined(I2C4)
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_I2C4);
#endif /* I2C4 */
  LL_APB3_GRP1_DisableClockLowPower(LL_APB3_GRP1_PERIPH_LPTIM1);
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


/** brief: Clear the Stop flag.
  */
inline void PWR_ClearStopFlag(void)
{
  LL_PWR_ClearFlag_STOP();
}


/** brief: Enter the Stop mode request.
  */
inline void PWR_EnterStopMode(void)
{
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SCB_EnableDeepSleep();

  LL_PWR_SetPowerMode(LL_PWR_STOP_MODE);

  /* Ensure all memory accesses complete before entering Stop mode */
  __DSB();

  /* Wait For Interrupt Request */
  __WFI();

  /* Clear SLEEPDEEP bit after exiting Stop mode */
  SCB_DisableDeepSleep();
}


/** brief:  Check if the system has resumed from Stop mode.
  * retval: system_status_t
  */
inline system_status_t PWR_CheckSystemStopMode(void)
{
  system_status_t system_status = SYSTEM_PERIPHERAL_ERROR;

  /* Check that the system has resumed from Stop mode */
  if (LL_PWR_IsActiveFlag_STOP() != 0U)
  {
    system_status = SYSTEM_OK;
  }

  return system_status;
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