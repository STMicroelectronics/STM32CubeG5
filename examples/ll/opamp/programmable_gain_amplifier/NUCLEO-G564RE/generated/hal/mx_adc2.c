/**
  ******************************************************************************
  * @file           : mx_adc2.c
  * @brief          : ADC2 Peripheral initialization
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
#include "mx_adc2.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/* Exported functions for ADC2 in LL layer */
/******************************************************************************/
ADC_TypeDef *mx_adc2_init(void)
{
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC12);
  LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_TIM);

  /****************************************************************************/
  /* Configuration of basic features (mandatory)                              */
  /****************************************************************************/

  /* ==================== ADC instance ==================== */
  /* LL_ADC_SetResolution(ADC2, LL_ADC_RESOLUTION_12B); */ /* Configuration matches register reset state at startup. */
  /* LL_ADC_SetSamplingMode(ADC2, LL_ADC_SAMPLING_MODE_NORMAL); */ /* Configuration matches register reset state at startup. */

  /* ==================== Group Regular ==================== */
  /* LL_ADC_REG_SetContinuousMode(ADC2, LL_ADC_REG_CONV_SINGLE); */ /* Configuration matches register reset state at startup. */
  /* LL_ADC_REG_SetTriggerSource(ADC2, LL_ADC_REG_TRIG_SOFTWARE); */ /* Configuration matches register reset state at startup. */

  LL_ADC_REG_SetOverrun(ADC2, LL_ADC_REG_OVR_DATA_OVERWRITTEN);
  /* LL_ADC_REG_SetSequencerDiscont(ADC2, LL_ADC_REG_SEQ_DISCONT_DISABLE); */ /* Configuration matches register reset state at startup. */
  /* LL_ADC_REG_SetSequencerLength(ADC2, LL_ADC_REG_SEQ_SCAN_DISABLE); */ /* Configuration matches register reset state at startup. */

  /* Configure the ADC regular group sequencer */
  LL_ADC_REG_SetSequencerRanks(ADC2, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_3);
  LL_ADC_SetChannelSamplingTime(ADC2, LL_ADC_CHANNEL_3, LL_ADC_SAMPLINGTIME_848CYCLES_5);
  LL_ADC_SetChannelSingleDiff(ADC2, LL_ADC_CHANNEL_3, LL_ADC_IN_SINGLE_ENDED);
  LL_ADC_SetChannelPreselection(ADC2, LL_ADC_CHANNEL_3);

  /* ### ADC2 GPIO Configuration ########################### */

  /* GPIO Clocks activation */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);

  /**
    [GPIO Pin] ------> [Signal Name]

       PA7     ------>   ADC2_INP4
    **/

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_7, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  /* LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_ANALOG); */ /* Configuration matches register reset state at startup. */

  /* Enable the interruption for ADC */
  NVIC_SetPriority(ADC2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(ADC2_IRQn);
return ADC2;
}

void mx_adc2_deinit(void)
{
  LL_ADC_Disable(ADC2);

  NVIC_DisableIRQ(ADC2_IRQn);
  /* ### GPIO deinitialization of ADC2: ADC2_INP4 ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_7, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_7, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOA, LL_GPIO_PIN_7, LL_GPIO_PIN_RESET);
}

/******************************************************************************/
/*          ADC2 global interrupt is managed directly in user code.           */
/******************************************************************************/
/* void ADC2_IRQHandler(void)
{
}
  */
