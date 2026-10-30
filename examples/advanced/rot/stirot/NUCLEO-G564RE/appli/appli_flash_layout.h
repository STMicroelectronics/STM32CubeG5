/**
  ******************************************************************************
  * @file    appli_flash_layout.h
  * @brief   This file contains definitions of memory regions.
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

#ifndef MX_APPLI_FLASH_LAYOUT_H
#define MX_APPLI_FLASH_LAYOUT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************/
/* DEFINITIONS INHERITED FROM RoT CONFIGURATION.                              */
/* THEY ARE UPDATED AUTOMATICALLY BY APPLI PREBUILD: DO NOT MODIFY!           */
/******************************************************************************/

/* Installation mode */
#define INSTALLATION_MODE_OVERWRITE                       (0x1U)
#define INSTALLATION_MODE_SWAP                            (0x2U)
#define INSTALLATION_MODE                                 INSTALLATION_MODE_OVERWRITE

/* Data Images number
   0: no image
   1: data image */
#define DATA_IMAGE_NUMBER                                 (0x0U)

/* Data image primary slot offset */
#define FLASH_PRIMARY_DATA_SLOT_OFFSET                    (0x0U)

/* Data image primary slot size */
#define FLASH_PRIMARY_DATA_SLOT_SIZE                      (0x0U)

/* App image primary slot offset */
#define FLASH_PRIMARY_APP_SLOT_OFFSET                     (0x0U)

/* App image primary slot size */
#define FLASH_PRIMARY_APP_SLOT_SIZE                       (0x10000U)

/* App image secondary slot offset */
#define FLASH_SECONDARY_APP_SLOT_OFFSET                   (0x70000U)

/* App image secondary slot size */
#define FLASH_SECONDARY_APP_SLOT_SIZE                     (0x10000U)

/* Data image secondary slot offset */
#define FLASH_SECONDARY_DATA_SLOT_OFFSET                  (0x0U)

/* Data image secondary slot size */
#define FLASH_SECONDARY_DATA_SLOT_SIZE                    (0x0U)

/* App image header size */
#define APP_IMAGE_HEADER_SIZE                             (0x400U)

/* Data image header size */
#define DATA_IMAGE_HEADER_SIZE                            (0x20U)

/* App image secondary magic install request offset */
#define FLASH_SECONDARY_APP_INSTALL_REQ_OFFSET            (0x7FFF0U)

/* Data image secondary magic install request offset */
#define FLASH_SECONDARY_DATA_INSTALL_REQ_OFFSET           (0x0U)

#if (INSTALLATION_MODE == INSTALLATION_MODE_SWAP)
/* App image primary confirmation flag offset */
#define FLASH_PRIMARY_APP_CONFIRM_OFFSET                  (0x8BFE0U)

/* Data image primary confirmation flag offset */
#define FLASH_PRIMARY_DATA_CONFIRM_OFFSET                 (0x0U)

#endif /* (INSTALLATION_MODE == INSTALLATION_MODE_SWAP) */

/* App image primary version offset */
#define FLASH_PRIMARY_APP_VERSION_OFFSET                  (0x14U)

/* Data image primary version offset */
#define FLASH_PRIMARY_DATA_VERSION_OFFSET                 (0x0U)

/* Flash Driver Used to Confirm Image(s) */
#define FLASH_PRIMARY_DEV_NAME                            Driver_FLASH0

/* Flash Driver Used to Download Image(s) */
#define FLASH_SECONDARY_DEV_NAME                          Driver_FLASH0

/******************************************************************************/
/* DEFINITIONS HW RELATED                                                     */
/******************************************************************************/

/* Flash total size */
#define FLASH_TOTAL_SIZE                                  (0x80000U)

/* Flash programmation granularity */
#define FLASH_PROG_SIZE                                   (0x10U)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_APPLI_FLASH_LAYOUT_H */
