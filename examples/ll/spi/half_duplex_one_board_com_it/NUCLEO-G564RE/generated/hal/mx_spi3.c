/**
  ******************************************************************************
  * @file           : mx_spi3.c
  * @brief          : SPI3 Peripheral initialization
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
#include "mx_spi3.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/* Exported functions for SPI in LL layer */
/******************************************************************************/
SPI_TypeDef *mx_spi3_init(void)
{
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI3);
  /* LL_RCC_SetSPIClockSource(LL_RCC_SPI3_CLKSOURCE_PCLK1); */ /* Configuration matches register reset state at startup. */
  uint32_t cfg1_config = LL_SPI_DATA_WIDTH_8_BIT | LL_SPI_BAUD_RATE_PRESCALER_2;
  uint32_t cfg2_config = LL_SPI_MODE_SLAVE | LL_SPI_HALF_DUPLEX | LL_SPI_CLOCK_POLARITY_LOW
                       | LL_SPI_CLOCK_PHASE_1_EDGE | LL_SPI_MSB_FIRST | LL_SPI_NSS_SOFT;

  LL_SPI_SetConfig(SPI3, cfg1_config, cfg2_config);

  /* LL_SPI_SetStandard(SPI3, LL_SPI_PROTOCOL_MOTOROLA); */ /* Configuration matches register reset state at startup. */

  /* LL_SPI_SetInterDataIdleness(SPI3, LL_SPI_MIDI_DELAY_0_CYCLE); */ /* Configuration matches register reset state at startup. */

  /* LL_SPI_SetFIFOThreshold(SPI3, LL_SPI_FIFO_THRESHOLD_1_DATA); */ /* Configuration matches register reset state at startup. */

  /* ### SPI3 GPIO Configuration ########################### */

  /* GPIO Clocks activation */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);

  /**
    [GPIO Pin] ------> [Signal Name]

       PB0     ------>   SPI3_SCK
       PB1     ------>   SPI3_MISO
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_MEDIUM);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_1, LL_GPIO_SPEED_FREQ_MEDIUM);

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_0 | LL_GPIO_PIN_1, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_0, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_1, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_0, LL_GPIO_AF_9);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_1, LL_GPIO_AF_9);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_ALTERNATE);

  /* Enable the interrupt for SPI */
  NVIC_SetPriority(SPI3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(SPI3_IRQn);
return SPI3;
}

void mx_spi3_deinit(void)
{
  /* Disable the interrupt for SPI */
  NVIC_DisableIRQ(SPI3_IRQn);

  LL_SPI_Disable(SPI3);

  /* ### GPIO deinitialization of SPI3: SPI3_SCK,SPI3_MISO ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_0, LL_GPIO_AF_0);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_1, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_1, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_0 | LL_GPIO_PIN_1, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_0, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_1, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOB, LL_GPIO_PIN_0 | LL_GPIO_PIN_1, LL_GPIO_PIN_RESET);
}

/******************************************************************************/
/*          SPI3 global interrupt is managed directly in user code.           */
/******************************************************************************/
/* void SPI3_IRQHandler(void)
{
}
  */
