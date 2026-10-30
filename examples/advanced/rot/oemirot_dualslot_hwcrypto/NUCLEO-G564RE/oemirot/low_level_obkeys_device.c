/**
  ******************************************************************************
  * @file    low_level_obkeys_device.c
  * @brief   This file contains device definition for low_level_obkeys
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "region_defs.h"
#include "low_level_obkeys.h"

obk_flash_range_t erase_vect[] =
{
  {
    OBK_HDPL1_OFFSET + OBK_HDPL1_DATA_OFFSET,
    OBK_HDPL1_OFFSET + OBK_HDPL1_DATA_OFFSET + OBK_HDPL1_DATA_SIZE - 1U
  }
};

obk_flash_range_t write_vect[] =
{
  {
    OBK_HDPL1_OFFSET + OBK_HDPL1_DATA_OFFSET,
    OBK_HDPL1_OFFSET + OBK_HDPL1_DATA_OFFSET + OBK_HDPL1_DATA_SIZE - 1U
  }
};

obk_low_level_device_t OBK_FLASH0_DEV =
{
  .erase = { .nb = sizeof(erase_vect) / sizeof(obk_flash_range_t), .range = erase_vect},
  .write = { .nb = sizeof(write_vect) / sizeof(obk_flash_range_t), .range = write_vect},
  .read_error = 1
};
