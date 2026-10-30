/**
  ******************************************************************************
  * @file    mx_usbx_device.c
  * @brief   USBX Device applicative source file
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

/* Includes ------------------------------------------------------------------*/
#include "mx_usbx_device.h"
#include "ux_dcd_stm32.h"
#include "mx_usbx_device_printer.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TaskHandle_t ux_device_app_thread;
TaskHandle_t ux_device_read_thread;
TaskHandle_t ux_device_write_thread;
extern USB_DEVICE_HANDLE            device_handle;
extern USBD_USER_STRING_DESC_HANDLE string_descriptor_handle;
hal_pcd_handle_t *p_usb_device = UX_NULL;
USB_DEVICE_ENDPOINT_HANDLE printer_endpoints_app[] =
{
  {
    USBD_PRINTER_EPIN_ADDR,
    USBD_PRINTER_EPIN_TYPE,
    USBD_PRINTER_EPIN_FS_MPS,
    USBD_PRINTER_EPIN_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_PRINTER_EPIN_HS_MPS,
    USBD_PRINTER_EPIN_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  },
  {
    USBD_PRINTER_EPOUT_ADDR,
    USBD_PRINTER_EPOUT_TYPE,
    USBD_PRINTER_EPOUT_FS_MPS,
    USBD_PRINTER_EPOUT_FS_BINTERVAL,
#if USBD_HIGH_SPEED_SUPPORTED == 1U
    USBD_PRINTER_EPOUT_HS_MPS,
    USBD_PRINTER_EPOUT_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};
USB_DEVICE_INTERFACE_HANDLE printer_interface_app[] =
{
  {
    USBD_PRINTER_ITF_NUMBERS,
    USBD_PRINTER_ITF_ATL_SETTING,
    USBD_PRINTER_ITF_EP_NUMBERS,
    USBD_PRINTER_ITF_CLASS,
    USBD_PRINTER_ITF_SUBCLASS,
    USBD_PRINTER_ITF_PROTOCOL,
    USBD_PRINTER_ITF_STR_DESC_IDX,
    0x00U,
    printer_endpoints_app,
    0x00U,
  }
};
/* Private function prototypes -----------------------------------------------*/
VOID app_ux_device_thread_entry(void *argument);
/**
  * @brief  Application USBX Device Initialization.
  */
UINT app_usbx_device_init(VOID)
{
  UINT status = UX_SUCCESS;
  USB_DESCRIPTOR                    usbd_desc;
  ULONG                             report_desc_len;
  UX_DEVICE_CLASS_PRINTER_PARAMETER printer_parameter = {0};
  report_desc_len = sizeof(printer_parameter);
  usb_device_descriptor_register_class(USBD_CLASS_TYPE_PRINTER, printer_interface_app, &report_desc_len);
  usb_device_descriptor_get_framework(&usbd_desc);

  /* Install the device portion of USBX */
  status = ux_device_stack_initialize(usbd_desc.device_high_speed.framework,
                                      usbd_desc.device_high_speed.framework_length,
                                      usbd_desc.device_full_speed.framework,
                                      usbd_desc.device_full_speed.framework_length,
                                      usbd_desc.string.framework,
                                      usbd_desc.string.framework_length,
                                      usbd_desc.languageid.framework,
                                      usbd_desc.languageid.framework_length,
                                      UX_NULL);

  if (status != UX_SUCCESS)
  {
    return status;
  }
  /* Initialize the printer class parameters for the device */
  printer_parameter.ux_device_class_printer_device_id           = usbd_printer_get_deviceID();
  printer_parameter.ux_device_class_printer_instance_activate   = usbd_printer_activate;
  printer_parameter.ux_device_class_printer_instance_deactivate = usbd_printer_deactivate;
  printer_parameter.ux_device_class_printer_soft_reset          = usbd_printer_soft_reset;
  /* Initialize the device printer class */
  status = ux_device_stack_class_register(_ux_system_device_class_printer_name,
                                          ux_device_class_printer_entry,
                                          0x01,
                                          printer_interface_app->interface_numbers,
                                          &printer_parameter);

  if (status != UX_SUCCESS)
  {
    return status;
  }
  /* Create the device application main thread */
  if (xTaskCreate(app_ux_device_thread_entry, UX_DEVICE_APP_THREAD_NAME,
                  UX_DEVICE_APP_THREAD_STACK_SIZE, NULL,
                  UX_DEVICE_APP_THREAD_PRIO, &ux_device_app_thread) != pdPASS)
  {
    return UX_THREAD_ERROR;
  }
  if (xTaskCreate(usbd_printer_write_thread_entry, "USBD_PRINTER_WRITE",
                  UX_DEVICE_APP_THREAD_STACK_SIZE, NULL,
                  UX_DEVICE_APP_THREAD_PRIO, &ux_device_write_thread) != pdPASS)
  {
    return UX_THREAD_ERROR;
  }
  if (xTaskCreate(usbd_printer_read_thread_entry, "USBD_PRINTER_READ",
                  UX_DEVICE_APP_THREAD_STACK_SIZE, NULL,
                  UX_DEVICE_APP_THREAD_PRIO, &ux_device_read_thread) != pdPASS)
  {
    return UX_THREAD_ERROR;
  }
  return UX_SUCCESS;
}


/**
  * @brief  Application USBX Device De-Initialization.
  * @retval none
  */
UINT app_usbx_device_deinit(VOID)
{
  UINT status = UX_SUCCESS;


  /* Unregister printer class. */
  status = ux_device_stack_class_unregister(_ux_system_device_class_printer_name, ux_device_class_printer_entry);
  if (status != UX_SUCCESS)
  {
    return status;
  }

  /* The code below is required for uninstalling the device portion of USBX.  */
  status = ux_device_stack_uninitialize();

  if (status != UX_SUCCESS)
  {
    return status;
  }
  return UX_SUCCESS;
}
/**
  * @brief  Function implementing app_ux_device_thread_entry.
  * @param  thread_input: User thread input parameter.
  * @retval none
  */
VOID app_ux_device_thread_entry(void *argument)
{
  UX_PARAMETER_NOT_USED(argument);
  UINT status = UX_SUCCESS;
  /* Initialization of USB device */
  if (p_usb_device == UX_NULL)
  {
    mx_example_pcd_init();
    /* initialize the device controller driver */
    p_usb_device = mx_example_pcd_gethandle();
    status = ux_dcd_stm32_initialize(0, (ULONG)p_usb_device);
    if (status != UX_SUCCESS)
    {
      return;
    }
  }
  /* sleep for 10ms */
  ux_utility_thread_sleep(MS_TO_TICK(10));
  /* One-shot init task can delete itself after USB device stack startup. */
  vTaskDelete(NULL);
}
