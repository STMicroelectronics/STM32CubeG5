/**
  ******************************************************************************
  * @file    mx_usbx_device_printer.h
  * @brief   USBX Device Printer applicative header file
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
#ifndef MX_USBX_DEVICE_PRINTER_H
#define MX_USBX_DEVICE_PRINTER_H
#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "ux_api.h"
#include "ux_device_class_printer.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions prototypes -------------------------------------------- */
VOID usbd_printer_activate(VOID *printer_instance);
VOID usbd_printer_deactivate(VOID *printer_instance);
UCHAR *usbd_printer_get_deviceID(VOID);
VOID usbd_printer_soft_reset(VOID *printer_instance);
VOID usbd_printer_read_thread_entry(void *argument);
VOID usbd_printer_write_thread_entry(void *argument);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MX_USBX_DEVICE_PRINTER_H */
