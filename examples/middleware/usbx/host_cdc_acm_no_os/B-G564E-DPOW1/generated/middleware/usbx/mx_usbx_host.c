/**
  ******************************************************************************
  * @file    mx_usbx_host.c
  * @brief   USBX Host applicative source file
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
#include "mx_usbx_host.h"
#include "ux_hcd_stm32.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_hcd_handle_t *p_usb_host = UX_NULL;
UX_HOST_CLASS_CDC_ACM      *cdc_acm;
/* Private function prototypes -----------------------------------------------*/
static UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *current_class, VOID *current_instance);
static VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code);
/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Application USBX Host Initialization.
    * @retval status
  */
UINT app_usbx_host_init(VOID)
{
  UINT status = UX_SUCCESS;

  /* Install the host portion of USBX */
  status = ux_host_stack_initialize(ux_host_event_callback);
  if (status != UX_SUCCESS)
  {
    return status;
  }

  /* Register a callback error function */
  ux_utility_error_callback_register(&ux_host_error_callback);
  /* Register CDC ACM class. */
  status = ux_host_stack_class_register(_ux_system_host_class_cdc_acm_name, ux_host_class_cdc_acm_entry);
  if (status != UX_SUCCESS)
  {
    return status;
  }
  /* Register all the USB host controllers available in this system. */
  status = ux_host_stack_hcd_register(_ux_system_host_hcd_stm32_name,
                                      _ux_hcd_stm32_initialize, 0U,
                                      (ULONG)p_usb_host);
  if (status != UX_SUCCESS)
  {
    return status;
  }

  /* Start Application Message */
  USBH_USRLOG("**** USB Host **** \n");
  USBH_USRLOG("USB Host library started.\n");

  /* Wait for Device to be attached */
  USBH_USRLOG("Starting CDC ACM Application");
  USBH_USRLOG("Connect your CDC ACM Device");
  return UX_SUCCESS;
}

/**
  * @brief  Application USBX Host De-Initialization.
  * @retval none
  */
UINT app_usbx_host_deinit(VOID)
{
  UINT status = UX_SUCCESS;
  if (p_usb_host != UX_NULL)
  {
  /* Unregister all the USB host controllers available in this system. */
  ux_host_stack_hcd_unregister(_ux_system_host_hcd_stm32_name, 0U,
                               (ULONG)p_usb_host);
    p_usb_host = UX_NULL;
  }

  /* Unregister CDC ACM class. */
  status = ux_host_stack_class_unregister(ux_host_class_cdc_acm_entry);
  if (status != UX_SUCCESS)
  {
    return status;
  }

  /* The code below is required for uninstalling the host portion of USBX.  */
  status = ux_host_stack_uninitialize();

  if (status != UX_SUCCESS)
  {
    return status;
  }
  return UX_SUCCESS;
}

/**
  * @brief  ux_host_event_callback
  *         This callback is invoked to notify application of instance changes.
  * @param  event: event code.
  * @param  current_class: Pointer to class.
  * @param  current_instance: Pointer to class instance.
  * @retval status
  */
static UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *current_class, VOID *current_instance)
{
  UINT status = UX_SUCCESS;

  switch (event)
  {
    case UX_DEVICE_INSERTION:

      /* Get current CDC Class */
      if (current_class -> ux_host_class_entry_function == ux_host_class_cdc_acm_entry)
      {
        if (cdc_acm == UX_NULL)
        {
          /* Get current CDC Instance */
          cdc_acm = (UX_HOST_CLASS_CDC_ACM *)current_instance;

          /* Check if this is CDC DATA instance */
          if (cdc_acm -> ux_host_class_cdc_acm_bulk_in_endpoint == UX_NULL)
          {
            cdc_acm = UX_NULL;
          }
          else
          {
            USBH_USRLOG("USB CDC Device Found");
            USBH_USRLOG("PID: %#x ", (UINT)cdc_acm -> ux_host_class_cdc_acm_device -> ux_device_descriptor.idProduct);
            USBH_USRLOG("VID: %#x ", (UINT)cdc_acm -> ux_host_class_cdc_acm_device -> ux_device_descriptor.idVendor);
            USBH_USRLOG("Data Interface initialized");
          }
        }
      }

      break;

    case UX_DEVICE_REMOVAL:

      if ((VOID *)cdc_acm == current_instance)
      {
        /* Clear cdc instance */
        cdc_acm = UX_NULL;
        USBH_USRLOG("\nUSB CDC ACM Device Removal");
      }

      break;

    case UX_DEVICE_CONNECTION:

      break;

    case UX_DEVICE_DISCONNECTION:

      break;

    default:

      break;
  }

  return status;
}

/**
  * @brief ux_host_error_callback
  *         This callback is invoked to notify application of error changes.
  * @param  system_level: system level parameter.
  * @param  system_context: system context code.
  * @param  error_code: error event code.
  * @retval Status
  */
static VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code)
{
  switch (error_code)
  {
    case UX_DEVICE_ENUMERATION_FAILURE:
      USBH_USRLOG("USB Device Enumeration Failure");
      break;

    case  UX_NO_DEVICE_CONNECTED:
      USBH_USRLOG("USB Device disconnected");
      break;

    default:
      break;
  }
}
/**
  * @brief  Application USBX Host Process.
  * @retval none
  */
UINT app_usbx_host_process(VOID)
{
  UINT return_status = UX_SUCCESS;
  if (p_usb_host == UX_NULL)
  {
    mx_example_hcd_init();

    p_usb_host = mx_usb_drd_fs_host_gethandle();

    /* Register all the USB host controllers available in this system. */
    return_status = ux_host_stack_hcd_register(_ux_system_host_hcd_stm32_name,
                                               _ux_hcd_stm32_initialize, 0U,
                                               (ULONG)p_usb_host);

    if (return_status != UX_SUCCESS)
    {
      return return_status;
    }
  }
  return_status = ux_host_stack_tasks_run();
  usbh_cdc_acm_send_process();
  usbh_cdc_acm_receive_process();
  return return_status;
}
