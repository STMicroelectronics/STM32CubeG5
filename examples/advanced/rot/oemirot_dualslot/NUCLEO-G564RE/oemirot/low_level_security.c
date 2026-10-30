/**
  ******************************************************************************
  * @file    low_level_security.c
  * @brief   security protection implementation for secure boot
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
#include "boot_hal_cfg.h"
#include "boot_hal_flowcontrol.h"
#include "region_defs.h"
#include "mcuboot_config.h"
#include "low_level_security.h"
#include "low_level_security_config.h"
#include "boot_hal_utils.h"
#include "low_level_rng.h"
#include "bootutil_priv.h"
#include <inttypes.h> /* PRIx32, PRIu32 */

/* Private defines -----------------------------------------------------------*/
#if defined(__FPU_USED) && (__FPU_USED == 1U)
#error "FPU core sticky bit must be disabled in boot stage to allow flexibility to use it or not in next stage"
#endif /* (__FPU_USED) && (__FPU_USED == 1U) */

/* Log macro enabled only when uFlowStage is FLOW_STAGE_CFG */
#define SECURITY_PROTECTION_LOG(...) \
  do { if (uFlowStage == FLOW_STAGE_CFG) { ROT_LOG_INF(__VA_ARGS__); } } while (0)

#define TAMP_SR_NO_EVENT (0U)


#define WRP_CFG_START          (FLASH_ROT_WRP_START_OFFSET / PAGE_SIZE)
#define WRP_CFG_END            (FLASH_ROT_WRP_END_OFFSET / PAGE_SIZE)
#if (WRP_CFG_END > LAST_PAGE_IN_BANK)
#error "WRP configuration check not supported"
#endif /* (WRP_CFG_END > LAST_PAGE_IN_BANK) */

#define HDP_CFG_START          (0U)
#define HDP_CFG_END            (FLASH_ROT_HDP_END_OFFSET / HDP_BLOCK_SIZE)
#if (HDP_CFG_END > LAST_HDP_BLOCK_IN_BANK)
#error "HDP configuration check not supported"
#endif /* (HDP_CFG_END > LAST_PAGE_IN_BANK) */

#define SECWM_CFG_START        (0U)
#define SECWM_BANK_1_CFG_START SECWM_CFG_START
#define SECWM_BANK_2_CFG_START SECWM_CFG_START
#define SECWM_CFG_END          (FLASH_ROT_SECWM_END_OFFSET / PAGE_SIZE)
#if (SECWM_CFG_END > LAST_PAGE_IN_BANK)
#define SECWM_BANK_1_CFG_END   (LAST_PAGE_IN_BANK)
#define SECWM_BANK_2_CFG_END   (SECWM_CFG_END - (LAST_PAGE_IN_BANK + 1U))
#else
#define SECWM_BANK_1_CFG_END   (SECWM_CFG_END)
#define SECWM_BANK_2_CFG_END   (SECWM_CFG_END)
#endif /* (SECWM_CFG_END > LAST_PAGE_IN_BANK) */

#define SECWM_REGION_MUST_BE_SECURE    (0xAAAAAAAAU)
#define SECWM_REGION_MUST_BE_NONSECURE (0x55555555U)

/* Private types -------------------------------------------------------------*/
typedef struct
{
  hal_flash_bank_t bank;
  uint32_t exp_start;
  uint32_t exp_end;
  uint32_t must_be_secure; /* SECWM_REGION_MUST_BE_SECURE or SECWM_REGION_MUST_BE_NONSECURE */
  const uint8_t *label;
} secwm_region_cfg_t;

/* Private variables ---------------------------------------------------------*/
static const secwm_region_cfg_t secwm_regions[] =
{
  {
    HAL_FLASH_BANK_1,
    SECWM_BANK_1_CFG_START,
    SECWM_BANK_1_CFG_END,
    SECWM_REGION_MUST_BE_SECURE,
    (const uint8_t *)"secwm1"
  },
  {
    HAL_FLASH_BANK_2,
    SECWM_BANK_2_CFG_START,
    SECWM_BANK_2_CFG_END,
    ((SECWM_CFG_END > LAST_PAGE_IN_BANK) != 0U) ?
    SECWM_REGION_MUST_BE_SECURE : SECWM_REGION_MUST_BE_NONSECURE,
    (const uint8_t *)"secwm2"
  },
};

/* Private function prototypes -----------------------------------------------*/
static void mpu_init_cfg(void);
static void mpu_appli_cfg(void);
static void ram_init_cfg(void);
static void tamper_init_cfg(void);
static void tamper_clear_flags(void);
static void flash_ecc_init_cfg(void);
static void flash_ecc_appli_cfg(void);
static void gtzc_init_cfg(void);
static void sau_init_cfg(void);
static void flash_init_cfg(void);
static void flash_appli_cfg(void);
static void crypto_reset_clocks(void);
static void cache_init_cfg(void);

/* external function prototypes ----------------------------------------------*/

/* Exported function definitions ---------------------------------------------*/

/**
  * @brief  Provided the early tamper configuration for Reset_Handler.
  * @param  None
  * @note   No stack must be used.
  * @retval None
  */
