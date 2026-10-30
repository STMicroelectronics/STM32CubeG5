/**
  ******************************************************************************
  * file           : stm32g5xx_ll_example.c
  * brief          : PLAY XOR gate source with LL API
  ******************************************************************************
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ll_example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** brief:  Locks the MX_PLAYx module.
  */
void PLAY_Lock(void)
{
  LL_PLAY_Lock(MX_PLAYx);
}


/** brief:  Toggles the MX_PLAYx Software Trigger 0 input.
  */
void PLAY_ToggleSWTrigger(void)
{
  /* Toggle Software Trigger 0 */
  LL_PLAY_ToggleSWTrigger(MX_PLAYx, LL_PLAY_SWTRIG0);

  while (LL_PLAY_IsSWTriggerWriteBusy(MX_PLAYx) != 0U);
}