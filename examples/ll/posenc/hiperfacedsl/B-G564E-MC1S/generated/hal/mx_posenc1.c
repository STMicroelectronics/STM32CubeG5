/**
  ******************************************************************************
  * @file           : mx_posenc1.c
  * @brief          : POSENC1 Peripheral initialization
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
#include "mx_posenc1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
/******************************************************************************/
/* Exported functions for POSENC1 in LL layer */
/******************************************************************************/
POSENC_TypeDef *mx_posenc1_init(void)
{
  LL_APB3_GRP1_EnableClock(LL_APB3_GRP1_PERIPH_POSENC1);
  LL_RCC_SetPOSENCClockSource(LL_RCC_POSENC1_CLKSOURCE_PLL2P);

  /****************************************************************************/
  /* Initialization of POSENC instance                                        */
  /****************************************************************************/

  LL_POSENC_ConfigEncoder(POSENC1, LL_POSENC_STROBE_RESYNC_DISABLED, LL_POSENC_ENCSEL_HIPERFACEDSL, LL_POSENC_STROBE_TIM1_TRGO);

  /****************************************************************************/
  /* Initialization of HiperfaceDSL feature                                   */
  /****************************************************************************/

  /* LL_POSENC_HD_SetEventDrivenFastPosition(POSENC1, LL_POSENC_HD_EVTDRV_FASTPOSITION_DISABLED); */ /* Configuration matches register reset state at startup. */
  /* LL_POSENC_HD_SetEventDrivenSafePosition_1(POSENC1, LL_POSENC_HD_EVTDRV_SAFE_POSITION_1_DISABLED); */ /* Configuration matches register reset state at startup. */

  /****************************************************************************/
  /* Configuration of GPIO                                                    */
  /****************************************************************************/
  /* ### POSENC1 GPIO Configuration ########################### */

  /* GPIO Clocks activation */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOF);

  /**
    [GPIO Pin] ------> [Signal Name] ------> [Labels]

       PF14    ------>   POSENC1_EN   ------>  DIR
       PF12    ------>   POSENC1_IN   ------>  RX
       PF11    ------>   POSENC1_OUT   ------>  TX
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  LL_GPIO_SetPinSpeed(DIR_PORT, DIR_PIN, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinSpeed(RX_PORT, RX_PIN, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinSpeed(TX_PORT, TX_PIN, LL_GPIO_SPEED_FREQ_VERY_HIGH);

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(GPIOF, DIR_PIN | RX_PIN | TX_PIN, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(DIR_PORT, DIR_PIN, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinPull(RX_PORT, RX_PIN, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinPull(TX_PORT, TX_PIN, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(DIR_PORT, DIR_PIN, LL_GPIO_AF_6);
  LL_GPIO_SetAFPin_8_15(RX_PORT, RX_PIN, LL_GPIO_AF_6);
  LL_GPIO_SetAFPin_8_15(TX_PORT, TX_PIN, LL_GPIO_AF_6);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  LL_GPIO_SetPinMode(DIR_PORT, DIR_PIN, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinMode(RX_PORT, RX_PIN, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinMode(TX_PORT, TX_PIN, LL_GPIO_MODE_ALTERNATE);

  /* Enable the interruption for POSENC */
  NVIC_SetPriority(POSENC1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(POSENC1_IRQn);

  return POSENC1;
}

void mx_posenc1_deinit(void)
{
  LL_APB3_GRP1_DisableClock(LL_APB3_GRP1_PERIPH_POSENC1);

  LL_APB3_GRP1_ForceReset(LL_APB3_GRP1_PERIPH_POSENC1);
  LL_APB3_GRP1_ReleaseReset(LL_APB3_GRP1_PERIPH_POSENC1);

  /* ### GPIO deinitialization of POSENC1: POSENC1_EN,POSENC1_IN,POSENC1_OUT ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(DIR_PORT, DIR_PIN, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinMode(RX_PORT, RX_PIN, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinMode(TX_PORT, TX_PIN, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_8_15(DIR_PORT, DIR_PIN, LL_GPIO_AF_0);
  LL_GPIO_SetAFPin_8_15(RX_PORT, RX_PIN, LL_GPIO_AF_0);
  LL_GPIO_SetAFPin_8_15(TX_PORT, TX_PIN, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(DIR_PORT, DIR_PIN, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinSpeed(RX_PORT, RX_PIN, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinSpeed(TX_PORT, TX_PIN, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOF, DIR_PIN | RX_PIN | TX_PIN, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(DIR_PORT, DIR_PIN, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinPull(RX_PORT, RX_PIN, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinPull(TX_PORT, TX_PIN, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOF, DIR_PIN | RX_PIN | TX_PIN, LL_GPIO_PIN_RESET);

  NVIC_DisableIRQ(POSENC1_IRQn);
}

/******************************************************************************/
/*         POSENC1 global interrupt is managed directly in user code.         */
/******************************************************************************/
/* void POSENC1_IRQHandler(void)
{
}
  */