void SECURITY_ApplyTamperEarlyStartup(void)
{
  /* enable write access to backup domain */
  HAL_PWR_DisableRTCDomainWriteProtection();
  (void)HAL_PWR_IsEnabledRTCDomainWriteProtection(); /* Dummy read to wait for write effectiveness */

  /**
    * Enable RCC APB clock of RTC for tamper register access
    * Note: This access is done in register interface,
    * because if a tamper has occurred, the RAM is write-ignored / read as zero
    */
  STM32_ATOMIC_SET_BIT_32(RCC->APB3ENR, LL_APB3_GRP1_PERIPH_RTCAPB & LL_APB3_GRP1_PERIPH_ALL_EN_MASK);

  /**
    * Release reset of back-up domain in case it is set, to avoid
    * blocking the device (system reset does not release it)
    */
  STM32_ATOMIC_CLEAR_BIT_32(RCC->RTCCR, RCC_RTCCR_RTCDRST);
  (void)STM32_READ_REG(RCC->RTCCR); /* Dummy read to wait for write effectiveness */

  /* LL_TAMP */
  /* Check if a tamper has occurred */
  if (LL_TAMP_IsActiveFlag_ITAMP(LL_TAMP_ITAMP_ALL) != 0U)
  {
    /* ITAMP flag set, clear it */
    tamper_clear_flags();
  }
  else if (LL_TAMP_IsActiveFlag_TAMP(LL_TAMP_ALL) != 0U)
  {
    /* TAMP flag set, clear it */
    tamper_clear_flags();
  }
  else
  {
    /**
      * Enable TAMP IRQ , to catch tamper interrupt in TAMP_IRQHandler
      * else a stack in SRAM is cleaned a HardFault can occur, at every pop of
      * function
      */
    __enable_irq();
    HAL_CORTEX_NVIC_EnableIRQ(TAMP_IRQn);
  }
}

/**
  * @brief  Provided the early RAMCFG configuration for Reset_Handler.
  * @param  None
  * @note   No stack must be used.
  * @retval None
  */
void SECURITY_ApplyRamCfgEarlyStartup(void)
{
  /**
    * If a HAL API is called in this function, the stack is used (via the PUSH / POP mechanism).
    * Since the "SRAM erase on reset" is still running, this PUSH / POP mechanism corrupts
    * the registers and causes a hard fault.
    * The following HAL APIs are not compatible:
    *  - HAL_RCC_RAMCFG_EnableClock
    *  - HAL_RAMCFG_SetEraseOnSystemRst
    *  - HAL_RAMCFG_GetEraseOnSystemRst
    * LL_AHB4_GRP1_EnableClock isn't compatible either, so register accesses are mandatory.
    */
  /* Clock RAMCFG */
  STM32_ATOMIC_SET_BIT_32(RCC->AHB1ENR, LL_AHB1_GRP1_PERIPH_RAMCFG & LL_AHB1_GRP1_PERIPH_ALL_EN_MASK);
  (void)STM32_READ_REG(RCC->AHB1ENR); /* Dummy read to ensure clocking */

  /* Wait for SRAM availability */
  while (LL_RAMCFG_IsActiveFlag_SRAMBUSY(RAMCFG_SRAM1) != 0U)
  {
    __NOP();
  }
}

/**
  * @brief  Enable the cryptography clocks.
  * @param  None
  * @retval None
  */
void SECURITY_EnableCryptoClocks(void)
{
  /**
    * - Stop all used Crypton S_HCLK using RCC
    * - Drive low pulse on RNG_HRESETn, HASH_HRESETn using RCC
    * - Start all used crypton clocks RNG_HCLK, HASH_HCLK
    */
  crypto_reset_clocks();

  /* -3- Enable the clock of the used Crypto IPs */
  HAL_RCC_RNG_EnableClock();
  HAL_RCC_HASH_EnableClock();
}

/**
  * @brief  Disable the cryptography clocks.
  * @param  None
  * @retval None
  */
void SECURITY_DisableCryptoClocks(void)
{
  /**
    * - Stop all used Crypton S_HCLK using RCC
    * - Drive low pulse on RNG_HRESETn, HASH_HRESETn using RCC
    */
  crypto_reset_clocks();
}

/**
  * @brief  Apply the runtime security protections.
  * @param  None
  * @note   By default, the recommended security protections for the platform are applied.
  * @retval None
  */
void SECURITY_ApplyRunTimeProtections(void)
{
  SECURITY_PROTECTION_LOG("Applying runtime security protections:");

  /* Configure cache */
  cache_init_cfg();

  /* Configure peripheral security attribution */
  gtzc_init_cfg();

  /* Set MPU to forbid execution outside of immutable code */
  mpu_init_cfg();

  /* Enable SAU to gain access to flash area non secure for write/read */
  sau_init_cfg();

  /* Configure FLASH access */
  flash_init_cfg();

  /* With OEMIROT_DEV_MODE, active tamper calls Error_Handler */
  tamper_init_cfg();

  /* Configure SRAM ECC */
  ram_init_cfg();

  /* Configure FLASH ECC */
  flash_ecc_init_cfg();
}

/**
  * @brief  Update the runtime security protections for application start.
  * @param  None
  * @retval None
  */
void SECURITY_UpdateRunTimeProtections(void)
{
  /* Update MPU config for application execution */
  mpu_appli_cfg();

  /* Update FLASH configuration for application execution */
  flash_appli_cfg();

  /* Update FLASH ECC for application execution */
  flash_ecc_appli_cfg();
}

/**
  * @brief  Check if the Static security protections are correctly configured.
  *         Those protections are not impacted by a Reset. They are set using the Option Bytes.
  *         When the device is locked, these protections cannot be changed anymore.
  * @param  None
  * @note   By default, the recommended static security protections for the platform is checked.
  * @retval None
  */
