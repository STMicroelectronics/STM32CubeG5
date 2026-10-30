/**
  ******************************************************************************
  * @file           : secure_nsclib.c
  * @brief          : This file contains the non-secure callable APIs (secure world)
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
static const uint8_t Password[] = "I love STM32";

/**
  * @brief  Compare a password stored in non-secure memory with the secure password.
  * @param  password_received Pointer to the received password.
  * @param  password_size_byte Received password size in byte.
  * @retval 1 when the password matches, otherwise 0.
  */
CMSE_NS_ENTRY uint32_t IsPasswordCorrect(const uint8_t *password_received, uint32_t password_size_byte)
{
  const uint8_t *checked_password;
  uint32_t index;

  if ((password_size_byte != (sizeof(Password) - 1U)) && (password_size_byte != sizeof(Password)))
  {
    return 0U;
  }

  checked_password = cmse_check_address_range((void *)password_received,
                                              password_size_byte,
                                              CMSE_NONSECURE);
  if (checked_password == NULL)
  {
    return 0U;
  }

  if (password_size_byte == sizeof(Password))
  {
    if (checked_password[password_size_byte - 1U] != (uint8_t)'\r')
    {
      return 0U;
    }
    password_size_byte--;
  }

  for (index = 0U; index < password_size_byte; index++)
  {
    if (checked_password[index] != Password[index])
    {
      return 0U;
    }
  }

  return 1U;
}
