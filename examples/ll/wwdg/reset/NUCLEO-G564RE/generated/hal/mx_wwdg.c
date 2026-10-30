/**
  ******************************************************************************
  * @file           : mx_wwdg.c
  * @brief          : WWDG Peripheral initialization
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
#include "mx_wwdg.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
/********************************************/
/* Exported functions for WWDG in LL layer */
/********************************************/
WWDG_TypeDef *mx_wwdg_init(void)
{
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_WWDG);

  return WWDG;
}

system_status_t mx_wwdg_start(void)
{
  /* Enable the WWDG driver and set the reload value to work with */
  LL_WWDG_SetControl(WWDG, 127UL);

  /* Write to WWDG CFR register the Prescaler and Window values to work with */
  LL_WWDG_SetConfig(WWDG, LL_WWDG_PRESCALER_128, 127UL);

  return SYSTEM_OK;
}