void SECURITY_CheckStaticProtections(void)
{
  hal_flash_t flash = mx_rot_flash_gethandle()->instance;
  uint32_t ob_value = 0U;
  uint32_t i = 0U;
  uint32_t lifecycle_idx = 0U;
  uint32_t ob_start_page = 0U;
  uint32_t ob_end_page   = 0U;
  uint32_t ob_page_nbr   = 0U;
  uint32_t expected = 0U;
  (void)i;

  SECURITY_PROTECTION_LOG("Checking Option Bytes static security protections:");

  /* Check OB TZEN */
  expected = (uint32_t)HAL_FLASH_ITF_OB_TRUSTZONE_ENABLED;
  ob_value = (uint32_t)HAL_FLASH_ITF_OB_IsEnabledTrustZone(flash);
  if (ob_value != expected)
  {
    ROT_LOG_ERR("* tzen KO: expected [0x%" PRIx32 "], OB [0x%" PRIx32 "]",
                expected,
                ob_value);
    Error_Handler();
  }
  SECURITY_PROTECTION_LOG("- tzen OK");

  /* Check OB unique boot entry */
  expected = (uint32_t)HAL_FLASH_ITF_OB_UNIQUE_BOOT_USER_FLASH;
  ob_value = (uint32_t)HAL_FLASH_ITF_OB_GetUniqueBootEntry(flash);
  if (ob_value != expected)
  {
    ROT_LOG_ERR("* unique boot entry KO: expected [0x%" PRIx32 "], OB [0x%" PRIx32 "]",
                expected,
                ob_value);
    Error_Handler();
  }
  SECURITY_PROTECTION_LOG("- unique boot entry OK");

  /* Check OB boot address */
  expected = (uint32_t)ROT_VTOR_ADDR;
  ob_value = (uint32_t) HAL_FLASH_ITF_OB_GetBootAddr(flash);
  if (ob_value != expected)
  {
    ROT_LOG_ERR("* boot address KO: expected [0x%" PRIx32 "], OB [0x%" PRIx32 "]", expected, ob_value);
    Error_Handler();
  }
  SECURITY_PROTECTION_LOG("- boot address OK [0x%" PRIx32 "]", expected);

  /* Check OB Boot lock protection */
  ob_value = (uint32_t)HAL_FLASH_ITF_OB_IsLockedBootConfig(flash);
  expected = (uint32_t)HAL_FLASH_ITF_OB_BOOT_LOCKED;
  if (ob_value != expected)
  {
    ROT_LOG_ERR("* boot lock KO: expected [0x%" PRIx32 "], OB [0x%" PRIx32 "]",
                expected,
                ob_value);
    Error_Handler();
  }
  SECURITY_PROTECTION_LOG("- boot lock OK");

#if defined(OEMIROT_OB_WRP_CHECK)
  /* Check OB write protection */
  for (i = WRP_CFG_START; i <= WRP_CFG_END; i++)
  {
    if (HAL_FLASH_ITF_OB_IsEnabledPageWRP(flash, HAL_FLASH_BANK_1, i) != HAL_FLASH_ITF_OB_WRP_PAGE_PROTECTED)
    {
      ROT_LOG_ERR("* wrp1 KO: wrp missing on page %" PRIu32, i);
      Error_Handler();
    }
  }
  SECURITY_PROTECTION_LOG("- wrp1 OK [start: 0x%" PRIx32 ", end: 0x%" PRIx32 "]",
                          (uint32_t)WRP_CFG_START,
                          (uint32_t)WRP_CFG_END);

#endif /* OEMIROT_OB_WRP_CHECK */

  /* Check OB hide protection */
  HAL_FLASH_ITF_OB_GetHDPArea(flash, HAL_FLASH_BANK_1, &ob_start_page, &ob_page_nbr);
  ob_end_page = ob_start_page + ob_page_nbr - 1U;

  if ((ob_start_page != (uint32_t)HDP_CFG_START) || (ob_end_page != (uint32_t)HDP_CFG_END))
  {
    ROT_LOG_ERR("* hdp1 KO: expected [start: 0x%" PRIx32 ", end: 0x%" PRIx32
                "], OB [start: 0x%" PRIx32 ", end: 0x%" PRIx32 "]",
                (uint32_t)HDP_CFG_START,
                (uint32_t)HDP_CFG_END,
                ob_start_page,
                ob_end_page);
    Error_Handler();
  }
  SECURITY_PROTECTION_LOG("- hdp1 OK [start: 0x%" PRIx32 ", end: 0x%" PRIx32 "]",
                          (uint32_t)HDP_CFG_START, (uint32_t)HDP_CFG_END);

  /* Check OB secure watermark (bank 1 and bank 2) */
  for (i = 0U; i < (sizeof(secwm_regions) / sizeof(secwm_regions[0])); i++)
  {
    const secwm_region_cfg_t *p_region = &secwm_regions[i];

    HAL_FLASH_ITF_OB_GetSecWMArea(flash, p_region->bank, &ob_start_page, &ob_page_nbr);
    ob_end_page = ob_start_page + ob_page_nbr - 1U;

    if (p_region->must_be_secure == SECWM_REGION_MUST_BE_SECURE)
    {
      /* Bank must cover the expected secure range */
      if ((ob_start_page != p_region->exp_start) || (ob_end_page != p_region->exp_end))
      {
        ROT_LOG_ERR("* %s KO: expected [0x%" PRIx32 ", 0x%" PRIx32 "], OB [0x%" PRIx32 ", 0x%" PRIx32 "]",
                    (const char *)p_region->label, p_region->exp_start, p_region->exp_end, ob_start_page, ob_end_page);
        Error_Handler();
      }
    }
    else if (p_region->must_be_secure == SECWM_REGION_MUST_BE_NONSECURE)
    {
      /* Bank must be fully non-secure (end < start means empty watermark) */
      if (ob_end_page >= ob_start_page)
      {
        ROT_LOG_ERR("* %s KO: expected fully non-secure, OB [start: 0x%" PRIx32 ", end: 0x%" PRIx32 "]",
                    (const char *)p_region->label, ob_start_page, ob_end_page);
        Error_Handler();
      }
    }
    else
    {
      /* Unexpected value */
      Error_Handler();
    }
    SECURITY_PROTECTION_LOG("- %s OK [start: 0x%" PRIx32 ", end: 0x%" PRIx32 "]",
                            (const char *)p_region->label, p_region->exp_start, p_region->exp_end);
  }

  /* Check OB SRAM ECC */
  ob_value = (uint32_t)HAL_FLASH_ITF_OB_IsEnabledSRAMECC(flash, (uint32_t)HAL_FLASH_ITF_OB_SRAM1);
  expected = (uint32_t)HAL_FLASH_ITF_OB_SRAM_ECC_ENABLED;
  if (ob_value != expected)
  {
    ROT_LOG_ERR("* sram ecc KO: expected [0x%" PRIx32 "], OB [0x%" PRIx32 "]",
                expected,
                ob_value);
    Error_Handler();
  }
  SECURITY_PROTECTION_LOG("- sram ecc OK");

  /* Check OB SRAM ERASE on reset */
  ob_value = (uint32_t)HAL_FLASH_ITF_OB_GetSystemRstSRAMErase(flash, HAL_FLASH_ITF_OB_SRAM1);
  expected = (uint32_t)HAL_FLASH_ITF_OB_SYS_RST_SRAM_ERASE;
  if (ob_value != expected)
  {
    ROT_LOG_ERR("* sram erase on reset KO: expected [0x%" PRIx32 "], OB [0x%" PRIx32 "]",
                expected,
                ob_value);
    Error_Handler();
  }
  SECURITY_PROTECTION_LOG("- sram erase on reset OK");

  /* Check product_state : boot if current product_state is greater or equal to selected product_state */
  expected = (uint32_t)OEMIROT_OB_PRODUCT_STATE_MIN;
  /* Identify product_state mini selected */
  for (lifecycle_idx = 0U; lifecycle_idx < NB_PRODUCT_STATE; lifecycle_idx++)
  {
    if (ProductStatePrioList[lifecycle_idx] == expected)
    {
      break;
    }
  }
  if (lifecycle_idx >= NB_PRODUCT_STATE)
  {
    Error_Handler();
  }
  /* Control if current product_state is allowed */
  ob_value = (uint32_t)HAL_FLASH_ITF_OB_GetProductState(flash);
  for (; lifecycle_idx < NB_PRODUCT_STATE; lifecycle_idx++)
  {
    if (ProductStatePrioList[lifecycle_idx] == ob_value)
    {
      break;
    }
  }
  if (lifecycle_idx >= NB_PRODUCT_STATE)
  {
    ROT_LOG_ERR("* product_state KO: minimum expected [0x%" PRIx32 "], OB [0x%" PRIx32 "]",
                expected,
                ob_value);
    Error_Handler();
  }
  SECURITY_PROTECTION_LOG("- product_state OK [0x%" PRIx32 "]", (uint32_t)ProductStatePrioList[lifecycle_idx]);
}

