/**
  ******************************************************************************
  * @file           : mx_hrtim1.c
  * @brief          : Peripheral initialization
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
#include "mx_hrtim1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/

/* Exported variables by reference--------------------------------------------*/

/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for SW instance in LL layer */
/******************************************************************************/

HRTIM_TypeDef *mx_hrtim1_init(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_HRTIM1);
  /* No GPIO configuration required for HRTIM1 */
  /* HRTIM output channels configuration */
  LL_HRTIM_OUT_SetOutputSetSrc(HRTIM1, LL_HRTIM_OUTPUT_CHB2, LL_HRTIM_OUTPUTSET_PERIOD);
  LL_HRTIM_OUT_SetOutputResetSrc(HRTIM1, LL_HRTIM_OUTPUT_CHB2, LL_HRTIM_OUTPUTRESET_CMP1);
  /* LL_HRTIM_OUT_SetPolarity(HRTIM1, LL_HRTIM_OUTPUT_CHB2, LL_HRTIM_OUT_POSITIVE_POLARITY); */ /* Configuration matches register reset state at startup. */
  /* LL_HRTIM_OUT_SetIdleLevel(HRTIM1, LL_HRTIM_OUTPUT_CHB2, LL_HRTIM_OUT_IDLELEVEL_INACTIVE); */ /* Configuration matches register reset state at startup. */
  /* LL_HRTIM_OUT_SetFaultState(HRTIM1, LL_HRTIM_OUTPUT_CHB2, LL_HRTIM_OUT_FAULTSTATE_NONE); */ /* Configuration matches register reset state at startup. */

  /* Timer B configuration */
  LL_HRTIM_TIM_SetPrescaler(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_PRESCALERRATIO_MUL16);
  LL_HRTIM_TIM_SetPeriod(HRTIM1, LL_HRTIM_TIMB, 44800);
  /* LL_HRTIM_TIM_SetRepetition(HRTIM1, LL_HRTIM_TIMB, 0); */ /* Configuration matches register reset state at startup. */
  LL_HRTIM_TIM_SetCounterMode(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_MODE_CONTINUOUS);
  /* LL_HRTIM_TIM_SetCountingMode(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_COUNTING_MODE_UP); */ /* Configuration matches register reset state at startup. */

  /* LL_HRTIM_TIM_SetUpdateGating(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_UPDATEGAT_INDEPENDENT); */ /* Configuration matches register reset state at startup. */
  /* LL_HRTIM_TIM_SetResetTrig(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_RESETTRIG_NONE); */ /* Configuration matches register reset state at startup. */
  /* LL_HRTIM_TIM_SetInterleavedMode(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_INTERLEAVED_MODE_DISABLE); */ /* Configuration matches register reset state at startup. */
  /* LL_HRTIM_TIM_SetBurstModeOption(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_BURSTMODE_MAINTAINCLOCK); */ /* Configuration matches register reset state at startup. */
  LL_HRTIM_OUT_SetIdleMode(HRTIM1, LL_HRTIM_OUTPUT_CHB1, LL_HRTIM_OUT_IDLE_WHEN_BURST);
  /* LL_HRTIM_OUT_SetBMEntryMode(HRTIM1, LL_HRTIM_OUTPUT_CHB1, LL_HRTIM_OUT_BM_ENTRYMODE_REGULAR); */ /* Configuration matches register reset state at startup. */
  LL_HRTIM_OUT_SetIdleMode(HRTIM1, LL_HRTIM_OUTPUT_CHB2, LL_HRTIM_OUT_IDLE_WHEN_BURST);
  /* LL_HRTIM_OUT_SetBMEntryMode(HRTIM1, LL_HRTIM_OUTPUT_CHB2, LL_HRTIM_OUT_BM_ENTRYMODE_REGULAR); */ /* Configuration matches register reset state at startup. */
  /* LL_HRTIM_TIM_SetComp1Mode(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_GTCMP1_EQUAL); */ /* Configuration matches register reset state at startup. */
  /* LL_HRTIM_TIM_SetTriggeredHalfMode(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_TRIGHALF_DISABLE); */ /* Configuration matches register reset state at startup. */
  /* LL_HRTIM_TIM_SetCompareMode(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_COMPARE_UNIT_2, LL_HRTIM_COMPAREMODE_REGULAR); */ /* Configuration matches register reset state at startup. */
  /* LL_HRTIM_TIM_SetComp3Mode(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_GTCMP3_EQUAL); */ /* Configuration matches register reset state at startup. */
  /* LL_HRTIM_TIM_SetCompareMode(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_COMPARE_UNIT_4, LL_HRTIM_COMPAREMODE_REGULAR); */ /* Configuration matches register reset state at startup. */
  LL_HRTIM_TIM_SetCompareValue(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_COMPARE_UNIT_1, 22400);
  /* LL_HRTIM_TIM_SetCompareValue(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_COMPARE_UNIT_2, 0); */ /* Configuration matches register reset state at startup. */
  /* LL_HRTIM_TIM_SetCompareValue(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_COMPARE_UNIT_3, 0); */ /* Configuration matches register reset state at startup. */
  /* LL_HRTIM_TIM_SetCompareValue(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_COMPARE_UNIT_4, 0); */ /* Configuration matches register reset state at startup. */

  /* LL_HRTIM_TIM_SetCaptureTrig(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_CAPTURE_UNIT_1, LL_HRTIM_CAPTURETRIG_NONE); */ /* Configuration matches register reset state at startup. */
  /* LL_HRTIM_TIM_SetCaptureTrig(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_CAPTURE_UNIT_2, LL_HRTIM_CAPTURETRIG_NONE); */ /* Configuration matches register reset state at startup. */
  /* LL_HRTIM_TIM_SetDACTrig(HRTIM1, LL_HRTIM_TIMB, LL_HRTIM_DACTRIG_NONE); */ /* Configuration matches register reset state at startup. */

  /* ### HRTIM1 GPIO Configuration ########################### */

  /* GPIO Clocks activation */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);

  /**
    [GPIO Pin] ------> [Signal Name]

       PA10    ------>   HRTIM1_CHB2
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  /* LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_10, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_10, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_10, LL_GPIO_AF_12);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_ALTERNATE);

  return HRTIM1;
}

void mx_hrtim1_deinit(void)
{
  LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_HRTIM1);

  LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_HRTIM1);
  LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_HRTIM1);

  /* ### GPIO deinitialization of HRTIM1: HRTIM1_CHB2 ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_10, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_10, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_10, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_10, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_10, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOA, LL_GPIO_PIN_10, LL_GPIO_PIN_RESET);
}
