/**
  ******************************************************************************
  * @file           : mx_filex_interfaces.h
  * @brief          : FileX applicative file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_filex_license.md file
  * in the same directory as the generated code.
  * If no mx_filex_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MX_FILEX_INTERFACES_H__
#define __MX_FILEX_INTERFACES_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mx_levelx_interfaces.h"
#include "stm32_fx_nand_flash_driver.h"

/* Exported types ------------------------------------------------------------*/

typedef struct MX_FILEX_MEDIA_FORMAT_CONFIG_STRUCT {
  UINT  number_of_fats;
  UINT  directory_entries;
  UINT  hidden_sectors;
  ULONG total_sectors;
  UINT  bytes_per_sector;
  ULONG sectors_per_cluster;
  UINT  heads;
  UINT  sectors_per_track;
} MX_FILEX_MEDIA_FORMAT_CONFIG;

/* Exported macro ------------------------------------------------------------*/
#define mx_filex_driver_0  stm32_fx_nand_flash_driver
#define volume_name_0 "STM32_VOLUME"


/* Exported functions prototypes ---------------------------------------------*/
VOID *mx_filex_driver_0_get_ctx(VOID);
MX_FILEX_MEDIA_FORMAT_CONFIG *mx_filex_driver_0_get_media_format_config(VOID);

#ifdef __cplusplus
}
#endif

#endif /* __MX_FILEX_INTERFACES_H__ */