/* Private function definitions ----------------------------------------------*/

/**
  * @brief  Disable and reset the cryptography clocks.
  * @param  None
  * @retval None
  */
static void crypto_reset_clocks(void)
{
  /* -1- Disable the clock of all the used Crypto IPs */
  HAL_RCC_RNG_DisableClock();
  HAL_RCC_HASH_DisableClock();

  /* -2- Reset all the used Crypto IPs */
  HAL_RCC_RNG_Reset();
  HAL_RCC_HASH_Reset();
}

/**
  * @brief  Mpu initial configuration for OEMiRoT execution.
  * @param  None
  * @note   The MPU is configured at the beginning of OEMiRoT execution,
  *         to protect the OEMiRoT code and data, and forbid execution outside of OEMiRoT code area.
  *         The MPU configuration is checked later in the execution to ensure it has not been modified.
  * @retval None
  */
static void mpu_init_cfg(void)
{
#if defined(OEMIROT_MPU_ENABLE)
  hal_cortex_mpu_region_config_t read_region_config = {0U};
  uint32_t i = 0U;
  hal_cortex_mpu_device_mem_attr_t device_mem_attr = HAL_CORTEX_MPU_DEVICE_MEM_INVALID;
  hal_cortex_mpu_normal_mem_cache_attr_t code_mem_attr = HAL_CORTEX_MPU_NORMAL_MEM_INVALID;
  hal_cortex_mpu_normal_mem_cache_attr_t data_mem_attr = HAL_CORTEX_MPU_NORMAL_MEM_INVALID;
  hal_cortex_mpu_normal_mem_cache_attr_t datanocache_mem_attr = HAL_CORTEX_MPU_NORMAL_MEM_INVALID;
  uint32_t privdefena_bit = 0U;
  uint32_t hfnmien_bit = 0U;
  hal_cortex_mpu_status_t mpu_enabled = HAL_CORTEX_MPU_DISABLED;

  SECURITY_PROTECTION_LOG("- mpu");

  /* Configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* Configure MPU regions */
    for (i = 0U; i < ARRAY_SIZE(mpu_regions_cfg_init); i++)
    {
      /* Configure the MPU region */
      if (HAL_CORTEX_MPU_SetConfigRegion(mpu_regions_cfg_init[i].region_idx,
                                         &(mpu_regions_cfg_init[i].region_config)) != HAL_OK)
      {
        Error_Handler();
      }

      /* Enable the MPU region */
      HAL_CORTEX_MPU_EnableRegion(mpu_regions_cfg_init[i].region_idx);

      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, mpu_regions_cfg_init[i].flow_step_enable,
                        mpu_regions_cfg_init[i].flow_ctrl_enable);
    }

    /* Configure memory attributes */
    HAL_CORTEX_MPU_SetDeviceMemAttr(MPU_MEM_ATTR_DEVICE_IDX, MPU_MEM_ATTR_DEVICE_VAL);
    HAL_CORTEX_MPU_SetCacheMemAttr(MPU_MEM_ATTR_CODE_IDX, MPU_MEM_ATTR_CODE_VAL);
    HAL_CORTEX_MPU_SetCacheMemAttr(MPU_MEM_ATTR_DATA_IDX, MPU_MEM_ATTR_DATA_VAL);
    HAL_CORTEX_MPU_SetCacheMemAttr(MPU_MEM_ATTR_DATANOCACHE_IDX, MPU_MEM_ATTR_DATANOCACHE_VAL);

    /* Configure overall MPU (including during HF/NMI handlers, and without background region) */
    HAL_CORTEX_MPU_Enable(HAL_CORTEX_MPU_HARDFAULT_NMI_ENABLE, HAL_CORTEX_MPU_ACCESS_FAULT_ALL);

    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_I_EN, FLOW_CTRL_MPU_I_EN);
  }
  /* Verification stage */
  else
  {
    /* Verify MPU regions */
    for (i = 0U; i < ARRAY_SIZE(mpu_regions_cfg_init); i++)
    {
      /* Get configuration of the MPU region */
      HAL_CORTEX_MPU_GetConfigRegion(mpu_regions_cfg_init[i].region_idx, &read_region_config);

      /* Verify MPU region configuration */
      if ((read_region_config.base_addr != mpu_regions_cfg_init[i].region_config.base_addr)
          || (read_region_config.limit_addr != mpu_regions_cfg_init[i].region_config.limit_addr)
          || (read_region_config.access_attr != mpu_regions_cfg_init[i].region_config.access_attr)
          || (read_region_config.exec_attr != mpu_regions_cfg_init[i].region_config.exec_attr)
          || (read_region_config.attr_idx != mpu_regions_cfg_init[i].region_config.attr_idx))
      {
        Error_Handler();
      }

      /* Verify MPU region is enabled */
      if (HAL_CORTEX_MPU_IsEnabledRegion(mpu_regions_cfg_init[i].region_idx) != HAL_CORTEX_MPU_REGION_ENABLED)
      {
        Error_Handler();
      }

      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, mpu_regions_cfg_init[i].flow_step_check,
                        mpu_regions_cfg_init[i].flow_ctrl_check);
    }

    /* Verify memory attributes */
    device_mem_attr = HAL_CORTEX_MPU_GetDeviceMemAttr(MPU_MEM_ATTR_DEVICE_IDX);
    code_mem_attr = HAL_CORTEX_MPU_GetCacheMemAttr(MPU_MEM_ATTR_CODE_IDX);
    data_mem_attr = HAL_CORTEX_MPU_GetCacheMemAttr(MPU_MEM_ATTR_DATA_IDX);
    datanocache_mem_attr = HAL_CORTEX_MPU_GetCacheMemAttr(MPU_MEM_ATTR_DATANOCACHE_IDX);

    if ((device_mem_attr != MPU_MEM_ATTR_DEVICE_VAL)
        || (code_mem_attr != MPU_MEM_ATTR_CODE_VAL)
        || (data_mem_attr != MPU_MEM_ATTR_DATA_VAL)
        || (datanocache_mem_attr != MPU_MEM_ATTR_DATANOCACHE_VAL))
    {
      Error_Handler();
    }

    /* Verify MPU overall configuration */
    privdefena_bit = STM32_READ_BIT(MPU->CTRL, MPU_CTRL_PRIVDEFENA_Msk);
    hfnmien_bit = STM32_READ_BIT(MPU->CTRL, MPU_CTRL_HFNMIENA_Msk);
    mpu_enabled = HAL_CORTEX_MPU_IsEnabled();

    if ((privdefena_bit != (uint32_t)HAL_CORTEX_MPU_ACCESS_FAULT_ALL)
        || (hfnmien_bit != (uint32_t)HAL_CORTEX_MPU_HARDFAULT_NMI_ENABLE)
        || (mpu_enabled != HAL_CORTEX_MPU_ENABLED))
    {
      Error_Handler();
    }

    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_MPU_I_CH, FLOW_CTRL_MPU_I_CH);
  }
