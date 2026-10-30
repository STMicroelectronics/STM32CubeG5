/**
  ******************************************************************************
  * @file           : mx_tim8.c
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
#include "mx_tim8.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for TIM8 in LL layer */
/******************************************************************************/

TIM_TypeDef *mx_tim8_init(void)
{
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM8);

  /* Timer configuration to reach the output frequency at 24.005 kHz */
  /* LL_TIM_SetPrescaler(TIM8, 0); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_SetCounterMode(TIM8, LL_TIM_COUNTERMODE_UP); */ /* Configuration matches register reset state at startup. */
  LL_TIM_SetAutoReload(TIM8, 11663);
  /* LL_TIM_SetClockDivision(TIM8, LL_TIM_CLOCKDIVISION_DIV1); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_SetClockDivision2(TIM8, LL_TIM_CLOCKDIVISION2_DIV1); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_SetClockSource(TIM8, LL_TIM_CLK_INTERNAL); */ /* Configuration matches register reset state at startup. */

  /* Channel 1 Configuration */
  /* LL_TIM_OC_SetPolarity(TIM8, LL_TIM_CHANNEL_CH1, LL_TIM_OCPOLARITY_HIGH); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_OC_SetIdleState(TIM8, LL_TIM_CHANNEL_CH1, LL_TIM_OCIDLESTATE_RESET); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_OC_SetOverrideState(TIM8, LL_TIM_CHANNEL_CH1, LL_TIM_OCOVERRIDE_RESET); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_OC_SetBreakMode(TIM8, LL_TIM_CHANNEL_CH1, LL_TIM_OCBREAKMODE_IMMEDIATE); */ /* Configuration matches register reset state at startup. */

  /* Compare Unit 1 Configuration */
  LL_TIM_OC_SetMode(TIM8, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
  LL_TIM_OC_SetCompareValue(TIM8, LL_TIM_OC_COMPARE_UNIT_1, 5832);
  LL_TIM_OC_EnablePreload(TIM8, LL_TIM_CHANNEL_CH1);

  /* Channel 2 Configuration */
  /* LL_TIM_OC_SetPolarity(TIM8, LL_TIM_CHANNEL_CH2, LL_TIM_OCPOLARITY_HIGH); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_OC_SetIdleState(TIM8, LL_TIM_CHANNEL_CH2, LL_TIM_OCIDLESTATE_RESET); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_OC_SetOverrideState(TIM8, LL_TIM_CHANNEL_CH2, LL_TIM_OCOVERRIDE_RESET); */ /* Configuration matches register reset state at startup. */
  /* LL_TIM_OC_SetBreakMode(TIM8, LL_TIM_CHANNEL_CH2, LL_TIM_OCBREAKMODE_IMMEDIATE); */ /* Configuration matches register reset state at startup. */

  /* Compare Unit 2 Configuration */
  LL_TIM_OC_SetMode(TIM8, LL_TIM_CHANNEL_CH2, LL_TIM_OCMODE_PWM1);
  LL_TIM_OC_SetCompareValue(TIM8, LL_TIM_OC_COMPARE_UNIT_2, 4374);
  LL_TIM_OC_EnablePreload(TIM8, LL_TIM_CHANNEL_CH2);

  /* Disable update interrupt with update generation (UG)
     by changing update request source (URS) to avoid update flag (UIF) */
  LL_TIM_SetUpdateSource(TIM8, LL_TIM_UPDATESOURCE_COUNTER);
  /* Generate an update event to reload the prescaler
     and the repetition counter (if applicable) values immediately */
  LL_TIM_GenerateEvent_UPDATE(TIM8);

  /* Update Event Management */
  LL_TIM_SetUpdateSource(TIM8, LL_TIM_UPDATESOURCE_REGULAR);
  /* LL_TIM_EnableUpdateEvent(TIM8); */ /* Configuration matches register reset state at startup. */

  /* ### TIM8 GPIO Configuration ########################### */

  /* GPIO Clocks activation */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC | LL_AHB4_GRP1_PERIPH_GPIOB);

  /**
    [GPIO Pin] ------> [Signal Name]

       PC6     ------>   TIM8_CH1
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  /* LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_6, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_6, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_0_7(GPIOC, LL_GPIO_PIN_6, LL_GPIO_AF_4);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);

  /**
    [GPIO Pin] ------> [Signal Name]

       PB6     ------>   TIM8_CH2
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  /* LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_6, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_6, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_6, LL_GPIO_AF_5);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);

  return TIM8;
}

void mx_tim8_deinit(void)
{
  LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM8);

  LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM8);
  LL_APB2_GRP1_ReleaseReset(LL_APB2_GRP1_PERIPH_TIM8);

  /* ### GPIO deinitialization of TIM8: TIM8_CH1 ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_6, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_0_7(GPIOC, LL_GPIO_PIN_6, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOC, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOC, LL_GPIO_PIN_6, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOC, LL_GPIO_PIN_6, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOC, LL_GPIO_PIN_6, LL_GPIO_PIN_RESET);

  /* ### GPIO deinitialization of TIM8: TIM8_CH2 ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_6, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_6, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_6, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOB, LL_GPIO_PIN_6, LL_GPIO_PIN_RESET);
}
