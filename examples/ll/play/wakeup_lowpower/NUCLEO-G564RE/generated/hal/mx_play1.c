/**
  ******************************************************************************
  * @file           : mx_play1.c
  * @brief          : PLAY1 Peripheral initialization
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
#include "mx_play1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/* Exported functions for PLAY1 in LL layer */
/******************************************************************************/
PLAY_TypeDef *mx_play1_init(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_PLAY1);
  LL_RCC_SetPLAYClockSource(LL_RCC_PLAY1_CLKSOURCE_LSI);

  /************** PLAY input configuration **************/

  /* Input: User button */
  LL_PLAY_INPUT_Config(PLAY1, LL_PLAY_IN4, 0U, LL_PLAY_EDGE_DETECTION_MODE_BYPASSED, 100U);

  /************** PLAY LUT configuration **************/

  /* LUT0 configuration */
  /* Logical expression: A */
  LL_PLAY_LUT_SetTruthTable(PLAY1, LL_PLAY_LUT0, 0xAAAA);
  /* Unused inputs are connected to SW trigger 15 */
  LL_PLAY_LUT_ConfigInputs(PLAY1, LL_PLAY_LUT0, LL_PLAY_LUT_SOURCE_FILTER4, LL_PLAY_LUT_SOURCE_SWTRIG15, LL_PLAY_LUT_SOURCE_SWTRIG15, LL_PLAY_LUT_SOURCE_SWTRIG15, LL_PLAY_LUT_CLK_GATE_OFF);

  /************** PLAY output configuration **************/

  /* Output: PLAY signal to EXTI for wakeup */
  /* Connect LUT0 (direct output) to PLAY output 14 */
  /* LL_PLAY_OUTPUT_SetSource(PLAY1, LL_PLAY_OUT14, LL_PLAY_LUT0_OUT_DIRECT); */ /* Configuration matches register reset state at startup. */

  /* ### PLAY1 GPIO Configuration ########################### */

  /* GPIO Clocks activation */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC);

  /**
    [GPIO Pin] ------> [Signal Name]

       PC13    ------>   PLAY1_IN4
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_13, LL_GPIO_SPEED_FREQ_HIGH);

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_13, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_13, LL_GPIO_PULL_DOWN);

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_13, LL_GPIO_AF_14);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_ALTERNATE);

  /* LL_EXTI_DisableRisingTrig_32_63(LL_EXTI_LINE_51); */ /* Configuration matches register reset state at startup. */
  LL_EXTI_EnableFallingTrig_32_63(LL_EXTI_LINE_51);
  /* Set line 51 Interrupt priority */
  NVIC_SetPriority(PLAY1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(PLAY1_IRQn);

  NVIC_SetPriority(PLAY1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(PLAY1_IRQn);

  return PLAY1;
}

void mx_play1_deinit(void)
{
  NVIC_DisableIRQ(PLAY1_IRQn);

  LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_PLAY1);
  LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_PLAY1GLOBAL);
  LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_PLAY1GLOBAL);

  /* ### GPIO deinitialization of PLAY1: PLAY1_IN4 ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_13, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOC, LL_GPIO_PIN_13, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_13, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_13, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_13, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOC, LL_GPIO_PIN_13, LL_GPIO_PIN_RESET);

  LL_EXTI_DisableIT_32_63(LL_EXTI_LINE_51);
  LL_EXTI_DisableEvent_32_63(LL_EXTI_LINE_51);
  LL_EXTI_DisableRisingTrig_32_63(LL_EXTI_LINE_51);
  LL_EXTI_DisableFallingTrig_32_63(LL_EXTI_LINE_51);
  LL_EXTI_ClearRisingFlag_32_63(LL_EXTI_LINE_51);
  LL_EXTI_ClearFallingFlag_32_63(LL_EXTI_LINE_51);

  /* Set line 51 Interrupt priority */
  NVIC_DisableIRQ(PLAY1_IRQn);
}

/******************************************************************************/
/*    PLAY global non-secure interrupt OR play1_out14 through EXTI line is managed directly in user code.   */
/*
void PLAY1_IRQHandler(void)
{
}
  */
/******************************************************************************/
