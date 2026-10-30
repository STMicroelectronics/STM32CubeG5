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
UX_HOST_CLASS_CDC_ACM          *cdc_acm;
UX_HOST_CLASS_CDC_ACM          *cdc_acm_control;
UX_EVENT_FLAGS_GROUP ux_app_EventFlag;

hal_hcd_handle_t *p_usb_host = UX_NULL;
TaskHandle_t ux_host_app_thread;
TaskHandle_t cdc_acm_send_thread_handle;
TaskHandle_t cdc_acm_recieve_thread_handle;
/* Private function prototypes -----------------------------------------------*/
VOID app_ux_host_thread_entry(void *argument);
static UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *current_class, VOID *current_instance);
static VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code);
/* Exported functions --------------------------------------------------------*/
/** ########## Step 3 ##########
  *  Initialization of USBX CDC ACM Host
  */
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
  /* Create the host application main thread */
  if (xTaskCreate(app_ux_host_thread_entry,UX_HOST_APP_THREAD_NAME,
                  UX_HOST_APP_THREAD_STACK_SIZE, NULL,
                  UX_HOST_APP_THREAD_PRIO, &ux_host_app_thread) != pdPASS)
  {
    return UX_THREAD_ERROR;
  }

    /* Create the cdc_acm send thread */
  status = xTaskCreate(usbh_cdc_acm_send_thread_entry, "cdc_acm send thread entry",
                       UX_HOST_APP_THREAD_STACK_SIZE, NULL,
                       UX_HOST_APP_THREAD_PRIO, &cdc_acm_send_thread_handle);
  if (status != pdPASS)
  {
    return UX_ERROR;
  }

  /* Create the cdc_acm receive thread */
  status = xTaskCreate(usbh_cdc_acm_receive_thread_entry, "cdc_acm receive thread entry",
                       UX_HOST_APP_THREAD_STACK_SIZE, NULL,
                       UX_HOST_APP_THREAD_PRIO, &cdc_acm_recieve_thread_handle);
  if (status != pdPASS)
  {
    return UX_ERROR;
  }


  /* Create the event flags group */
  status = _ux_os_utility_event_flags_create(&ux_app_EventFlag, "Event Flag");
  if (status != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Start Application Message */
  USBH_USRLOG("**** USB OTG HS CDC ACM Host **** \n");
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
  UX_HOST_CLASS_CDC_ACM          *temp_cdc_acm = NULL;

  switch (event)
  {
    case UX_DEVICE_INSERTION:

      /* Get current CDC Class */
      if (current_class -> ux_host_class_entry_function == ux_host_class_cdc_acm_entry)
      {
        /* Get current CDC Instance */
        temp_cdc_acm = (UX_HOST_CLASS_CDC_ACM *)current_instance;

        /* Save DATA interface instance (used for bulk IN/OUT transfers). */
        if ((cdc_acm == UX_NULL) && (temp_cdc_acm -> ux_host_class_cdc_acm_bulk_in_endpoint != UX_NULL))
        {
          USBH_USRLOG("USB CDC Device Found");
          USBH_USRLOG("PID: %#x ", (UINT)temp_cdc_acm -> ux_host_class_cdc_acm_device -> ux_device_descriptor.idProduct);
          USBH_USRLOG("VID: %#x ", (UINT)temp_cdc_acm -> ux_host_class_cdc_acm_device -> ux_device_descriptor.idVendor);
          USBH_USRLOG("Data Interface initialized");

          cdc_acm = temp_cdc_acm;
        }
      }

      break;

    case UX_DEVICE_REMOVAL:

      if ((VOID *)cdc_acm == current_instance)
      {
        /* Clear cdc instance */
        cdc_acm = UX_NULL;
        USBH_USRLOG("\nUSB CDC ACM Device Removal");

        /* Set NEW_RECEIVED_DATA flag */
        if (_ux_os_utility_event_flags_set(&ux_app_EventFlag, 1, UX_OR) != UX_SUCCESS)
        {
          USBH_ERRLOG("Error: Failed to set the event flag.\n");
          vTaskDelay(pdMS_TO_TICKS(10));
        }
      }

      if ((VOID *)cdc_acm_control == current_instance)
      {
        cdc_acm_control = UX_NULL;
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
  * @brief  Function implementing app_ux_host_thread_entry.
  * @param  thread_input: User thread input parameter.
  * @retval none
  */
VOID app_ux_host_thread_entry(void *argument)
{
  UINT status = UX_SUCCESS;


  UX_PARAMETER_NOT_USED(argument);

  /* Initialize the usbx system component */
  mx_example_hcd_init();

  p_usb_host = mx_usb_drd_fs_host_gethandle();

  /* Register all the USB host controllers available in this system. */
  status = ux_host_stack_hcd_register(_ux_system_host_hcd_stm32_name,
                                      _ux_hcd_stm32_initialize, 0U,
                                      (ULONG)p_usb_host);

  if (status != UX_SUCCESS)
  {
    return;
  }

  /* Suspend the USB host thread */
  vTaskSuspend(ux_host_app_thread);

}
