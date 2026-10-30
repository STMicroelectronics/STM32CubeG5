/**
  ******************************************************************************
  * file           : example_ahb.c
  * brief          : example program body
  ******************************************************************************
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* @user: SRAM_SINGLE_ERR_ADDR_BASE and SRAM_DOUBLE_ERR_ADDR_BASE must be inside SRAM region. */
#define SRAM_ERR_ADDR_BASE SRAM_BASE
#define SRAM_ERR_ADDR_SIZE (0x100U / 0x04U)
#define SRAM_SINGLE_ERR_ADDR_BASE (SRAM_BASE + 0x20U)
#define SRAM_DOUBLE_ERR_ADDR_BASE (SRAM_BASE + 0x40U)

#define DATA_VALUE                ((uint32_t)0xAA55AA55UL)
#define SINGLE_ERR_DATA_VALUE     ((uint32_t)0xAA55AA54UL)
#define DOUBLE_ERR_DATA_VALUE     ((uint32_t)0xAA55AA50UL)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_ramcfg_t RamcfgInstance;

volatile uint8_t IsSingleErrorDetected = 0U;
volatile uint8_t IsDoubleErrorDetected = 0U;

volatile uint32_t SingleErrorAddr = 0U;
volatile uint32_t DoubleErrorAddr = 0U;

uint32_t DummyValue = 0U;

/* Private functions prototype -----------------------------------------------*/
static void FillMemory(uint32_t *pMemory, uint16_t MemorySize, uint32_t Data);

/** ########## Step 1 ##########
  * The applicative code initializes the RAMCFG instance.
  */
app_status_t app_init(void)
{
  /* Getting Ramcfg Instance */
  RamcfgInstance = mx_example_ramcfg_init();
  if (RamcfgInstance == 0U)
  {
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");

  return EXEC_STATUS_INIT_OK;
_app_init_exit:
  return EXEC_STATUS_ERROR;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Injects one single-bit ECC error and one double-bit ECC error into memory.
    */
  /* Writes known data. */
  FillMemory((uint32_t *)SRAM_ERR_ADDR_BASE, SRAM_ERR_ADDR_SIZE, DATA_VALUE);

  /* Disables ECC. */
  if (HAL_RAMCFG_ECC_Disable(RamcfgInstance) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Injects one single error and one double error into memory. */
  *(uint32_t *)SRAM_SINGLE_ERR_ADDR_BASE = SINGLE_ERR_DATA_VALUE;
  *(uint32_t *)SRAM_DOUBLE_ERR_ADDR_BASE = DOUBLE_ERR_DATA_VALUE;

  /* Enables ECC monitoring and the related interrupts. */
  if (HAL_RAMCFG_ECC_Enable_IT(RamcfgInstance, (HAL_RAMCFG_IT_ECC_SINGLE | HAL_RAMCFG_IT_ECC_DOUBLE_NMI)) != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2: One ECC single error and one ECC double error injected.\n");

  /** ########## Step 3 ##########
    * Detects and corrects the single ECC error.
    */
  /* Trigger the single ECC error. */
  DummyValue = *(uint32_t *)SRAM_SINGLE_ERR_ADDR_BASE;

  /* Wait for the single ECC error interrupt. */
  while (IsSingleErrorDetected == 0U)
  {
    __WFI();
  }
  PRINTF("[INFO] Step 3: Single ECC detected and corrected at address 0x%08lX\n", SingleErrorAddr);

  /** ########## Step 4 ##########
    * Detects the double ECC error.
    */
  /* Trigger the double ECC error. */
  DummyValue = *(uint32_t *)SRAM_DOUBLE_ERR_ADDR_BASE;

  /* Wait for the double ECC error interrupt. */
  while (IsDoubleErrorDetected == 0U)
  {
    __WFI();
  }
  PRINTF("[INFO] Step 4: Double ECC detected and not corrected at address 0x%08lX\n", DoubleErrorAddr);

  return_status = EXEC_STATUS_OK;
_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 5 ##########
  * Disables and stops the ECC monitoring of the selected RAMCFG instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_ramcfg_deinit();
  PRINTF("[INFO] Step 5: de-init.\n");
  return EXEC_STATUS_OK;
} /* end app_deinit */


/** RAMCFG ECC ErrorCallback
  * @user This RAMCFG error callback implementation can be customized.
  */
hal_status_t HAL_RAMCFG_ECC_ErrorCallback(hal_ramcfg_t hramcfg)
{
  hal_ramcfg_ecc_info_t p_info;

  HAL_RAMCFG_ECC_GetInfo(hramcfg, &p_info);

  if ((p_info.type == HAL_RAMCFG_ECC_SINGLE) && (p_info.status == HAL_RAMCFG_ECC_CORRECTED))
  {
    SingleErrorAddr = p_info.address;
    IsSingleErrorDetected = 1U;
  }

  if ((p_info.type == HAL_RAMCFG_ECC_DOUBLE) && (p_info.status == HAL_RAMCFG_ECC_NOT_CORRECTED))
  {
    DoubleErrorAddr = p_info.address;
    IsDoubleErrorDetected = 1U;
  }

  return HAL_OK;
}

static void FillMemory(uint32_t *pMemory, uint16_t MemorySize, uint32_t Data)
{
  /* Write Data to each 32-bit word in the selected range. */
  for (uint32_t IndexTmp = 0; IndexTmp < MemorySize; IndexTmp++)
  {
    pMemory[IndexTmp] = Data;
  }
}

