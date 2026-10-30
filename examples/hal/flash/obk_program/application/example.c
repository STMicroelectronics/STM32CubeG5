/**
  ******************************************************************************
  * file           : example.c
  * brief          : FLASH OBK program and HDPL readings example (polling mode).
  *                  This example shows how to:
  *                    - Program OBK1, OBK2 and OBK3 using the recommended
  *                      erase / program / swap sequence.
  *                    - Increment HDPL up to level 3.
  *                    - Check OBK readings when HDPL = 3.
  *
  *                  At HDPL3:
  *                    - OBK1 and OBK2 must be hidden (RAZ + OBKERR).
  *                    - OBK3 must remain readable (data + no OBKERR).
  *
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

/* Description of one OBK area used in this example. */
typedef struct
{
  hal_flash_obk_area_t          area_id;      /*!< OBK area identifier.                            */
  uint32_t                      base_addr;    /*!< Non-secure OBK base address of this area.       */
  hal_sbs_hdp_obk_level_value_t hdpl_offset; /*!< OBK-HDPL offset value for this area.            */
  uint32_t                      swap_offset; /*!< OBK byte offset of the end of this area.        */
  const char                    *label;        /*!< OBK area label used in logs.                  */
} obk_area_cfg_t;

/* Private define ------------------------------------------------------------*/
#define OBK_TIMEOUT_MS         (3000U)
#define OBK_DATA_WORD_NB       (8U)
#define OBK_AREA_NB            (3U)


/* Byte offset of the end of each OBK area */
#define FLASH_OBK_AREA_1_SWAP_OFFSET  (FLASH_OBK_HDPL0_SIZE + FLASH_OBK_HDPL1_SIZE)
#define FLASH_OBK_AREA_2_SWAP_OFFSET  (FLASH_OBK_AREA_1_SWAP_OFFSET + FLASH_OBK_HDPL2_SIZE)
#define FLASH_OBK_AREA_3_SWAP_OFFSET  (FLASH_OBK_AREA_2_SWAP_OFFSET + FLASH_OBK_HDPL3_SIZE)

/* The data to be programmed and its size is measured in bytes. */
static const uint32_t ObkDataWords[OBK_DATA_WORD_NB] =
{
  0x00112233UL,
  0x44556677UL,
  0x8899AABBUL,
  0xCCDDEEFFUL,
  0x11223344UL,
  0x55667788UL,
  0x99AABBCCUL,
  0xDDEEFF00UL
};
#define BUFFER_SIZE  (sizeof(ObkDataWords))

/* OBK areas used in this example. */
static const obk_area_cfg_t ObkAreaInfo[OBK_AREA_NB] =
{
  {
    HAL_FLASH_OBK_AREA_1, FLASH_OBK_HDPL1_BASE_NS, HAL_SBS_HDP_OBK_LEVEL_0,
    FLASH_OBK_AREA_1_SWAP_OFFSET, "OBK area 1"
  },
  {
    HAL_FLASH_OBK_AREA_2, FLASH_OBK_HDPL2_BASE_NS, HAL_SBS_HDP_OBK_LEVEL_1,
    FLASH_OBK_AREA_2_SWAP_OFFSET, "OBK area 2"
  },
  {
    HAL_FLASH_OBK_AREA_3, FLASH_OBK_HDPL3_BASE_NS, HAL_SBS_HDP_OBK_LEVEL_2,
    FLASH_OBK_AREA_3_SWAP_OFFSET, "OBK area 3"
  }
};

/* Private variables ---------------------------------------------------------*/
hal_flash_handle_t *pFLASH;  /* pointer referencing the FLASH handle from the generated code */
/* Private function prototypes -----------------------------------------------*/
static hal_status_t FlashObkEraseProgramCheck(const obk_area_cfg_t *p_area,
                                              const uint32_t       *p_data,
                                              uint32_t              data_word_count);
static hal_status_t FlashObkReadAccessCheck(const uint32_t *p_data, uint32_t data_word_count);
static app_status_t check_obk_page_content(hal_flash_obk_page_t obk_page,
                                           uint32_t start_addr,
                                           const uint32_t *expected_data,
                                           uint32_t size_byte);