#endif /* OEMIROT_MPU_ENABLE */
}

/**
  * @brief  Configure RAM with ECC and enable ECC interrupt if supported.
  * @param  None
  * @retval None
  */
static void ram_init_cfg(void)
{
  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    if (mx_rot_ramcfg_init() != HAL_RAMCFG_SRAM1)
    {
      Error_Handler();
    }

    /* Enable IT in case of Double error detection : ECC is activated through option bytes */
    if (HAL_RAMCFG_ECC_Enable_IT(SRAM_ECC, HAL_RAMCFG_IT_ECC_DOUBLE) != HAL_OK)
    {
      Error_Handler();
    }
    else
    {
      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_RAMCFG_I_EN, FLOW_CTRL_RAMCFG_I_EN);
    }
  }
  /* verification stage */
  else
  {
    if (LL_RAMCFG_IsEnabledIT_DE(RAMCFG_SRAM1) != 1U)
    {
      Error_Handler();
    }
    else
    {
      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_RAMCFG_I_CH, FLOW_CTRL_RAMCFG_I_CH);
    }
  }
}

/**
  * @brief  Mpu config update for jump to application.
  * @param  None
  * @note   The MPU configuration is updated before jumping to application,
  *         to set the appropriate permissions for the application execution
  *         (ex: allow execution and read/write access on application area,
  *         forbid execution and allow read/write access on data area, etc.).
  * @retval None
  */
