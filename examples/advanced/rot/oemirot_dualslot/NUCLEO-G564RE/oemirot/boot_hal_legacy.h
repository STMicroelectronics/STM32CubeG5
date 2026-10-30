/**
  ******************************************************************************
  * @file    boot_hal_legacy.h
  * @brief   Header for providing HAL API compatibility wrappers
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BOOT_HAL_LEGACY_H
#define BOOT_HAL_LEGACY_H

#include "boot_hal_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Exported defines ------------------------------------------------------------------*/
/* Keep OBK HAL calls compatible across platforms. */
#define HAL_FLASH_ITF_OBK_SetPage(instance, obk_area, obk_page) \
  (ARG_UNUSED(obk_area), HAL_FLASH_ITF_OBK_SetPage((instance), (obk_page)))
#define HAL_FLASH_ITF_OBK_SwapPage(instance, obk_area, swap_offset) \
  (ARG_UNUSED(obk_area), HAL_FLASH_ITF_OBK_SwapPage((instance), (swap_offset)))
#define HAL_FLASH_ITF_OBK_SwapPage_IT(instance, obk_area, swap_offset) \
  (ARG_UNUSED(obk_area), HAL_FLASH_ITF_OBK_SwapPage_IT((instance), (swap_offset)))
#define HAL_FLASH_ITF_OBK_Lock(instance, obk_area) \
  (ARG_UNUSED(obk_area), HAL_FLASH_ITF_OBK_Lock(instance))
#define HAL_FLASH_ITF_OBK_Unlock(instance, obk_area) \
  (ARG_UNUSED(obk_area), HAL_FLASH_ITF_OBK_Unlock(instance))

#ifdef __cplusplus
}
#endif

#endif /* BOOT_HAL_LEGACY_H */