/** ########## Step 1 ##########
  * Initializes the FLASH instance and prepares OBK access:
  *   - Retrieves the FLASH handle from the generated code.
  *   - Unlocks the FLASH interface control.
  *   - Unlocks the OBK interface control.
  *   - Selects the OBK-HDPL offset so that OBK1 (HDPL + 0) is reachable.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pFLASH = mx_example_flash_init();

  if (pFLASH == NULL)
  {
    goto _app_init_exit;
  }

  /* FLASH control access must be unlocked before any OBK service can be started. */
  if (HAL_FLASH_ITF_Unlock(HAL_FLASH) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Configure OBK-HDPL so that OBK1 (HDPL + 0) is reachable. */
  HAL_SBS_SetHDPOBKLevelValue(HAL_SBS_HDP_OBK_LEVEL_0);

  PRINTF("[INFO] Step 1: Device initialization completed - FLASH interface is UNLOCKED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  if (return_status == EXEC_STATUS_ERROR)
  {
    PRINTF("[ERROR] Step 1: Device initialization ERROR.\n");
  }
  return return_status;
} /* end app_init */


/**
  * ########## Steps 2..6 ##########
  *
  * Step 2: Report current HDPL (informational).
  * Step 3: Provision OBK1 using erase / program / swap sequence.
  * Step 4: Provision OBK2 at OBK-HDPL offset +1.
  * Step 5: Provision OBK3S at OBK-HDPL offset +2.
  * Step 6: Raise HDPL to level 3 and check OBK read access for each area.
  */
app_status_t app_process(void)
{
  app_status_t               return_status = EXEC_STATUS_ERROR;
  hal_status_t               hal_status;
  hal_sbs_hdp_level_value_t  hdpl_level;
  const char                *hdpl_str;

  /* Step 2: Report current HDPL. */
  hdpl_level = HAL_SBS_GetHDPLevelValue();
  switch (hdpl_level)
  {
    case HAL_SBS_HDP_LEVEL_0:
      hdpl_str = "HDPL0";
      break;
    case HAL_SBS_HDP_LEVEL_1:
      hdpl_str = "HDPL1";
      break;
    case HAL_SBS_HDP_LEVEL_2:
      hdpl_str = "HDPL2";
      break;
    case HAL_SBS_HDP_LEVEL_3:
      hdpl_str = "HDPL3";
      break;
    default:
      hdpl_str = "UNKNOWN";
      break;
  }
  PRINTF("[INFO] Step 2: Initial HDPL level is %s (encoded value 0x%02" PRIX32 ")\n",
         hdpl_str, (uint32_t)hdpl_level);

  /* Steps 3..5: provision OBK1, OBK2, OBK3S. */
  PRINTF("[INFO] Step 3: Provision OBK1 area using erase / program / swap sequence.\n");

  hal_status = FlashObkEraseProgramCheck(&ObkAreaInfo[0], ObkDataWords, OBK_DATA_WORD_NB);
  if (hal_status != HAL_OK)
  {
    PRINTF("[ERROR] Step 3: OBK1 provisioning sequence ERROR.\n");
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 4: Increment OBK-HDPL (offset +1) and provision OBK2 area.\n");
  hal_status = FlashObkEraseProgramCheck(&ObkAreaInfo[1], ObkDataWords, OBK_DATA_WORD_NB);
  if (hal_status != HAL_OK)
  {
    PRINTF("[ERROR] Step 4: OBK2 provisioning sequence ERROR.\n");
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 5: Increment OBK-HDPL (offset +2) and provision OBK3 area.\n");
  hal_status = FlashObkEraseProgramCheck(&ObkAreaInfo[2], ObkDataWords, OBK_DATA_WORD_NB);
  if (hal_status != HAL_OK)
  {
    PRINTF("[ERROR] Step 5: OBK3 provisioning sequence ERROR.\n");
    goto _app_process_exit;
  }

  /* Increment HDPL step-by-step. */
  hdpl_level = HAL_SBS_GetHDPLevelValue();

  if ((hdpl_level == HAL_SBS_HDP_LEVEL_0) || (hdpl_level == HAL_SBS_HDP_LEVEL_1))
  {
    if (HAL_SBS_SetHDPLevelValue(HAL_SBS_HDP_LEVEL_2) != HAL_OK)
    {
      PRINTF("[ERROR] Step 6: HDPL update to level 2 returned an ERROR.\n");
      goto _app_process_exit;
    }
    hdpl_level = HAL_SBS_GetHDPLevelValue();
  }

  if (hdpl_level == HAL_SBS_HDP_LEVEL_2)
  {
    if (HAL_SBS_SetHDPLevelValue(HAL_SBS_HDP_LEVEL_3) != HAL_OK)
    {
      PRINTF("[ERROR] Step 6: HDPL update to level 3 returned an ERROR.\n");
      goto _app_process_exit;
    }
    hdpl_level = HAL_SBS_GetHDPLevelValue();
  }

  if (hdpl_level != HAL_SBS_HDP_LEVEL_3)
  {
    PRINTF("[ERROR] Step 6: HDPL is not at level 3 after increment sequence "
           "(HDPL code=0x%02" PRIX32 ", expected 0x6F - see SBS_HDPLSR encoding).\n",
           (uint32_t)hdpl_level);
    goto _app_process_exit;
  }

  hdpl_str = "HDPL3";
  PRINTF("[INFO] Step 6: HDPL is now set to %s (encoded value 0x%02" PRIX32 ").\n",
         hdpl_str, (uint32_t)hdpl_level);

  STM32_UNUSED(hdpl_str);

  /* Perform OBK read access check at HDPL3. */
  hal_status = FlashObkReadAccessCheck(ObkDataWords, OBK_DATA_WORD_NB);
  if (hal_status != HAL_OK)
  {
    PRINTF("[ERROR] Step 7: OBK readings at HDPL3 do NOT match the expected HDPL isolation pattern.\n");
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 7: OBK readings at HDPL3 match the expected HDPL isolation pattern.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 8 ##########
  * Locks the FLASH control interfaces and deinitializes the FLASH instance.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status = EXEC_STATUS_OK;

  /* Lock the FLASH control interface. */
  HAL_FLASH_ITF_Lock(HAL_FLASH);

  mx_example_flash_deinit();

  PRINTF("[INFO] Step 8: Device de-init COMPLETED - FLASH interface is LOCKED.\n");

  return return_status;
}


/* Static functions ----------------------------------------------------------*/

/*
  * Erase the alternate OBK page, program the new data and check it.
  *
  * Sequence (polling mode):
  *   - Select OBK-HDPL level for this area.
  *   - Unlock the OBK area.
  *   - Erase the alternate OBK page.
  *   - Program two quadwords into the alternate page at base address.
  *   - Verify both quadwords if properly written.
  *   - Swap alternate/current OBK pages.
  *   - Lock the OBK area.
  *
  * After this sequence, the provided data is present in the CURRENT
  * OBK page of the selected area.
  */
static hal_status_t FlashObkEraseProgramCheck(const obk_area_cfg_t *p_area,
                                              const uint32_t       *p_data,
                                              uint32_t              data_word_count)
{
  hal_status_t status          = HAL_ERROR;
  uint32_t     data_size_bytes = data_word_count * sizeof(uint32_t);


  /* OBK control access is managed separately from the main FLASH interface. */
  if (HAL_FLASH_ITF_OBK_Unlock(HAL_FLASH) != HAL_OK)
  {
    goto _obk_program_exit;
  }

  /* Select the OBK-HDPL offset appropriate for this area. */
  HAL_SBS_SetHDPOBKLevelValue(p_area->hdpl_offset);

  /* Erase the alternate OBK page. */
  if (HAL_FLASH_OBK_MassErase(pFLASH, p_area->area_id, OBK_TIMEOUT_MS) != HAL_OK)
  {
    PRINTF("  [ERROR] %s: alternate page ERASE ERROR.\n", p_area->label);
    goto _obk_program_exit;
  }

  /* Program two quadwords into the alternate page (size in BYTES). */
  if (HAL_FLASH_OBK_ProgramByAddr(pFLASH,
                                  HAL_FLASH_OBK_PAGE_ALTERNATE,
                                  p_area->base_addr,
                                  p_data,
                                  data_size_bytes,
                                  OBK_TIMEOUT_MS) != HAL_OK)
  {
    PRINTF("  [ERROR] %s: alternate page PROGRAM ERROR.\n", p_area->label);
    goto _obk_program_exit;
  }

  /* Verify both quadwords at the programmed address (still mapped on alternate page). */
  if (check_obk_page_content(HAL_FLASH_OBK_PAGE_ALTERNATE,
                             p_area->base_addr,
                             p_data,
                             data_size_bytes) != EXEC_STATUS_OK)
  {
    PRINTF("  [ERROR] %s: alternate page VERIFY ERROR.\n", p_area->label);
    goto _obk_program_exit;
  }

  /* Swap alternate/current OBK pages to commit the new contents. */
  if (HAL_FLASH_ITF_OBK_SwapPage(HAL_FLASH, p_area->swap_offset) != HAL_OK)
  {
    PRINTF("  [ERROR] %s: page SWAP ERROR.\n", p_area->label);
    goto _obk_program_exit;
  }

  /* Lock the OBK area (while its OBK-HDPL selection is active). */
  if (HAL_FLASH_ITF_OBK_Lock(HAL_FLASH) != HAL_OK)
  {
    PRINTF("  [ERROR] %s: OBK control LOCK ERROR after provisioning.\n", p_area->label);
    goto _obk_program_exit;
  }

  status = HAL_OK;

_obk_program_exit:
  return status;
}

/*
  * At HDPL3, check OBK behavior using the first quadword only:
  *   - OBK1 / OBK2: must be hidden (RAZ plus OBKERR set).
  *   - OBK3      : must still return the data buffer with no OBKERR error.
  */
static hal_status_t FlashObkReadAccessCheck(const uint32_t *p_data, uint32_t data_word_count)
{
  hal_status_t status = HAL_ERROR;
  uint32_t     idx;

  (void)data_word_count; /* For visibility check we only use first word. */

  for (idx = 0U; idx < OBK_AREA_NB; idx++)
  {
    const obk_area_cfg_t *p_area = &ObkAreaInfo[idx];
    uint32_t              word0;

    word0 = *(__IO uint32_t *)(p_area->base_addr);

    if (idx < 2U)
    {
      /* OBK1 / OBK2 must be hidden at HDPL3. */
      if (word0 != 0x00000000UL)
      {
        PRINTF("  [ERROR] %s: expected RAZ at HDPL3, read=0x%08" PRIX32 ".\n",
               p_area->label, word0);
        goto _read_check_exit;
      }
    }
    else
    {
      /* OBK3 must remain visible at HDPL3. */
      if (word0 != p_data[0])
      {
        PRINTF(" [ERROR] %s: expected first word 0x%08" PRIX32
               " at HDPL3, read=0x%08" PRIX32 ".\n",
               p_area->label, p_data[0], word0);
        goto _read_check_exit;
      }
    }
  }

  status = HAL_OK;

_read_check_exit:
  return status;
}

static app_status_t check_obk_page_content(hal_flash_obk_page_t obk_page,
                                           uint32_t start_addr,
                                           const uint32_t *expected_data,
                                           uint32_t size_byte)
{
  uint32_t index;
  uint32_t word_count = size_byte / sizeof(uint32_t);

  /* Select the OBK page that must be exposed in the memory map for the readback loop. */
  if (HAL_FLASH_ITF_OBK_SetPage(HAL_FLASH,
                                (obk_page == HAL_FLASH_OBK_PAGE_ALTERNATE) ?
                                HAL_FLASH_OBK_PAGE_ALTERNATE : HAL_FLASH_OBK_PAGE_CURRENT) != HAL_OK)
  {
    return EXEC_STATUS_ERROR;
  }

  for (index = 0U; index < word_count; index++)
  {
    uint32_t data = *((volatile const uint32_t *)(start_addr + (index * sizeof(uint32_t))));
    uint32_t expected_value = expected_data[index];

    if (data != expected_value)
    {
      HAL_FLASH_ITF_OBK_SetPage(HAL_FLASH, HAL_FLASH_OBK_PAGE_CURRENT);
      return EXEC_STATUS_ERROR;
    }
  }

  HAL_FLASH_ITF_OBK_SetPage(HAL_FLASH, HAL_FLASH_OBK_PAGE_CURRENT);

  return EXEC_STATUS_OK;
}
