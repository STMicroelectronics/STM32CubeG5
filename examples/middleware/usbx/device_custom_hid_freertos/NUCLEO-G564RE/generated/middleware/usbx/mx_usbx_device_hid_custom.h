/**
  ******************************************************************************
  * @file    mx_usbx_device_hid_custom.h
  * @brief   USBX Device Custom HID applicative header file
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
#ifndef MX_USBX_DEVICE_HID_CUSTOM_H
#define MX_USBX_DEVICE_HID_CUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_device_class_hid.h"
#include "ux_stm32_device_descriptors_config.h"
#include "mx_led.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define LED1_REPORT_ID            0x01
#define LED1_REPORT_COUNT         0x01

#define KEY_REPORT_ID             0x05
/* Exported functions prototypes ---------------------------------------------*/
VOID usbd_custom_hid_activate(VOID *hid_instance);
VOID usbd_custom_hid_deactivate(VOID *hid_instance);
UINT usbd_custom_hid_set_feature(UX_SLAVE_CLASS_HID *hid_instance, UX_SLAVE_CLASS_HID_EVENT *hid_event);
UINT usbd_custom_hid_get_report(UX_SLAVE_CLASS_HID *hid_instance, UX_SLAVE_CLASS_HID_EVENT *hid_event);
VOID usbd_custom_hid_set_report(struct UX_SLAVE_CLASS_HID_STRUCT *hid_instance);
UCHAR *usbd_custom_hid_report_address(VOID);
ULONG usbd_custom_hid_report_length(VOID);
ULONG usbd_custom_hid_report_id(VOID);
ULONG usbd_custom_hid_EventMaxLength(VOID);
ULONG usbd_custom_hid_EventMaxNumber(VOID);

VOID usbd_custom_hid_thread_entry(VOID *args);
void UserDetectedExtiCallback(hal_exti_handle_t *hexti, hal_exti_trigger_t trigger);
VOID usbd_custom_hid_led_control(UINT event_idx, UINT state);

/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif  /* MX_USBX_DEVICE_HID_CUSTOM_H */
