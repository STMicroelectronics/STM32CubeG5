/**
  ******************************************************************************
  * @file    low_level_obkeys.c
  * @brief   Low Level Interface module to access OBK area in FLASH
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

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "low_level_obkeys.h"
#include "low_level_flash.h"
#include "low_level_security.h"
#include "boot_hal_flowcontrol.h"
#include "boot_hal_cfg.h"
#include "flash_layout.h"
#include "region_defs.h"
#include "boot_hal_utils.h"
#include "boot_hal_legacy.h"

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls_config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif /* !MBEDTLS_CONFIG_FILE */

/* External functions --------------------------------------------------------*/
extern void Error_Handler(void);

/* Private typedef -----------------------------------------------------------*/

/**
  * Arm Flash device structure.
  */
struct arm_obk_flash_dev_s
{
  obk_low_level_device_t *dev;
  ARM_FLASH_INFO *data;       /*!< OBK FLASH memory device data */
};

/* Private defines -----------------------------------------------------------*/
#define ST_SHA256_TIMEOUT         (3U)
#define FLASH_TIMEOUT_VALUE       (1000U)

#define MAX_SIZE_OBK              ((OBK_HDPL1_CFG_SIZE) > (OBK_HDPL1_DATA_SIZE) ? \
                                   (OBK_HDPL1_CFG_SIZE) : (OBK_HDPL1_DATA_SIZE))

#define OBK_FLASH0_PROG_UNIT      (OBK_FLASH_PROG_UNIT)

/* Swap the complete obk area; HAL converts the byte size to a key index */
#define OBK_SWAP_OFFSET           (FLASH_OBK_HDPL0_SIZE + \
                                   FLASH_OBK_HDPL1_SIZE + \
                                   FLASH_OBK_HDPL2_SIZE + \
                                   FLASH_OBK_HDPL3_SIZE)

/* Allows reading back after an OBK programming */
/* #define CHECK_PROGRAM */
/* Private variables ---------------------------------------------------------*/
static ARM_FLASH_INFO ARM_OBK_FLASH0_DEV_DATA =
{
  .sector_info    = NULL, /* unused */
  .sector_count   = 0U, /* unused */
  .sector_size    = 0U, /* unused */
  .page_size      = 0U, /* unused */
  .program_unit   = OBK_FLASH0_PROG_UNIT, /* Minimum write size in bytes */
  .erased_value   = 0U /* unused*/
};

static struct arm_obk_flash_dev_s ARM_OBK_FLASH0_DEV =
{
  .dev    = &(OBK_FLASH0_DEV),
  .data   = &(ARM_OBK_FLASH0_DEV_DATA)
};

/* Global variables ----------------------------------------------------------*/
#if defined(__ICCARM__)
#pragma location=".ram_init_ro"
#elif defined(__GNUC__)
__attribute__((section(".ram_init_ro")))
#endif /* __ICCARM__ */

obk_hdpl1_config_t OBK_Hdpl1Cfg;

/* Private function prototypes -----------------------------------------------*/
static bool is_range_valid(struct arm_obk_flash_dev_s *flash_dev, uint32_t offset);
static bool is_write_aligned(struct arm_obk_flash_dev_s *flash_dev, uint32_t param);
static bool is_write_allowed(struct arm_obk_flash_dev_s *flash_dev, uint32_t length);
static int32_t OBK_Read(uint32_t offset, void *p_data, uint32_t length);
static int32_t OBK_Write(uint32_t offset, const void *p_data, uint32_t length);
static uint32_t MemoryCompare(const uint8_t *p_add1, const uint8_t *p_add2, uint32_t size);
static hal_status_t ComputeSHA256(uint8_t *p_buffer, uint32_t length, uint8_t *p_sha256);
#if defined(CHECK_PROGRAM)
static bool MemoryIsErased(const uint8_t *p_add, uint32_t size);
#endif /* CHECK_PROGRAM */

/* Function Definitions ------------------------------------------------------*/
/**
  * @brief      Check if the Flash memory boundaries are not violated.
  * @param      flash_dev  Flash device structure \ref arm_obk_flash_dev_s
  * @param      offset     Highest Flash memory address which would be accessed.
  * @return     Returns true if Flash memory boundaries are not violated, false
  *             otherwise.
  */
