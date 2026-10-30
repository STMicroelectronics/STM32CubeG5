/**
  ******************************************************************************
  * @file    mx_usbx_host_cdc_acm.h
  * @brief   USBX Host CDC ACM applicative header file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_usbx_license.md file
  * in the same directory as the generated code.
  * If no mx_usbx_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_USBX_HOST_CDC_ACM_H
#define MX_USBX_HOST_CDC_ACM_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_host_class_cdc_acm.h"
#include "mx_usbx_host.h"
#include "mx_hal_def.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define NEW_RECEIVED_DATA    0x01
#define NEW_DATA_TO_SEND     0x02
#define REMOVED_CDC_INSTANCE 0x03
#define BUTTON_PRESSED 1
/* Exported functions prototypes ---------------------------------------------*/
/* Exported defines -----------------------------------------------------------*/
void usbh_cdc_acm_send_thread_entry(void *args);
void usbh_cdc_acm_receive_thread_entry(void *args);
void usbh_cdc_acm_reception_callback(struct UX_HOST_CLASS_CDC_ACM_STRUCT *cdc_acm,
                                     UINT status, UCHAR *reception_buffer, ULONG reception_size);
void UserDetectedExtiCallback(hal_exti_handle_t *hexti, hal_exti_trigger_t trigger);

#ifdef __cplusplus
}
#endif
#endif  /* MX_USBX_HOST_CDC_ACM_H */