static void mpu_appli_cfg(void)
{
#if defined(OEMIROT_MPU_ENABLE)
  hal_cortex_mpu_region_config_t read_region_config = {0U};
  uint32_t i = 0U;

  /* Configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* Reconfigure MPU regions */
    for (i = 0U; i < ARRAY_SIZE(mpu_regions_cfg_appli); i++)
    {
      /* Configure the MPU region */
      if (HAL_CORTEX_MPU_SetConfigRegion(mpu_regions_cfg_appli[i].region_idx,
                                         &(mpu_regions_cfg_appli[i].region_config)) != HAL_OK)
      {
        Error_Handler();
      }

#if (SECURITY_APP_MPU_REGION_COUNTER > 1)
      /* Enable the MPU region except if the first one in the table */
      if (i != 0U)
      {
        HAL_CORTEX_MPU_EnableRegion(mpu_regions_cfg_appli[i].region_idx);
      }
#endif /* (SECURITY_APP_MPU_REGION_COUNTER > 1) */

      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, mpu_regions_cfg_appli[i].flow_step_enable,
                        mpu_regions_cfg_appli[i].flow_ctrl_enable);
    }
  }
  /* Verification stage */
  else
  {
    /* Verify MPU regions */
    for (i = 0U; i < ARRAY_SIZE(mpu_regions_cfg_appli); i++)
    {
      /* Get configuration of the MPU region */
      HAL_CORTEX_MPU_GetConfigRegion(mpu_regions_cfg_appli[i].region_idx, &read_region_config);

      /* Verify MPU region configuration */
      if ((read_region_config.base_addr != mpu_regions_cfg_appli[i].region_config.base_addr)
          || (read_region_config.limit_addr != mpu_regions_cfg_appli[i].region_config.limit_addr)
          || (read_region_config.access_attr != mpu_regions_cfg_appli[i].region_config.access_attr)
          || (read_region_config.exec_attr != mpu_regions_cfg_appli[i].region_config.exec_attr)
          || (read_region_config.attr_idx != mpu_regions_cfg_appli[i].region_config.attr_idx))
      {
        Error_Handler();
      }

#if (SECURITY_APP_MPU_REGION_COUNTER > 1)
      if (i != 0U)
      {
        /* Verify the MPU region is enabled, if not the first one in the table */
        if (HAL_CORTEX_MPU_IsEnabledRegion(mpu_regions_cfg_appli[i].region_idx) != HAL_CORTEX_MPU_REGION_ENABLED)
        {
          Error_Handler();
        }
      }
      else
#endif /* (SECURITY_APP_MPU_REGION_COUNTER > 1) */
      {
        /* Verify the MPU region is disabled, if the first one in the table */
        if (HAL_CORTEX_MPU_IsEnabledRegion(mpu_regions_cfg_appli[i].region_idx) != HAL_CORTEX_MPU_REGION_DISABLED)
        {
          Error_Handler();
        }
      }

      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, mpu_regions_cfg_appli[i].flow_step_check,
                        mpu_regions_cfg_appli[i].flow_ctrl_check);
    }
  }
#endif /* OEMIROT_MPU_ENABLE */
}

/**
  * @brief  Configure the tamper detection.
  * @param  None
  * @retval None
  */
static void tamper_init_cfg(void)
{
#if defined(OEMIROT_TAMPER_ENABLE)
  uint32_t is_tamp_enabled = 0U;
  uint32_t is_it_enabled = 0U;
  uint32_t config_internal_tampers = 0U;

  SECURITY_PROTECTION_LOG("- tamper");

  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* TAMP Init */
    if (mx_rot_tamp_init() != SYSTEM_OK)
    {
      Error_Handler();
    }

    /* Enable access to tamper registers */
    HAL_PWR_DisableRTCDomainWriteProtection();

    /*  Internal Tamper activation  */
    if (HAL_TAMP_INTERNAL_SetConfigTampers(TAMP_INTERNAL_TAMPER, &IndivInternalTamperConf) != HAL_OK)
    {
      Error_Handler();
    }
    if (HAL_TAMP_INTERNAL_Start(TAMP_INTERNAL_TAMPER, TAMP_INTERNAL_IT_TAMPER) != HAL_OK)
    {
      Error_Handler();
    }
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_INT_EN, FLOW_CTRL_TAMP_INT_EN);

    /*  Set tamper configuration secure only  */
    if (HAL_TAMP_SetSecAttr(HAL_TAMP_SEC_ITEM_TAMP, HAL_TAMP_SEC) != HAL_OK)
    {
      Error_Handler();
    }
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_SEC_EN, FLOW_CTRL_TAMP_SEC_EN);

    /*  Set tamper configuration privileged only  */
    if (HAL_TAMP_SetPrivAttr(HAL_TAMP_PRIV_ITEM_TAMP, HAL_TAMP_PRIV) != HAL_OK)
    {
      Error_Handler();
    }
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_PRIV_EN, FLOW_CTRL_TAMP_PRIV_EN);

    /*  Activate Secret Erase  */
    if (HAL_TAMP_EnableResourceProtection(OEMIROT_TAMPER_RESOURCES) != HAL_OK)
    {
      Error_Handler();
    }
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_CFG_EN, FLOW_CTRL_TAMP_CFG_EN);
  }
  /* verification stage */
  else
  {
    /*  Check Internal Tamper activation */
    is_tamp_enabled = LL_TAMP_GetEnabled(TAMP_INTERNAL_TAMPER);
    is_it_enabled = LL_TAMP_IsEnabledIT(TAMP_INTERNAL_IT_TAMPER);
    config_internal_tampers = LL_TAMP_GetConfigInternalTampers(TAMP_ITAMP_NOERASE_TAMPER);

    if ((is_tamp_enabled != TAMP_INTERNAL_TAMPER)
        || (is_it_enabled != 1U)
        || (config_internal_tampers != 0x0U))
    {
      Error_Handler();
    }
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_INT_CH, FLOW_CTRL_TAMP_INT_CH);

    /*  Check tamper configuration secure only  */
    if (HAL_TAMP_GetSecAttr(HAL_TAMP_SEC_ITEM_TAMP) != HAL_TAMP_SEC)
    {
      Error_Handler();
    }
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_SEC_CH, FLOW_CTRL_TAMP_SEC_CH);

    /*  Check tamper configuration privileged only   */
    if (HAL_TAMP_GetPrivAttr(HAL_TAMP_PRIV_ITEM_TAMP) != HAL_TAMP_PRIV)
    {
      Error_Handler();
    }
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_PRIV_CH, FLOW_CTRL_TAMP_PRIV_CH);

    /*  Check Secret Erase */
    if (HAL_TAMP_IsEnabledResourceProtection(OEMIROT_TAMPER_RESOURCES) != HAL_TAMP_PROTECT_RESOURCES_ENABLED)
    {
      Error_Handler();
    }
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_TAMP_CFG_CH, FLOW_CTRL_TAMP_CFG_CH);
  }