static bool is_range_valid(struct arm_obk_flash_dev_s *flash_dev, uint32_t offset)
{
  ARG_UNUSED(flash_dev);

  return (offset <= OBK_HDPL1_END) ? (true) : (false);
}

/**
  * @brief  Check if the parameter is aligned to program_unit.
  * @param  flash_dev  Flash device structure \ref arm_obk_flash_dev_s
  * @param  param Any number that can be checked against the
  *               program_unit, e.g. Flash memory address or
  *               data length in bytes.
  * @retval Returns true if param is aligned to program_unit, false
  *         otherwise.
  */
static bool is_write_aligned(struct arm_obk_flash_dev_s *flash_dev, uint32_t param)
{
  return ((param % flash_dev->data->program_unit) != 0U) ? (false) : (true);
}

/**
  * @brief  Control if the length is a multiple of program_unit.
  * @param  flash_dev  Flash device structure \ref arm_obk_flash_dev_s
  * @param  length Number of bytes (multiple of program_unit)
  * @retval Returns true if length is a multiple of program_unit, false
  *         otherwise.
  */
static bool is_write_allowed(struct arm_obk_flash_dev_s *flash_dev, uint32_t length)
{
  return ((length % flash_dev->data->program_unit) != 0U) ? (false) : (true);
}

/**
  * @brief  Memory compare with constant time execution.
  * @param  p_add1 Address of the first buffer to compare
  * @param  p_add2 Address of the second buffer to compare
  * @param  size Size of the comparison
  * @note   Objective is to avoid basic attacks based on time execution
  * @retval 0 if equal, another value otherwise.
  */
static uint32_t MemoryCompare(const uint8_t *p_add1, const uint8_t *p_add2, uint32_t size)
{
  uint8_t result = 0x00U;
  uint32_t i = 0U;

  for (i = 0U; i < size; i++)
  {
    result |= p_add1[i] ^ p_add2[i];
  }
  return result;
}

/**
  * @brief  Verify that the memory is erased.
  * @param  p_add Address of the buffer to verify
  * @param  size Size of the comparison
  * @retval true if erased, false otherwise.
  */
#if defined(CHECK_PROGRAM)
static bool MemoryIsErased(const uint8_t *p_add, uint32_t size)
{
  uint8_t erased_pattern = UINT8_MAX;
  uint32_t i = 0U;

  for (i = 0U; i < size; i++)
  {
    if (*(p_add + i) != erased_pattern)
    {
      return false;
    }
  }
  return true;
}
#endif /* CHECK_PROGRAM */

/**
  * @brief  Read non-encrypted OBkeys.
  * @param  offset Offset in the OBKeys area (aligned on 16 bytes)
  * @param  p_data Data buffer to be filled (aligned on 4 bytes)
  * @param  length Number of bytes (multiple of 4 bytes)
  * @retval ARM_DRIVER error status
  */
