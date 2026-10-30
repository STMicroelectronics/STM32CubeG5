/**
  ******************************************************************************
  * @file    boot_nv_counters.c
  * @brief   This file provides all the Non volatile firmware functions.
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
#include <string.h>
#include "plat_nv_counters.h"
#include "plat_defs.h"
#include "low_level_obkeys.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum plat_err_t plat_err_Typedef;
typedef enum nv_counter_t nv_counter_Typedef;

/* Private configuration  ----------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Exported function ---------------------------------------------------------*/
/* Function prototypes are defined in MCUBoot mdw (plat_nv_counters.h), naming convention can not be respected */

/**
  * @brief  Initialize nv counter storage block if not formatted.
  *         Check consistency of nv counter storage
  *         Initialize global variable for write access.
  *
  * @retval plat_err_Typedef
  *           - PLAT_ERR_SUCCESS in case of success
  *           - PLAT_ERR_SYSTEM_ERR in case of error
  */
plat_err_Typedef plat_init_nv_counter(void)
{
  return PLAT_ERR_SUCCESS;
}

/**
  * @brief  Writes/updates counter data in NV security counter area.
  * @param  counter_id to be written
  * @param  value 32bits data to be written
  * @param  updated Pointer to cnt updated status flag (1: yes, 0: no)
  * @warning This function is not reentrant
  * @retval plat_err_Typedef
  *           PLAT_ERR_SUCCESS on write successfully done.
              PLAT_ERR_SYSTEM_ERR when max updateable value written
  */
plat_err_Typedef plat_set_nv_counter(nv_counter_Typedef counter_id,
                                     uint32_t value, uint32_t *updated)
{
  uint32_t current = 0U;

  /* reset updated flag */
  *updated = 0U;

  if (OBK_GetNVCounter(counter_id, (uint32_t *) &current) != HAL_OK)
  {
    return PLAT_ERR_SYSTEM_ERR;
  }

  /* never decrement the counter */
  if (current > value)
  {
    return PLAT_ERR_SYSTEM_ERR;
  }
  else if (value > current)
  {
    if (OBK_UpdateNVCounter(counter_id, value) != HAL_OK)
    {
      return PLAT_ERR_SYSTEM_ERR;
    }
    /* set updated flag */
    *updated = 1U;
  }
  else
  {
    return PLAT_ERR_SUCCESS;
  }

  return PLAT_ERR_SUCCESS;
}

/**
  * @brief  Read counter data in NV security counter area.
  * @param  counter_id to be read
  * @param  size counter size to be read
  * @param  val reference data to retrieve read counter value
  * @retval plat_err_Typedef
  *           PLAT_ERR_SUCCESS on write successfully done.
              PLAT_ERR_SYSTEM_ERR when max updateable value written
  */
plat_err_Typedef plat_read_nv_counter(nv_counter_Typedef counter_id,
                                      uint32_t size, uint32_t *val)
{
  /* counter is encoded uint32_t */
  if (OBK_GetNVCounter(counter_id, val) == HAL_OK)
  {
    if (*val != 0xFFFFFFFFU)
    {
      return PLAT_ERR_SUCCESS;
    }
  }
  return PLAT_ERR_SYSTEM_ERR;
}
