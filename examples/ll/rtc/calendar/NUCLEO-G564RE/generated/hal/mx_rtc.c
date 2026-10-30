/**
  ******************************************************************************
  * @file           : mx_rtc.c
  * @brief          : RTC Peripheral initialization
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
#include "mx_rtc.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/** Exported functions for RTC in LL layer            **/
/******************************************************************************/

system_status_t mx_rtc_init(void)
{
  /* Disable RTC Domain Write Protection */
  LL_PWR_DisableRTCDomainWriteProtection();

  /* Clock configuration */
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSE);

  LL_APB3_GRP1_EnableClock(LL_APB3_GRP1_PERIPH_RTCAPB);
  LL_RCC_EnableRTC();

  /* Disable write protection */
  LL_RTC_DisableWriteProtection();

  /* Enter init mode */
  uint32_t timeout = 1000;
  if (LL_RTC_IsActiveFlag_INIT() == 0U)
  {
    LL_RTC_EnableInitMode();
    while (LL_RTC_IsActiveFlag_INIT() != 1)
    {
      LL_Delay_NoISR(1);
      timeout--;
      if (timeout == 0)
      {
        return SYSTEM_PERIPHERAL_ERROR;
      }
    }
  }

  /* Configure mode and RTC prescalers */
  LL_RTC_SetPrescalers(127U - 1U, 255U - 1U);
  /* LL_RTC_SetBinaryMode(LL_RTC_BINARY_NONE); */ /* Configuration matches register reset state at startup. */

  /* Configure calendar */
  /* LL_RTC_SetHourFormatAndShadowRegBypass(LL_RTC_HOUR_FORMAT_24HOUR, LL_RTC_SHADOW_REG_KEEP); */ /* Configuration matches register reset state at startup. */
  LL_RTC_DATE_Config(LL_RTC_WEEKDAY_SATURDAY, 0x31U, LL_RTC_MONTH_JULY, 0x25U);
  LL_RTC_TIME_Config(LL_RTC_TIME_FORMAT_AM_24H, 0x12U, 0x0U, 0x0U);

  /* Exit init mode */
  timeout = 1000;
  LL_RTC_DisableInitMode();
  if (LL_RTC_IsEnabledBypassShadowReg() == 0U)
  {
    if (LL_RTC_IsActiveFlag_INIT() == 0U)
    {
      LL_RTC_ClearFlag_RS();
      while (LL_RTC_IsActiveFlag_RS() == 0U)
      {
        LL_Delay_NoISR(1) ;
        timeout--;
        if (timeout == 0)
        {
          return SYSTEM_PERIPHERAL_ERROR;
        }
      }
    }
  }

  /* Enable write protection */
  LL_RTC_EnableWriteProtection();

  /* No GPIO configuration required for RTC */
  return SYSTEM_OK;
}

void mx_rtc_deinit(void)
{
  /* No GPIO de-initialization required for RTC */
}
