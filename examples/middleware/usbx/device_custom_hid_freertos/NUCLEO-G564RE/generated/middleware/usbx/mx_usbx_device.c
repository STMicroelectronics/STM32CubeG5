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
#include "mx_usbx_device_hid_custom.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_pcd_handle_t *p_usb_device = UX_NULL;
TaskHandle_t ux_device_app_thread;
TaskHandle_t usbd_custom_hid_thread;
QueueHandle_t ux_hid_msgqueue;
USB_DEVICE_ENDPOINT_HANDLE hid_custom_endpoint_app[] =
{
  {
    USBD_HID_CUSTOM_EPIN_ADDR,
    USBD_HID_CUSTOM_EPIN_TYPE,
    USBD_HID_CUSTOM_EPIN_FS_MPS,
    USBD_HID_CUSTOM_EPIN_FS_BINTERVAL,
#if (USBD_HIGH_SPEED_SUPPORTED == 1U)
    USBD_HID_CUSTOM_EPIN_HS_MPS,
    USBD_HID_CUSTOM_EPIN_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  },
  {
    USBD_HID_CUSTOM_EPOUT_ADDR,
    USBD_HID_CUSTOM_EPOUT_TYPE,
    USBD_HID_CUSTOM_EPOUT_FS_MPS,
    USBD_HID_CUSTOM_EPOUT_FS_BINTERVAL,
#if (USBD_HIGH_SPEED_SUPPORTED == 1U)
    USBD_HID_CUSTOM_EPOUT_HS_MPS,
    USBD_HID_CUSTOM_EPOUT_HS_BINTERVAL,
#endif /* USBD_HIGH_SPEED_SUPPORTED */
  }
};

USB_DEVICE_INTERFACE_HANDLE hid_custom_interface_app[] =
{
  {
    USBD_HID_CUSTOM_ITF_NUMBERS,
    USBD_HID_CUSTOM_ITF_ATL_SETTING,
    USBD_HID_CUSTOM_ITF_EP_NUMBERS,
    USBD_HID_CUSTOM_ITF_CLASS,
    USBD_HID_CUSTOM_ITF_SUBCLASS,
    USBD_HID_CUSTOM_ITF_PROTOCOL,
    USBD_HID_CUSTOM_ITF_STR_DESC_IDX,
    0x00U,
    hid_custom_endpoint_app,
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
  UX_SLAVE_CLASS_HID_PARAMETER      custom_hid_parameter = {0};
  report_desc_len = usbd_custom_hid_report_length();
  usb_device_descriptor_register_class(USBD_CLASS_TYPE_HID, hid_custom_interface, &report_desc_len);

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
  /* Initialize the hid custom class parameters for the device */
  custom_hid_parameter.ux_slave_class_hid_instance_activate                    = usbd_custom_hid_activate;
  custom_hid_parameter.ux_slave_class_hid_instance_deactivate                  = usbd_custom_hid_deactivate;
  custom_hid_parameter.ux_device_class_hid_parameter_report_address            = usbd_custom_hid_report_address();
  custom_hid_parameter.ux_device_class_hid_parameter_report_length             = usbd_custom_hid_report_length();
  custom_hid_parameter.ux_device_class_hid_parameter_report_id                 = usbd_custom_hid_report_id();
  custom_hid_parameter.ux_device_class_hid_parameter_callback                  = usbd_custom_hid_set_feature;
  custom_hid_parameter.ux_device_class_hid_parameter_get_callback              = usbd_custom_hid_get_report;
#ifdef UX_DEVICE_CLASS_HID_INTERRUPT_OUT_SUPPORT
  custom_hid_parameter.ux_device_class_hid_parameter_receiver_initialize       = ux_device_class_hid_receiver_initialize;
  custom_hid_parameter.ux_device_class_hid_parameter_receiver_event_max_number = usbd_custom_hid_EventMaxNumber();
  custom_hid_parameter.ux_device_class_hid_parameter_receiver_event_max_length = usbd_custom_hid_EventMaxLength();
  custom_hid_parameter.ux_device_class_hid_parameter_receiver_event_callback   = usbd_custom_hid_set_report;
#endif /* UX_DEVICE_CLASS_HID_INTERRUPT_OUT_SUPPORT */

  /* Initialize the device hid custom class */
  status = ux_device_stack_class_register(_ux_system_slave_class_hid_name,
                                          ux_device_class_hid_entry,
                                          0x01,
                                          hid_custom_interface_app->interface_numbers,
                                          &custom_hid_parameter);

  if (status != UX_SUCCESS)
  {
    return status;
  }
  /* Create the device application main thread */
  if (xTaskCreate(app_ux_device_thread_entry,UX_DEVICE_APP_THREAD_NAME,
                  UX_DEVICE_APP_THREAD_STACK_SIZE, NULL,
                  UX_DEVICE_APP_THREAD_PRIO, &ux_device_app_thread) != pdPASS)
  {
    return UX_THREAD_ERROR;
  }
  /* Create usbd_custom_hid_thread_entry.  */
  status = xTaskCreate(usbd_custom_hid_thread_entry, "usbd custom hid thread entry", APP_DEFAULT_STACK_SIZE,
                       (VOID *) NULL, UX_DEVICE_APP_THREAD_PRIO, &usbd_custom_hid_thread);
  if (status != pdPASS)
  {
    return UX_ERROR;
  }
  ux_hid_msgqueue = xQueueCreate(sizeof(ULONG) ,sizeof(UX_SLAVE_CLASS_HID_EVENT));

  if (ux_hid_msgqueue == NULL)
  {
    return UX_ERROR;
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

  if (p_usb_device != UX_NULL)
  {
  /* Unregister USB device controller. */
  status = _ux_dcd_stm32_uninitialize(0, (ULONG)p_usb_device);

  if (status != UX_SUCCESS)
  {
    return status;
  }
    p_usb_device = UX_NULL;
  }

  /* Unregister hid class. */
  status = ux_device_stack_class_unregister(_ux_system_slave_class_hid_name, ux_device_class_hid_entry);
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
  UINT status = UX_SUCCESS;

  UX_PARAMETER_NOT_USED(argument);

  /* Initialization of USB device */
  if (p_usb_device == UX_NULL)
  {
    /* Initialization of USB device */
    p_usb_device = mx_usb_drd_fs_device_gethandle();

    /* Initialize the device controller driver */
    status = ux_dcd_stm32_initialize(0, (ULONG)p_usb_device);
    if (status != UX_SUCCESS)
    {
      return;
    }
  }
  /* Suspend the USB device thread */
  vTaskSuspend(ux_device_app_thread);
}
