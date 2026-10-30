/**
  ******************************************************************************
  * @file    usbpd_pdo_defs.c
  * @brief   USBPD PDO/APDO definition
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_usbpd_license.md file
  * in the root directory of this software component.
  * If no mx_usbpd_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_core.h"
/* Define   ------------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Definition of Source PDOs */
uint32_t USBPD_PWR_PDO_ListSRC[USBPD_PORT_COUNT][USBPD_MAX_NB_PDO] =
{
  {
    /* PDO 1 */
    (
      USBPD_PDO_TYPE_FIXED /* Fixed supply PDO */

      | USBPD_PDO_SRC_FIXED_SET_VOLTAGE(5000U) /* Voltage in mV */
      | USBPD_PDO_SRC_FIXED_SET_MAX_CURRENT(3000U) /* Max current in mA */
      | USBPD_PDO_SRC_FIXED_PEAKCURRENT_EQUAL /* Peak Current info */

      /* Common definitions applicable to all PDOs, defined only in PDO 1 */
      | USBPD_PDO_SRC_FIXED_UNCHUNK_NOT_SUPPORTED /* Unchunked Extended Messages */
      | USBPD_PDO_SRC_FIXED_DRD_NOT_SUPPORTED /* Dual-Role Data */
      | USBPD_PDO_SRC_FIXED_USBCOMM_NOT_SUPPORTED /* USB Communications */
      | USBPD_PDO_SRC_FIXED_EXT_POWER_NOT_AVAILABLE /* External Power */
      | USBPD_PDO_SRC_FIXED_USBSUSPEND_NOT_SUPPORTED /* USB Suspend Supported */
      | USBPD_PDO_SRC_FIXED_DRP_SUPPORTED /* Dual-Role Power */
    ),

    /* PDO 2 */ (0x00000000U),

    /* PDO 3 */ (0x00000000U),

    /* PDO 4 */ (0x00000000U),

    /* PDO 5 */ (0x00000000U),

    /* PDO 6 */ (0x00000000U),

    /* PDO 7 */ (0x00000000U),
  }
};

/* Definition of Sink PDOs */
uint32_t USBPD_PWR_PDO_ListSNK[USBPD_PORT_COUNT][USBPD_MAX_NB_PDO] =
{
  {
    /* PDO 1 */
    (
      USBPD_PDO_TYPE_FIXED /* Fixed supply PDO */

      | USBPD_PDO_SNK_FIXED_SET_VOLTAGE(5000U) /* Voltage in mV */
      | USBPD_PDO_SNK_FIXED_SET_OP_CURRENT(1500U) /* Operating current in mA */

      /* Common definitions applicable to all PDOs, defined only in PDO 1 */
      | USBPD_PDO_SNK_FIXED_FRS_NOT_SUPPORTED /* Fast Role Swap */
      | USBPD_PDO_SNK_FIXED_DRD_NOT_SUPPORTED /* Dual-Role Data */
      | USBPD_PDO_SNK_FIXED_USBCOMM_NOT_SUPPORTED /* USB Communications */
      | USBPD_PDO_SNK_FIXED_EXT_POWER_NOT_AVAILABLE /* External Power */
      | USBPD_PDO_SNK_FIXED_HIGHERCAPAB_NOT_SUPPORTED /* Higher Capability */
      | USBPD_PDO_SNK_FIXED_DRP_SUPPORTED /* Dual-Role Power */
    ),

    /* PDO 2 */ (0x00000000U),

    /* PDO 3 */ (0x00000000U),

    /* PDO 4 */ (0x00000000U),

    /* PDO 5 */ (0x00000000U),

    /* PDO 6 */ (0x00000000U),

    /* PDO 7 */ (0x00000000U),
  }
};
