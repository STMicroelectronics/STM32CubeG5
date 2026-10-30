/**
  ******************************************************************************
  * @file    low_level_obkeys.c
  * @author  MCD Application Team
  * @brief   Low Level Interface module to access OKB area in FLASH
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "low_level_obkeys.h"
#include "Driver_Common.h"
#include "mx_hal_def.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MAX_SIZE_OBK                    (FLASH_OBK_HDPL2_SIZE)
#define SAES_TIMEOUT_VALUE              100U

uint32_t SAESIV[4] =
{
  0x8001D1CEU, 0xD1CED1CEU, 0xD1CE8001U, 0xCED1CED1U
};

/* Private variables ---------------------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Read encrypted OBkeys.
  * @param  offset Offset in the OBKeys area (aligned on 16 bytes)
  * @param  p_data Data buffer to be filled (aligned on 4 bytes)
  * @param  length Number of bytes (multiple of 4 bytes)
  * @retval ARM_DRIVER error status
  */
int32_t OBK_Read(uint32_t offset, void *p_data, uint32_t length)
{
  hal_aes_handle_t *p_hsaes = { 0U };
  uint32_t data_encrypted[MAX_SIZE_OBK / 4U] = { 0UL };
  uint8_t *p_destination = (uint8_t *) data_encrypted;
  uint8_t *p_source = (uint8_t *)(FLASH_OBK_HDPL2_BASE_S + offset);

  /* Check parameters */
  if (length > MAX_SIZE_OBK)
  {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  memcpy(p_destination, p_source, length);

  /* Force use of EPOCH_S value for DHUK */
  HAL_SBS_SetEPOCHSelection(HAL_SBS_EPOCH_SEC);

  /* Get SAES handle */
  p_hsaes = mx_example_saes_gethandle();
  if (p_hsaes == NULL)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }

  /* Configure SAES */
  if (OBK_ConfigureSaes(p_hsaes) != ARM_DRIVER_OK)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }

  /* Size is n words */
  if (HAL_AES_Decrypt(p_hsaes, (const void *)&data_encrypted[0U], (uint16_t)length, p_data,
                      SAES_TIMEOUT_VALUE) != HAL_OK)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }

  return ARM_DRIVER_OK;
}

/**
  * @brief  Configure SAES.
  * @param  p_hsaes SAES handle
  * @retval ARM_DRIVER error status
  */
int32_t OBK_ConfigureSaes(hal_aes_handle_t *p_hsaes)
{
  if (HAL_AES_CBC_SetConfig(p_hsaes, SAESIV) != HAL_OK)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }

  HAL_AES_SetHWKey(p_hsaes, HAL_AES_KEY_SIZE_256BIT, HAL_AES_KEY_SELECT_DHUK, HAL_AES_KEY_MODE_NORMAL);
  HAL_AES_SetDataSwapping(p_hsaes, HAL_AES_DATA_SWAPPING_NO);

  return ARM_DRIVER_OK;
}
