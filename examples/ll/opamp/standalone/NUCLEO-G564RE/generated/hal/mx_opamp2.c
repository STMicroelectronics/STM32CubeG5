/**
  ******************************************************************************
  * @file           : mx_opamp2.c
  * @brief          : OPAMP2 Peripheral initialization
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
#include "mx_opamp2.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/******************************************************************************/
/* Exported functions for OPAMP2 in LL layer */
/******************************************************************************/
OPAMP_TypeDef *mx_opamp2_init(void)
{
  LL_APB3_GRP1_EnableClock(LL_APB3_GRP1_PERIPH_OPAMP2);

  /****************************************************************************/
  /* Configuration of basic features (mandatory)                              */
  /****************************************************************************/

  LL_OPAMP_SetConfig(OPAMP2, (LL_OPAMP_MODE_FUNCTIONAL
                              | LL_OPAMP_SPEED_MODE_HIGH
                              | LL_OPAMP_MODE_STANDALONE
                              | LL_OPAMP_INPUT_NONINVERT_IO0
                              | LL_OPAMP_INPUT_INVERT_IO0
                              | LL_OPAMP_OUTPUT_CONNECT_EXTERNAL)
                    );

  /* ### OPAMP2 GPIO Configuration ########################### */

  /* GPIO Clocks activation */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);

  /**
    [GPIO Pin] ------> [Signal Name]

       PA6     ------>   OPAMP2_VOUT
       PA5     ------>   OPAMP2_VINM0
       PA7     ------>   OPAMP2_VINP0
    **/

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_6, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_5, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_7, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  /* LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_ANALOG); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_ANALOG); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_ANALOG); */ /* Configuration matches register reset state at startup. */

return OPAMP2;
}

void mx_opamp2_deinit(void)
{
  LL_OPAMP_Disable(OPAMP2);

  LL_APB3_GRP1_ForceReset(LL_APB3_GRP1_PERIPH_OPAMP2);
  LL_APB3_GRP1_ReleaseReset(LL_APB3_GRP1_PERIPH_OPAMP2);

  LL_APB3_GRP1_DisableClock(LL_APB3_GRP1_PERIPH_OPAMP2);

  /* ### GPIO deinitialization of OPAMP2: OPAMP2_VOUT,OPAMP2_VINM0,OPAMP2_VINP0 ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_6, LL_GPIO_AF_0);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_5, LL_GPIO_AF_0);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_7, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_6 | LL_GPIO_PIN_5 | LL_GPIO_PIN_7, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_6, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_5, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_7, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOA, LL_GPIO_PIN_6 | LL_GPIO_PIN_5 | LL_GPIO_PIN_7, LL_GPIO_PIN_RESET);
}
