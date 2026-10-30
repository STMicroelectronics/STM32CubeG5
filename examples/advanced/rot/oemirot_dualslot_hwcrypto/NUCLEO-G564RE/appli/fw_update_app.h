/**
  ******************************************************************************
  * @file    fw_update_app.h
  * @brief   This file contains definitions for Firmware Update module
  *          functionalities.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef FW_UPDATE_APP_H
#define FW_UPDATE_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "appli_flash_layout.h"

void FW_UPDATE_Run(void);

typedef struct
{
  uint32_t  MaxSizeInBytes;        /*!< The maximum allowed size for the FwImage in User Flash (in Bytes) */
  uint32_t  DownloadAddr;          /*!< The download address for the FwImage in UserFlash */
  uint32_t  ImageOffsetInBytes;    /*!< Image write starts at this offset */
  uint32_t  ExecutionAddr;         /*!< The execution address for the FwImage in UserFlash */
} sfu_fw_image_flash_t;

/* Menu entry numbers. Each entry is numbered by auto-incrementing from the */
/* previous one, without referencing its name. The set of available entries */
/* depends conditions. */
typedef enum
{
  MENU_NUM_DOWNLOAD_APP_IMAGE = 1,
  MENU_NUM_REQUEST_APP_IMAGE_INSTALL,

#if (INSTALLATION_MODE == INSTALLATION_MODE_SWAP)
  MENU_NUM_VALIDATE_APP_IMAGE,
#endif /* (INSTALLATION_MODE == INSTALLATION_MODE_SWAP) */

#if (DATA_IMAGE_NUMBER == 1)
  MENU_NUM_DOWNLOAD_DATA_IMAGE,
  MENU_NUM_REQUEST_DATA_IMAGE_INSTALL,

#if (INSTALLATION_MODE == INSTALLATION_MODE_SWAP)
  MENU_NUM_VALIDATE_DATA_IMAGE,
#endif /* (INSTALLATION_MODE == INSTALLATION_MODE_SWAP) */

#endif /* (DATA_IMAGE_NUMBER == 1) */

  MENU_NUM_RESET_INSTALLATION,
} FW_UPDATE_MenuEntryNum_t;

#ifdef __cplusplus
}
#endif

#endif /* FW_UPDATE_APP_H */

