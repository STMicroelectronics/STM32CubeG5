/**
  ******************************************************************************
  * @file    boot_hal_hash_ref.c
  * @brief   This file contains hash ref service implementation
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
#include "boot_hal_hash_ref.h"
#include "low_level_obkeys.h"

#include "mx_hal_def.h"
#include "bootutil_priv.h"

static uint8_t ImageValidHashUpdate = 0U;
static uint8_t ImageValidHashRef[MCUBOOT_IMAGE_NUMBER * IMAGE_SHA_LENGTH] = {0U};

/* exported variables --------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* exported functions --------------------------------------------------------*/
/**
  * @brief This function load all hash references from flash.
  * @return 0 on success; nonzero on failure.
  */
int32_t boot_hash_ref_load(void)
{
  obk_hdpl1_data_t OBK_hdpl1_data = { 0U };

  /* Read all OBK hdpl1 data & control SHA256 */
  if (OBK_ReadHdpl1Data(&OBK_hdpl1_data) != HAL_OK)
  {
    return BOOT_EFLASH;
  }

  /* update hash references */
  memcpy(ImageValidHashRef, &OBK_hdpl1_data.Image0SHA[0], (IMAGE_SHA_LENGTH * MCUBOOT_IMAGE_NUMBER));

  return 0;
}

/**
  * @brief This function store all hash references in flash.
  * @return 0 on success; nonzero on failure.
  */
int32_t boot_hash_ref_store(void)
{
  obk_hdpl1_data_t OBK_hdpl1_data = { 0U };
  if (ImageValidHashUpdate != 0)
  {
    /* Read all OBK hdpl1/2 data & control SHA256 */
    if (OBK_ReadHdpl1Data(&OBK_hdpl1_data) !=  HAL_OK)
    {
      return BOOT_EFLASH;
    }

    /* update hash references */
    memcpy(&OBK_hdpl1_data.Image0SHA[0], ImageValidHashRef, (IMAGE_SHA_LENGTH * MCUBOOT_IMAGE_NUMBER));

    /* Update all OBK hdpl1 data with associated hash references */
    if (OBK_UpdateHdpl1Data(&OBK_hdpl1_data) != HAL_OK)
    {
      return BOOT_EFLASH;
    }
  }

  return 0;
}

/**
  * @brief This function set one hash reference in ram.
  * @param hash_ref hash reference to update
  * @param size size of the hash references
  * @param image_index index of image corresponding to hash reference
  * @return 0 on success; nonzero on failure.
  */
int32_t boot_hash_ref_set(uint8_t *hash_ref, uint8_t size, uint8_t image_index)
{
  /* Check size */
  if (size != IMAGE_SHA_LENGTH)
  {
    return BOOT_EFLASH;
  }

  /* Check image index */
  if (image_index >= MCUBOOT_IMAGE_NUMBER)
  {
    return BOOT_EFLASH;
  }

  /* Set hash reference */
  (void)memcpy(ImageValidHashRef + (image_index * IMAGE_SHA_LENGTH), hash_ref, IMAGE_SHA_LENGTH);

  /* Memorize that hash references will have to be updated in flash (later) */
  ImageValidHashUpdate++;

  return 0;
}

/**
  * @brief This function get one hash reference from ram.
  * @param hash_ref hash reference to get
  * @param size size of the hash reference
  * @param image_index index of image corresponding to hash reference
  * @return 0 on success; nonzero on failure.
  */
int32_t boot_hash_ref_get(uint8_t *hash_ref, uint8_t size, uint8_t image_index)
{
  /* Check size */
  if (size != IMAGE_SHA_LENGTH)
  {
    return BOOT_EFLASH;
  }

  /* Check image index */
  if (image_index >= MCUBOOT_IMAGE_NUMBER)
  {
    return BOOT_EFLASH;
  }

  /* Get hash reference */
  (void)memcpy(hash_ref, ImageValidHashRef + (image_index * IMAGE_SHA_LENGTH), IMAGE_SHA_LENGTH);

  return 0;
}