#endif /* OEMIROT_TAMPER_ENABLE */
}

/**
  * @brief  Disable NMI on double ECC flash error during OEMiRoT execution.
  * @param  None
  * @note   It avoids blocking the device in case of double ECC error during boot execution.
  *         The NMI is re-enabled on double ECC error for jump to application, to allow the
  *         device to reset in case of double ECC error during application execution.
  *         This is done in flash_ecc_appli_cfg() function.
  * @retval None
  */
static void flash_ecc_init_cfg(void)
{
  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    HAL_RCC_SBS_EnableClock();
    HAL_SBS_FLASH_DisableECCNMI();

    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_ECC_I_EN, FLOW_CTRL_FLASH_ECC_I_EN);
  }
  /* verification stage */
  else
  {
    /*  Check deactivation of ECC NMI */
    if (HAL_SBS_FLASH_IsEnabledECCNMI() != 0U)
    {
      Error_Handler();
    }
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_ECC_I_CH, FLOW_CTRL_FLASH_ECC_I_CH);
  }

}

/**
  * @brief  Enable back NMI on double ECC flash error for jump to application.
  * @param  None
  * @retval None
  */
static void flash_ecc_appli_cfg(void)
{
  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* Clear any potential double ECC error flag that might have occurred during the boot process */
    LL_FLASH_ClearFlag_ECCD(FLASH);
    (void)LL_FLASH_IsActiveFlag_ECCD(FLASH); /* Dummy read to wait for write effectiveness */

    HAL_SBS_FLASH_EnableECCNMI();
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_ECC_A_EN, FLOW_CTRL_FLASH_ECC_A_EN);
  }
  /* verification stage */
  else
  {
    /* Check activation of ECC NMI */
    if (HAL_SBS_FLASH_IsEnabledECCNMI() != 1U)
    {
      Error_Handler();
    }
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_ECC_A_CH, FLOW_CTRL_FLASH_ECC_A_CH);
  }
}

/**
  * @brief  Clear tamper flags and pending IRQ in case of tamper detection during boot.
  * @param  None
  * @note   This function is called when a tamper event is detected during the early startup
  *         (or when the device was in sleep mode and woke up on tamper event).
  *         It clears the tamper flags and pending IRQ to allow the device to boot.
  * @retval None
  */
static void tamper_clear_flags(void)
{
  /* Clear ITAMP and TAMP flags */
  LL_TAMP_ClearFlag_ITAMP(LL_TAMP_ITAMP_ALL);
  LL_TAMP_ClearFlag_TAMP(LL_TAMP_ALL);
  (void)LL_TAMP_IsActiveFlag_ITAMP(LL_TAMP_ITAMP_ALL); /* Dummy read to wait for write effectiveness */
  HAL_CORTEX_NVIC_ClearPendingIRQ(TAMP_IRQn);

  Error_Handler();
}

/**
  * @brief  Configure TrustZone peripheral security attribution.
  * @param  None
  * @retval None
  */
static void gtzc_init_cfg(void)
{
  SECURITY_PROTECTION_LOG("- gtzc");

  /* configuration stage */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    HAL_RCC_TZSC1_EnableClock();
    /* Configure SAES as Secure when application isolation is disabled */
    STM32_SET_BIT(GTZC1_TZSC1_PERIPH_S->SECCFGR[2], GTZC_CFGR3_SAES_Msk);
    /* no FLOW control : this configuration is not part of the security but this is just for functionality */
  }
  /* verification stage */
  else
  {
    /* nothing to do : this configuration is not part of the security but this is just for functionality */
  }
}

/**
  * @brief  Sau idau init.
  * @param  None
  * @retval None
  */
static void sau_init_cfg(void)
{
  uint32_t ctrl_reg = 0U;

  SECURITY_PROTECTION_LOG("- sau");

  /* configuration stage
     =================== */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* Disable SAU, and all memory is marked as Non-Secure */
    SAU->CTRL = ((0U << SAU_CTRL_ENABLE_Pos) & SAU_CTRL_ENABLE_Msk) |
                ((1U << SAU_CTRL_ALLNS_Pos)  & SAU_CTRL_ALLNS_Msk);

    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_SAU_I_EN, FLOW_CTRL_SAU_I_EN);
  }
  /* verification stage
     ================== */
  else
  {
    /* Control SAU configuration */
    ctrl_reg = SAU->CTRL;
    if (ctrl_reg != (((0U << SAU_CTRL_ENABLE_Pos) & SAU_CTRL_ENABLE_Msk) |
                     ((1U << SAU_CTRL_ALLNS_Pos)  & SAU_CTRL_ALLNS_Msk)))
    {
      Error_Handler();
    }
    else
    {
      /* Execution stopped if flow control failed */
      FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_SAU_I_CH, FLOW_CTRL_SAU_I_CH);
    }
  }
}

/**
  * @brief  Configure flash access.
  * @param  None
  * @retval None
  */