static int32_t OBK_Read(uint32_t offset, void *p_data, uint32_t length)
{
  uint8_t *p_destination = (uint8_t *) p_data;
  uint8_t *p_source = (uint8_t *)(FLASH_OBK_BASE_S + offset);

  /* Check parameters */
  if ((length == 0U)
      || (length > MAX_SIZE_OBK)
      || (offset > OBK_HDPL1_END)
      || ((length - 1U) > (OBK_HDPL1_END - offset)))
  {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (is_range_valid(&ARM_OBK_FLASH0_DEV, offset + length - 1U) != true)
  {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  memcpy(p_destination, p_source, length);

  return ARM_DRIVER_OK;
}

/**
  * @brief  Write non-encrypted OBkeys.
  * @param  offset Offset in the OBKeys area (aligned on 16 bytes)
  * @param  p_data Data buffer to be written non-encrypted (aligned on 4 bytes)
  * @param  length Number of bytes (multiple of 4 bytes)
  * @retval ARM_DRIVER error status
  */
static int32_t OBK_Write(uint32_t offset, const void *p_data, uint32_t length)
{
#if defined(CHECK_PROGRAM)
  uint8_t p_written_data[MAX_SIZE_OBK] = {0U};
#endif /* CHECK_PROGRAM */
  uint32_t data_address = (uint32_t) p_data;
  uint32_t destination  = FLASH_OBK_BASE + offset;
  hal_flash_obk_area_t obk_flash_area = HAL_FLASH_OBK_AREA_1;
  hal_flash_handle_t *p_hflash = mx_rot_flash_gethandle();
  hal_flash_t p_flash = HAL_FLASH_GetInstance(p_hflash);

  /* Check parameters */
  if ((is_range_valid(&ARM_OBK_FLASH0_DEV, offset + length - 1U) != true)
      || (is_write_aligned(&ARM_OBK_FLASH0_DEV, offset) != true)
      || (is_write_allowed(&ARM_OBK_FLASH0_DEV, length) != true)
      || (length > MAX_SIZE_OBK))
  {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  /* Unlock FLASH area */
  (void) HAL_FLASH_ITF_Unlock(p_flash);
  (void) HAL_FLASH_ITF_OBK_Unlock(p_flash, obk_flash_area);

  /* Force the programming mode */
  if (HAL_FLASH_SetProgrammingMode(p_hflash,
                                   HAL_FLASH_PROGRAM_QUADWORD) != HAL_OK)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }

  /* Erase OBKeys */
  if (HAL_FLASH_OBK_MassErase(p_hflash, obk_flash_area, FLASH_TIMEOUT_VALUE) != HAL_OK)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }

#if defined(CHECK_PROGRAM)
  /* Select the alternate page for reading */
  if (HAL_FLASH_ITF_OBK_SetPage(p_flash, obk_flash_area, HAL_FLASH_OBK_PAGE_ALTERNATE) != HAL_OK)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }

  /* Verify that the erased words are all 0xFFFFFFFF */
  if (MemoryIsErased((uint8_t *)destination, length) != true)
  {
    ROT_LOG_ERR("The FLASH OBK alternate page isn't erased");
    return ARM_DRIVER_ERROR_SPECIFIC;
  }
#endif /* CHECK_PROGRAM */

  /* Program OBKeys */
  if (HAL_FLASH_OBK_ProgramByAddr(p_hflash,
                                  HAL_FLASH_OBK_PAGE_ALTERNATE,
                                  destination,
                                  (uint32_t const *)data_address,
                                  length,
                                  FLASH_TIMEOUT_VALUE) != HAL_OK)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }

  /* Swap all OBKeys */
  if (HAL_FLASH_ITF_OBK_SwapPage(p_flash, obk_flash_area, OBK_SWAP_OFFSET) != HAL_OK)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }

  /* Erase the alternate OBK page */
  if (HAL_FLASH_OBK_MassErase(p_hflash, obk_flash_area, FLASH_TIMEOUT_VALUE) != HAL_OK)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }

  if (HAL_FLASH_ITF_OBK_SetPage(p_flash, obk_flash_area, HAL_FLASH_OBK_PAGE_CURRENT) != HAL_OK)
  {
    return ARM_DRIVER_ERROR_SPECIFIC;
  }

#if defined(CHECK_PROGRAM)
  /* Verify that the programmed data matches the expected values */
  if (OBK_Read(offset, (void *)p_written_data, length) != ARM_DRIVER_OK)
  {
    ROT_LOG_ERR("The programmed FLASH OBK couldn't be read");
    return ARM_DRIVER_ERROR_SPECIFIC;
  }
  if (MemoryCompare((uint8_t *)destination, (uint8_t *)p_written_data, length) != 0U)
  {
    ROT_LOG_ERR("The FLASH OBK programming failed");
    return ARM_DRIVER_ERROR_SPECIFIC;
  }
#endif /* CHECK_PROGRAM */

  /* Lock the User Flash area */
  (void) HAL_FLASH_ITF_OBK_Lock(p_flash, obk_flash_area);
  (void) HAL_FLASH_ITF_Lock(p_flash);

  return ARM_DRIVER_OK;
}

/**
  * @brief  Compute SHA256.
  * @param  p_buffer pointer to the input buffer to be hashed
  * @param  length length of the input buffer in bytes
  * @param  p_sha256 pointer to the computed digest
  * @retval HAL error status
  */
