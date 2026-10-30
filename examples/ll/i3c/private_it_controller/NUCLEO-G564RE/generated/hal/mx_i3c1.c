/**
  ******************************************************************************
  * @file           : mx_i3c1.c
  * @brief          : I3C1 Peripheral initialization
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
#include "mx_i3c1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/* Exported functions for I3C in LL layer */
/******************************************************************************/
I3C_TypeDef *mx_i3c1_init(void)
{
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I3C1);
  /* LL_RCC_SetI3CClockSource(LL_RCC_I3C1_CLKSOURCE_PCLK1); */ /* Configuration matches register reset state at startup. */

  /* Set the controller mode */
  LL_I3C_SetMode(I3C1, LL_I3C_MODE_CONTROLLER);

  /**
    * I3C1 timing_reg0 calculated by CubeMX2 with:
    * - SDA rise time = 350 ns
    * - Input frequency = 140 MHz
    * - Bus usage = UTILS_I3C_PURE_I3C_BUS
    * - I3C bus frequency = 12.5 MHz
    * - I3C duty cycle = 50 %
    * I3C1 timing_reg1 calculated by CubeMX2 with:
    * - Wait time = LL_I3C_OWN_ACTIVITY_STATE_0
    */
  LL_I3C_ConfigClockWaveForm(I3C1, 0x320504U);
  LL_I3C_SetBusCharacteristic(I3C1, 0x1C008AU);

  /**
    * Set the SCL clock stalling cycles (converted from nanoseconds to cycles using kernel clock frequency).
    * stall_time_cycle = (stall_time_ns * input_frequency_hz / 1000000000), rounded up to the next integer
    * stall_time_cycle = (1020 * 140000000 / 1000000000), rounded up = 143U
    */
  LL_I3C_ConfigStallTime(I3C1, 143U, LL_I3C_CTRL_STALL_TX | LL_I3C_CTRL_STALL_RX);

  /* Set the FIFO configuration */
  LL_I3C_ConfigCtrlFifo(I3C1, LL_I3C_TXFIFO_THRESHOLD_1_2, LL_I3C_RXFIFO_THRESHOLD_1_2, LL_I3C_CTRL_FIFO_CONTROL_ONLY);

  /* Enable the I3C peripheral */
  LL_I3C_Enable(I3C1);

  /* ### I3C1 GPIO Configuration ########################### */

  /* GPIO Clocks activation */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA | LL_AHB4_GRP1_PERIPH_GPIOB);

  /**
    [GPIO Pin] ------> [Signal Name]

       PA15    ------>   I3C1_SCL
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_15, LL_GPIO_SPEED_FREQ_HIGH);

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_15, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_15, LL_GPIO_PULL_UP); */ /* Configuration matches register reset state at startup. */

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_15, LL_GPIO_AF_3);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  /* LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_15, LL_GPIO_MODE_ALTERNATE); */ /* Configuration matches register reset state at startup. */

  /**
    [GPIO Pin] ------> [Signal Name]

       PB9     ------>   I3C1_SDA
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_HIGH);

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_9, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_9, LL_GPIO_AF_3);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);

  /* Enable the Event interrupt for I3C1 */
  NVIC_SetPriority(I3C1_EV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(I3C1_EV_IRQn);

  /* Enable the Error interrupt for I3C1 */
  NVIC_SetPriority(I3C1_ERR_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(I3C1_ERR_IRQn);

  return I3C1;
}

void mx_i3c1_deinit(void)
{
  /* Disable the Event interrupt for I3C1 */
  NVIC_DisableIRQ(I3C1_EV_IRQn);

  /* Disable the Error interrupt for I3C1 */
  NVIC_DisableIRQ(I3C1_ERR_IRQn);

  LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_I3C1);
  LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_I3C1);

  LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_I3C1);
  /* ### GPIO deinitialization of I3C1: I3C1_SCL ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_15, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_15, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_15, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_15, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_15, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOA, LL_GPIO_PIN_15, LL_GPIO_PIN_RESET);

  /* ### GPIO deinitialization of I3C1: I3C1_SDA ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_9, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_9, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_9, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOB, LL_GPIO_PIN_9, LL_GPIO_PIN_RESET);
}

/******************************************************************************/
/*           I3C1 event interrupt is managed directly in user code.           */
/******************************************************************************/
/* void I3C1_EV_IRQHandler(void)
{
}
  */

/******************************************************************************/
/*           I3C1 error interrupt is managed directly in user code.           */
/******************************************************************************/
/* void I3C1_ERR_IRQHandler(void)
{
}
  */