static void flash_init_cfg(void)
{
  hal_flash_handle_t *p_hflash = mx_rot_flash_gethandle();
  hal_flash_t p_flash = HAL_FLASH_GetInstance(p_hflash);
  uint32_t page_idx;

  SECURITY_PROTECTION_LOG("- flash");

  /* configuration stage
  =================== */
  if (uFlowStage == FLOW_STAGE_CFG)
  {

    /* Configure Bank1 flash pages accessible by secure access */
    if (HAL_FLASH_ITF_BB_SetSecAttrByPage(p_flash, HAL_FLASH_BANK_1,
                                          0U, LAST_PAGE_IN_BANK + 1U,
                                          HAL_FLASH_ITF_SEC) != HAL_OK)
    {
      ROT_LOG_ERR("* Failed to configure secure attribute by page.");
      Error_Handler();
    }

    for (page_idx = 0U; page_idx <= LAST_PAGE_IN_BANK; page_idx++)
    {
      if (HAL_FLASH_ITF_BB_GetSecAttrByPage(p_flash, HAL_FLASH_BANK_1, page_idx) != HAL_FLASH_ITF_SEC)
      {
        Error_Handler();
      }
    }
    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_I_EN_B1, FLOW_CTRL_FLASH_I_EN_B1);

    /* Configure Bank2 flash pages accessible by secure access */
    if (HAL_FLASH_ITF_BB_SetSecAttrByPage(p_flash, HAL_FLASH_BANK_2,
                                          0U, LAST_PAGE_IN_BANK + 1U,
                                          HAL_FLASH_ITF_SEC) != HAL_OK)
    {
      ROT_LOG_ERR("Failed to configure secure attribute by page.");
      Error_Handler();
    }

    for (page_idx = 0U; page_idx <= LAST_PAGE_IN_BANK; page_idx++)
    {
      if (HAL_FLASH_ITF_BB_GetSecAttrByPage(p_flash, HAL_FLASH_BANK_2, page_idx) != HAL_FLASH_ITF_SEC)
      {
        Error_Handler();
      }
    }
    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_I_EN_B2, FLOW_CTRL_FLASH_I_EN_B2);
  }
  /* verification stage
  ================== */
  else
  {
    /* Verify Bank1 flash pages accessible by secure access */
    for (page_idx = 0U; page_idx <= LAST_PAGE_IN_BANK; page_idx++)
    {
      if (HAL_FLASH_ITF_BB_GetSecAttrByPage(p_flash, HAL_FLASH_BANK_1, page_idx) != HAL_FLASH_ITF_SEC)
      {
        Error_Handler();
      }
    }
    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_I_CH_B1, FLOW_CTRL_FLASH_I_CH_B1);

    /* Verify Bank2 flash pages accessible by secure access */
    for (page_idx = 0U; page_idx <= LAST_PAGE_IN_BANK; page_idx++)
    {
      if (HAL_FLASH_ITF_BB_GetSecAttrByPage(p_flash, HAL_FLASH_BANK_2, page_idx) != HAL_FLASH_ITF_SEC)
      {
        Error_Handler();
      }
    }
    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_I_CH_B2, FLOW_CTRL_FLASH_I_CH_B2);
  }
}

/**
  * @brief  Configure flash access.
  * @param  None
  * @retval None
  */
static void flash_appli_cfg(void)
{
  hal_flash_handle_t *p_hflash = mx_rot_flash_gethandle();
  hal_flash_t p_flash = HAL_FLASH_GetInstance(p_hflash);
  uint32_t page_idx;

  /* configuration stage
  =================== */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* Configure all flash pages accessible by non-secure access */
    if (HAL_FLASH_ITF_BB_SetSecAttrByPage(p_flash, HAL_FLASH_BANK_1,
                                          0, LAST_PAGE_IN_BANK + 1,
                                          HAL_FLASH_ITF_NSEC) != HAL_OK)
    {
      ROT_LOG_ERR("* Failed to configure secure attribute by page.");
      Error_Handler();
    }

    for (page_idx = 0U; page_idx <= LAST_PAGE_IN_BANK; page_idx++)
    {
      if (HAL_FLASH_ITF_BB_GetSecAttrByPage(p_flash, HAL_FLASH_BANK_1, page_idx) != HAL_FLASH_ITF_NSEC)
      {
        Error_Handler();
      }
    }
    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_A_EN_B1, FLOW_CTRL_FLASH_A_EN_B1);

    if (HAL_FLASH_ITF_BB_SetSecAttrByPage(p_flash, HAL_FLASH_BANK_2,
                                          0, LAST_PAGE_IN_BANK + 1,
                                          HAL_FLASH_ITF_NSEC) != HAL_OK)
    {
      ROT_LOG_ERR("Failed to configure secure attribute by page.");
      Error_Handler();
    }

    for (page_idx = 0U; page_idx <= LAST_PAGE_IN_BANK; page_idx++)
    {
      if (HAL_FLASH_ITF_BB_GetSecAttrByPage(p_flash, HAL_FLASH_BANK_2, page_idx) != HAL_FLASH_ITF_NSEC)
      {
        Error_Handler();
      }
    }
    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_A_EN_B2, FLOW_CTRL_FLASH_A_EN_B2);
  }
  /* verification stage
  ================== */
  else
  {
    /* Verify all flash pages accessible by secure access */
    for (page_idx = 0U; page_idx <= LAST_PAGE_IN_BANK; page_idx++)
    {
      if (HAL_FLASH_ITF_BB_GetSecAttrByPage(p_flash, HAL_FLASH_BANK_1, page_idx) != HAL_FLASH_ITF_NSEC)
      {
        Error_Handler();
      }
    }
    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_A_CH_B1, FLOW_CTRL_FLASH_A_CH_B1);

    for (page_idx = 0U; page_idx <= LAST_PAGE_IN_BANK; page_idx++)
    {
      if (HAL_FLASH_ITF_BB_GetSecAttrByPage(p_flash, HAL_FLASH_BANK_2, page_idx) != HAL_FLASH_ITF_NSEC)
      {
        Error_Handler();
      }
    }
    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_FLASH_A_CH_B2, FLOW_CTRL_FLASH_A_CH_B2);
  }
}

/**
  * @brief  Cache init.
  * @param  None
  * @retval None
  */
static void cache_init_cfg(void)
{
  /* configuration stage
  =================== */
  if (uFlowStage == FLOW_STAGE_CFG)
  {
    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_CACHE_LOCK_I_EN, FLOW_CTRL_CACHE_LOCK_I_EN);
  }
  /* verification stage
  ================== */
  else
  {
    /* Execution stopped if flow control failed */
    FLOW_CONTROL_STEP(uFlowProtectValue, FLOW_STEP_CACHE_LOCK_I_CH, FLOW_CTRL_CACHE_LOCK_I_CH);
  }
}
