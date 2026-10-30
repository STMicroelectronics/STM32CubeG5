/**
  ******************************************************************************
  * @file    low_level_obkeys.h
  * @brief   Header for low_level_obkeys.c module
  *
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LOW_LEVEL_OBKEYS_H
#define LOW_LEVEL_OBKEYS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mx_hal_def.h"
#include "mcuboot_config.h"
#include "flash_layout.h"
#include "boot_hal_hash_ref.h"

#include "plat_nv_counters.h"
#include "Driver_Flash.h"

/* Exported constants --------------------------------------------------------*/
#define OBK_FLASH_PROG_UNIT           (0x10U)

#define AUTH_PUB_KEY_HASH_LENGTH      SHA384_LENGTH
#define ENC_PRIV_KEY_LENGTH           32U

#define RESERVED1                     ((4U - (AUTH_PUB_KEY_HASH_LENGTH % 4U)) % 4U)
#define RESERVED2                     ((4U - (AUTH_PUB_KEY_HASH_LENGTH % 4U)) % 4U)
#define RESERVED3                     ((4U - (ENC_PRIV_KEY_LENGTH % 4U)) % 4U)
#define RESERVED4                     ((OBK_FLASH_PROG_UNIT -\
                                        ((AUTH_PUB_KEY_HASH_LENGTH + RESERVED1 +\
                                          AUTH_PUB_KEY_HASH_LENGTH + RESERVED2 +\
                                          ENC_PRIV_KEY_LENGTH + RESERVED3) % OBK_FLASH_PROG_UNIT))\
                                       % OBK_FLASH_PROG_UNIT)

/* OBK layout ----------------------------------------------------------------*/
/* Size reserved at start of OBKey HDPL1 for RSS DA */
#define OBK_HDPL1_RES_SIZE            (0x200U)
/* First OBKey HDPL1 for immutable config */
#define OBK_HDPL1_CFG_OFFSET          (OBK_HDPL1_OFFSET + OBK_HDPL1_RES_SIZE)

/* Size for OBKey HDPL1 config section */
#define OBK_HDPL1_CFG_SIZE            (sizeof(obk_hdpl1_config_t))
/* First OBKey HDPL1 for data section */
#define OBK_HDPL1_DATA_OFFSET         (OBK_HDPL1_CFG_OFFSET + OBK_HDPL1_CFG_SIZE)
/* Size for OBKey HDPL1 data section */
#define OBK_HDPL1_DATA_SIZE           (sizeof(obk_hdpl1_data_t))

/* Exported types ------------------------------------------------------------*/

/* Hdpl 1 immutable section : to be aligned on FLASH_PROG_UNIT (16 bytes)
   ====================================================================== */
typedef struct
{
  uint8_t  SHA256[SHA256_LENGTH]; /* Mandatory to be first in the structure */
  __attribute__((__aligned__(4))) uint8_t Hdpl3SecureAuthenticationPubKeyHash[AUTH_PUB_KEY_HASH_LENGTH];
  __attribute__((__aligned__(4))) uint8_t Hdpl3NonSecureAuthenticationPubKeyHash[AUTH_PUB_KEY_HASH_LENGTH];
  __attribute__((__aligned__(4))) uint8_t Hdpl3EncryptionPrivKey[ENC_PRIV_KEY_LENGTH];
  uint8_t  Reserved4[RESERVED4]; /* Alignment on 16 bytes */
} obk_hdpl1_config_t;

/* Hdpl 1 data section : to be aligned on FLASH_PROG_UNIT (16 bytes)
   ================================================================= */
typedef struct
{
  uint8_t  SHA256[SHA256_LENGTH]; /* Mandatory to be first in the structure */
  uint32_t Image0CurVersion;
  uint32_t Image0PrevVersion;
  uint32_t Image1CurVersion;
  uint32_t Image1PrevVersion;
  uint32_t Image2CurVersion;
  uint32_t Image2PrevVersion;
  uint32_t Image3CurVersion;
  uint32_t Image3PrevVersion;
  uint8_t  Image0SHA[IMAGE_SHA_LENGTH];
  uint8_t  Image1SHA[IMAGE_SHA_LENGTH];
  uint8_t  Image2SHA[IMAGE_SHA_LENGTH];
  uint8_t  Image3SHA[IMAGE_SHA_LENGTH];
  uint8_t  Reserved[0]; /* Alignment on 16 bytes */
} obk_hdpl1_data_t;

/* Driver configuration
   ==================== */
typedef struct
{
  uint32_t base;
  uint32_t limit;
} obk_flash_range_t;

typedef struct
{
  uint32_t nb;
  obk_flash_range_t *range;
} obk_flash_vect_t;

typedef struct
{
  obk_flash_vect_t erase;
  obk_flash_vect_t write;
  uint32_t read_error;
} obk_low_level_device_t;

/* External variables --------------------------------------------------------*/
extern obk_low_level_device_t OBK_FLASH0_DEV;
extern ARM_DRIVER_FLASH OBK_Driver_FLASH0;
extern obk_hdpl1_config_t OBK_Hdpl1Cfg;

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void OBK_ReadHdpl1Config(obk_hdpl1_config_t *p_obk_hdpl1_cfg);
hal_status_t OBK_ReadHdpl1Data(obk_hdpl1_data_t *p_obk_hdpl1_data);
hal_status_t OBK_UpdateHdpl1Data(obk_hdpl1_data_t *p_obk_hdpl1_data);
void OBK_VerifyHdpl1Config(obk_hdpl1_config_t *p_obk_hdpl1_cfg);

hal_status_t OBK_UpdateNVCounter(enum nv_counter_t counter_id, uint32_t counter);
hal_status_t OBK_GetNVCounter(enum nv_counter_t counter_id, uint32_t *p_counter);

#ifdef __cplusplus
}
#endif

#endif /* LOW_LEVEL_OBKEYS_H */