static hal_status_t ComputeSHA256(uint8_t *p_buffer, uint32_t length, uint8_t *p_sha256)
{
  hal_hash_handle_t *p_hash;
  hal_hash_config_t hash_config;
  uint32_t computed_size = 0U;

  /* Get hash handle */
  p_hash = mx_rot_hash_gethandle();

  hash_config.algorithm = HAL_HASH_ALGO_SHA256;
  hash_config.data_swapping = HAL_HASH_DATA_SWAP_BYTE;
  if (HAL_HASH_SetConfig(p_hash, &hash_config) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* HASH computation */
  if (HAL_HASH_Compute(p_hash,
                       p_buffer,
                       length,
                       p_sha256,
                       SHA256_LENGTH,
                       &computed_size,
                       ST_SHA256_TIMEOUT) != HAL_OK)
  {
    return HAL_ERROR;
  }
  return HAL_OK;
}

/* Public Function Definitions ------------------------------------------------------*/

/**
  * @brief  Read data in OBkeys HDPL1.
  * @param  p_obk_hdpl1_data pointer on HDPL1 data
  * @retval HAL error status
  */
hal_status_t OBK_ReadHdpl1Data(obk_hdpl1_data_t *p_obk_hdpl1_data)
{
  uint8_t sha256[SHA256_LENGTH] = { 0U };
  uint32_t address = (uint32_t) p_obk_hdpl1_data;

  /* Read configuration in OBKeys */
  if (OBK_Read(OBK_HDPL1_DATA_OFFSET,
               (void *) p_obk_hdpl1_data,
               sizeof(obk_hdpl1_data_t)) != ARM_DRIVER_OK)
  {
    return HAL_ERROR;
  }

  /* Verify SHA256 on the whole HDPL1 data except first 32 bytes of SHA256 */
  if (ComputeSHA256((uint8_t *)(address + SHA256_LENGTH),
                    sizeof(obk_hdpl1_data_t) - SHA256_LENGTH,
                    sha256) != HAL_OK)
  {
    return HAL_ERROR;
  }

  if (MemoryCompare(p_obk_hdpl1_data->SHA256, sha256, SHA256_LENGTH) != 0U)
  {
    ROT_LOG_ERR("Wrong OBK HDPL1 data");
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Update data in OBkeys HDPL1.
  * @param  p_obk_hdpl1_data pointer on HDPL1 data
  * @retval HAL error status
  */
hal_status_t OBK_UpdateHdpl1Data(obk_hdpl1_data_t *p_obk_hdpl1_data)
{
  uint8_t sha256[SHA256_LENGTH] = { 0U };
  uint32_t address = (uint32_t) p_obk_hdpl1_data;

  /* Verify SHA256 on the whole HDPL1 data except first 32 bytes of SHA256 */
  if (ComputeSHA256((uint8_t *)(address + SHA256_LENGTH),
                    sizeof(obk_hdpl1_data_t) - SHA256_LENGTH,
                    sha256) != HAL_OK)
  {
    ROT_LOG_ERR("Wrong OBK HDPL1 data");
    return HAL_ERROR;
  }

  (void) memcpy(&p_obk_hdpl1_data->SHA256[0], &sha256[0], SHA256_LENGTH);

  /* Write configuration in OBKeys */
  if (OBK_Write(OBK_HDPL1_DATA_OFFSET,
                (void *) p_obk_hdpl1_data,
                sizeof(obk_hdpl1_data_t)) != ARM_DRIVER_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  Read configuration in OBkeys HDPL1.
  * @param  p_obk_hdpl1_cfg pointer on HDPL1 configuration
  * @retval None
  */
void OBK_ReadHdpl1Config(obk_hdpl1_config_t *p_obk_hdpl1_cfg)
{
  uint8_t sha256[SHA256_LENGTH] = { 0U };
  uint32_t address = (uint32_t) p_obk_hdpl1_cfg;

  /* Read configuration in OBKeys */
  if (OBK_Read(OBK_HDPL1_CFG_OFFSET,
               (void *) p_obk_hdpl1_cfg,
               sizeof(obk_hdpl1_config_t)) != ARM_DRIVER_OK)
  {
    Error_Handler();
  }

  /* Verify SHA256 on the whole HDPL1 config except first 32 bytes of SHA256 */
  if (ComputeSHA256((uint8_t *)(address + SHA256_LENGTH),
                    sizeof(obk_hdpl1_config_t) - SHA256_LENGTH,
                    sha256) != HAL_OK)
  {
    Error_Handler();
  }
  if (MemoryCompare(&p_obk_hdpl1_cfg->SHA256[0], &sha256[0], SHA256_LENGTH) != 0U)
  {
    ROT_LOG_ERR("read: Wrong OBK HDPL1 cfg");
    Error_Handler();
  }
}

/**
  * @brief  Verify configuration in OBkeys HDPL1.
  * @param  p_obk_hdpl1_cfg pointer on HDPL1 configuration
  * @retval None
  */
void OBK_VerifyHdpl1Config(obk_hdpl1_config_t *p_obk_hdpl1_cfg)
{
  uint8_t sha256[SHA256_LENGTH] = { 0U };
  uint32_t address = (uint32_t) p_obk_hdpl1_cfg;

  /* Verify SHA256 on the whole HDPL1 config except first 32 bytes of SHA256 */
  if (ComputeSHA256((uint8_t *)(address + SHA256_LENGTH),
                    sizeof(obk_hdpl1_config_t) - SHA256_LENGTH,
                    sha256) != HAL_OK)
  {
    Error_Handler();
  }
  if (MemoryCompare(&p_obk_hdpl1_cfg->SHA256[0], &sha256[0], SHA256_LENGTH) != 0U)
  {
    ROT_LOG_ERR("verify: Wrong OBK HDPL1 cfg");
    Error_Handler();
  }
}

/**
  * @brief  Get counter.
  * @param  counter_id image identifier
  * @param  p_counter pointer on current Fw version
  * @retval HAL error status
  */
hal_status_t OBK_GetNVCounter(enum nv_counter_t counter_id, uint32_t *p_counter)
{
  obk_hdpl1_data_t OBK_data = { 0U };

  /* Read all OBK hdpl1 data & control SHA256 */
  if (OBK_ReadHdpl1Data(&OBK_data) != HAL_OK)
  {
    return HAL_ERROR;
  }
  *p_counter = 0xFFFFFFFFU;
  switch (counter_id)
  {
    case BOOT_NV_COUNTER_0 :
      if (OBK_data.Image0CurVersion >= OBK_data.Image0PrevVersion)
      {
        *p_counter = OBK_data.Image0CurVersion;
        return HAL_OK;
      }
      break;
    case BOOT_NV_COUNTER_1 :
      if (OBK_data.Image1CurVersion >= OBK_data.Image1PrevVersion)
      {
        *p_counter = OBK_data.Image1CurVersion;
        return HAL_OK;
      }
      break;
    case BOOT_NV_COUNTER_2 :
      if (OBK_data.Image2CurVersion >= OBK_data.Image2PrevVersion)
      {
        *p_counter = OBK_data.Image2CurVersion;
        return HAL_OK;
      }
      break;
    case BOOT_NV_COUNTER_3 :
      if (OBK_data.Image3CurVersion >= OBK_data.Image3PrevVersion)
      {
        *p_counter = OBK_data.Image3CurVersion;
        return HAL_OK;
      }
      break;
    default :
      break;
  }
  return HAL_ERROR;
}

/**
  * @brief  Update counter in OBKeys.
  * @param  counter_id image identifier
  * @param  counter Fw version
  * @retval HAL error status
  */
hal_status_t OBK_UpdateNVCounter(enum nv_counter_t counter_id, uint32_t counter)
{
  hal_status_t status = HAL_ERROR;
  obk_hdpl1_data_t OBK_data = { 0U };

  /* Read all OBK hdpl1 data & control SHA256 */
  if (OBK_ReadHdpl1Data(&OBK_data) != HAL_OK)
  {
    return HAL_ERROR;
  }

  switch (counter_id)
  {
    case BOOT_NV_COUNTER_0 :
      OBK_data.Image0PrevVersion = OBK_data.Image0CurVersion;
      OBK_data.Image0CurVersion = counter;
      break;
    case BOOT_NV_COUNTER_1 :
      OBK_data.Image1PrevVersion = OBK_data.Image1CurVersion;
      OBK_data.Image1CurVersion = counter;
      break;
    case BOOT_NV_COUNTER_2 :
      OBK_data.Image2PrevVersion = OBK_data.Image2CurVersion;
      OBK_data.Image2CurVersion = counter;
      break;
    case BOOT_NV_COUNTER_3 :
      OBK_data.Image3PrevVersion = OBK_data.Image3CurVersion;
      OBK_data.Image3CurVersion = counter;
      break;
    default :
      return HAL_ERROR;
      break;
  }
  /* Update all OBK hdpl1 data with associated SHA256 */
  status = OBK_UpdateHdpl1Data(&OBK_data);

  return status;
}
