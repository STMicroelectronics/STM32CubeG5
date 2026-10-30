/**
  ******************************************************************************
  * file           : secure_nsclib.c
  * brief          : This file contains the non-secure callable APIs (secure world)
  *                  This is a set of non-secure callable applicative functions compiled in the secure context.
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
#include "main.h"
#include "secure_nsclib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/

/**
  * brief  Non-secure callable function template
  * param  param  uint32_t parameter
  * retval uint32_t
  */
CMSE_NS_ENTRY uint32_t func_template(uint32_t param)
{
  /* Write your non-secure callable code here */
  return param;
}
