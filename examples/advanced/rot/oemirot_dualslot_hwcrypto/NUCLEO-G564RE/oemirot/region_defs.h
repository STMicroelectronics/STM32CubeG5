/**
  * Copyright (c) 2017-2018 ARM Limited
  *
  * Copyright (c) 2018-2026 STMicroelectronics.
  * All rights reserved.
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *     http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  */

#ifndef REGION_DEFS_H
#define REGION_DEFS_H

#include "flash_layout.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*********************** Hardware constraints ********************************/

/*
 * OEMiRoT SRAM
 * SRAM1 is selected because ECC and Erase on reset capable.
 * The xxx_ASM are used in naked inline assembly, so they are defined as numeric literals (no U suffix).
 * In the C code, ROT_DATA_START and ROT_DATA_SIZE are defined as unsigned constants derived
 * from assembly-safe values.
 */
#define ROT_DATA_START_ASM          0x30000000
#define ROT_DATA_SIZE_ASM           0x00008000
/* SRAM usable at start of appli: SRAM1/2/3 */
#define SRAM_BASE                   (0x30000000U) /* SRAM1 base address */
#define SRAM_SIZE                   (0x0001C000U) /* contiguous SRAM size from SRAM1 */

/* FLASH base address */
#define ROM_BASE                    (FLASH_BASE) /* User FLASH base address */

/* System memory */
#define RO_AND_SYSTEM_MEM_BASE      (0x08FFF800U)
#define RO_AND_SYSTEM_MEM_SIZE      (0x3000800U)

/* OBK */
#define OBK_HDPL0_SIZE              (FLASH_OBK_HDPL0_SIZE)
#define OBK_HDPL1_SIZE              (FLASH_OBK_HDPL1_SIZE)
#define OBK_HDPL2_SIZE              (FLASH_OBK_HDPL2_SIZE)
#define OBK_HDPL3S_SIZE             (FLASH_OBK_HDPL3S_SIZE)
#define OBK_HDPL3NS_SIZE            (FLASH_OBK_HDPL3NS_SIZE)

/* Peripherals */
/* #define PERIPH_BASE dfp definition is used */
#define PERIPH_SIZE                 (0x10000000U)

/*********************** Generic definitions *********************************/

/* Alias definitions for secure and non-secure ROM areas */
#define ROM_ALIAS_BASE              (ROM_BASE)
#define ROM_ALIAS(x)                (ROM_ALIAS_BASE + (x))

/* Alias for flash_map source code compatibility (in mw) */
#define FLASH_BASE_ADDRESS          (ROM_BASE)

/* RoT code region */
#define ROT_CODE_START              (ROM_ALIAS(FLASH_ROT_CODE_OFFSET))
#define ROT_CODE_SIZE               (FLASH_ROT_CODE_SIZE)
#define ROT_CODE_END                (ROT_CODE_START + ROT_CODE_SIZE - 1U)

/* RoT VTOR address */
#define ROT_VTOR_ADDR               ((uint32_t) ROT_CODE_START)

/* RoT slot areas regions */
#define ROT_AREAS_START             (ROM_ALIAS(FLASH_AREAS_START_OFFSET))
#define ROT_AREAS_END               (ROM_ALIAS(FLASH_AREAS_END_OFFSET))

/* SRAM region */
/* SRAM_ROT_xxxx definitions are aligned with RAM_ROT_xxxx definitions of the linker file. */
#define SRAM_ROT_BASE               (TO_UNSIGNED(ROT_DATA_START_ASM))
#define SRAM_ROT_SIZE               (TO_UNSIGNED(ROT_DATA_SIZE_ASM))
#define ROT_DATA_START              (SRAM_ROT_BASE)
#define ROT_DATA_SIZE               (SRAM_ROT_SIZE)
/*
 * BOOT_SHARED_DATA_xxxx definitions are not used
 * but should be defined for flash_map source code compatibility (in mw)
 */
#define BOOT_SHARED_DATA_SIZE       (0U)
#define BOOT_SHARED_DATA_BASE       (0U)

/* app regions */
#define CODE_START                  (ROM_ALIAS(APP_CODE_OFFSET))
#define CODE_SIZE                   (APP_CODE_SIZE)
#define CODE_END                    (CODE_START + CODE_SIZE - 1U)

/* MPU SRAM region */
#define SRAM_START                  (SRAM_BASE)
#define SRAM_END                    (SRAM_BASE + SRAM_SIZE - 1U)

/* MPU Peripherals region */
#define PERIPH_START                (PERIPH_BASE)
#define PERIPH_END                  (PERIPH_BASE + PERIPH_SIZE - 1U)

/* MPU RO and system memory region (for access to RSSLIB function pointers and device memory size) */
#define RO_AND_SYSTEM_MEM_START     (RO_AND_SYSTEM_MEM_BASE)
#define RO_AND_SYSTEM_MEM_END       (RO_AND_SYSTEM_MEM_BASE + RO_AND_SYSTEM_MEM_SIZE - 1U)

/* OBKeys */
#define OBK_HDPL0_OFFSET            (0x0U)
#define OBK_HDPL0_END               (OBK_HDPL0_OFFSET + OBK_HDPL0_SIZE - 1U)
#define OBK_HDPL1_OFFSET            (OBK_HDPL0_OFFSET + OBK_HDPL0_SIZE)
#define OBK_HDPL1_END               (OBK_HDPL1_OFFSET + OBK_HDPL1_SIZE - 1U)
#define OBK_HDPL2_OFFSET            (OBK_HDPL1_OFFSET + OBK_HDPL1_SIZE)
#define OBK_HDPL2_END               (OBK_HDPL2_OFFSET + OBK_HDPL2_SIZE - 1U)
#define OBK_HDPL3S_OFFSET           (OBK_HDPL2_OFFSET + OBK_HDPL2_SIZE)
#define OBK_HDPL3S_END              (OBK_HDPL3S_OFFSET + OBK_HDPL3S_SIZE - 1U)
#define OBK_HDPL3NS_OFFSET          (OBK_HDPL3S_OFFSET + OBK_HDPL3S_SIZE)
#define OBK_HDPL3NS_END             (OBK_HDPL3NS_OFFSET + OBK_HDPL3NS_SIZE - 1U)

/* MPU OBKeys region */
#define OBK_START                   (FLASH_OBK_BASE)
#define OBK_END                     (FLASH_OBK_BASE + FLASH_OBK_SIZE - 1U)
/* MPU RO memory region (for access to RSSLIB function pointers) */
#define RSSLIB_START                (RSSLIB_BASE)
#define RSSLIB_END                  (RSSLIB_BASE + RSSLIB_SIZE - 1U)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* REGION_DEFS_H */
