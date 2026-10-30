/**
  ******************************************************************************
  * @file           : mx_i2c1.c
  * @brief          : I2C1 Peripheral initialization
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
#include "mx_i2c1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/* Exported functions for I2C1 in LL layer */
/******************************************************************************/
I2C_TypeDef *mx_i2c1_i2c_init(void)
{
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
  /* LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_PCLK1); */ /* Configuration matches register reset state at startup. */

  /*
    Timing automatically calculated with:
     - I2C1 input clock at 140000000 Hz
     - I2C clock speed at 1000000 Hz
  */
  LL_I2C_SetTiming(I2C1, 0xF02A3D);
  LL_I2C_ConfigOwnAddress1(I2C1, 93 << 1U, LL_I2C_OWNADDRESS1_7BIT);
  /* LL_I2C_SetMasterAddressingMode(I2C1, LL_I2C_ADDRESSING_MODE_7BIT); */ /* Configuration matches register reset state at startup. */
  LL_I2C_WRITE_REG(I2C1, CR2, (LL_I2C_READ_REG(I2C1, CR2) | I2C_CR2_AUTOEND | I2C_CR2_NACK));

  LL_I2C_Enable(I2C1);
  /* ### I2C1 GPIO Configuration ########################### */

  /* GPIO Clocks activation */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA | LL_AHB4_GRP1_PERIPH_GPIOB);

  /**
    [GPIO Pin] ------> [Signal Name]

       PA15    ------>   I2C1_SCL
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  /* LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_15, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */

  /* Configure IO output type (Push-Pull or Open-Drain) */
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_15, LL_GPIO_OUTPUT_OPENDRAIN);

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_15, LL_GPIO_PULL_UP); */ /* Configuration matches register reset state at startup. */

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOA, LL_GPIO_PIN_15, LL_GPIO_AF_4);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  /* LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_15, LL_GPIO_MODE_ALTERNATE); */ /* Configuration matches register reset state at startup. */

  /**
    [GPIO Pin] ------> [Signal Name]

       PB9     ------>   I2C1_SDA
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  /* LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */

  /* Configure IO output type (Push-Pull or Open-Drain) */
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_OPENDRAIN);

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_9, LL_GPIO_PULL_UP);

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_9, LL_GPIO_AF_4);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);

  /* LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_PCLK1); */ /* Configuration matches register reset state at startup. */
  /* Enable the Event interruption for I2C */
  NVIC_SetPriority(I2C1_EV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(I2C1_EV_IRQn);

  /* Enable the Error interruption for I2C */
  NVIC_SetPriority(I2C1_ERR_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(I2C1_ERR_IRQn);

  return I2C1;
}

void mx_i2c1_i2c_deinit(void)
{
  /* Disable the Event interruption for I2C */
  NVIC_DisableIRQ(I2C1_EV_IRQn);

  /* Disable the Error interruption for I2C */
  NVIC_DisableIRQ(I2C1_ERR_IRQn);

  LL_I2C_Disable(I2C1);

  LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_I2C1);
  LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_I2C1);

  LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_I2C1);
  /* ### GPIO deinitialization of I2C1: I2C1_SCL ########################### */

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

  /* ### GPIO deinitialization of I2C1: I2C1_SDA ########################### */

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
/*           I2C1 event interrupt is managed directly in user code.           */
/******************************************************************************/
/* void I2C1_EV_IRQHandler(void)
{
}
  */

/******************************************************************************/
/*           I2C1 error interrupt is managed directly in user code.           */
/******************************************************************************/
/* void I2C1_ERR_IRQHandler(void)
{
}
  */
