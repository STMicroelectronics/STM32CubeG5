/**
  ******************************************************************************
  * @file    mx_usbx_device_hid_custom.c
  * @brief   USBX Device Custom HID applicative source file
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
#include "mx_usbx_device_hid_custom.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UX_SLAVE_CLASS_HID      *p_HID_custom;
UX_SLAVE_CLASS_HID_EVENT hid_event;
UX_DEVICE_CLASS_HID_RECEIVED_EVENT hid_received_event;
extern QueueHandle_t ux_hid_msgqueue;
extern volatile uint8_t ButtonState;

/* Exported variables --------------------------------------------------------*/
UCHAR usbd_custom_hid_report_descriptor[] =
{
  0x06, 0xFF, 0x00,             /* Usage Page (Vendor Page: 0xFF00)     */
  0x09, 0x01,                   /* Usage (Demo Kit)                     */
  0xa1, 0x01,                   /* Collection (Application)             */
  0x85, LED1_REPORT_ID,     /*   Report id (1)                      */
  0x09, 0x01,                   /*   Usage (LED 1)                      */
  0x15, 0x00,                   /*   Logical Minimum (0)                */
  0x25, 0x01,                   /*   Logical Maximum (1)                */
  0x75, 0x08,                   /*   Report Size (8)                    */
  0x95, LED1_REPORT_COUNT,  /*   Report Count (1)                   */
  0xB1, 0x82,                   /*   Feature (Data,Var,Abs,Vol)         */
  0x85, LED1_REPORT_ID,     /*   Report id (1)                      */
  0x09, 0x01,                   /*   Usage (LED 1)                      */
  0x91, 0x82,                   /*   Output (Data,Var,Abs,Vol)          */
  0x85, KEY_REPORT_ID,          /*   Report id (4)                      */
  0x09, 0x05,                   /*   Usage (Push Button)                */
  0x15, 0x00,                   /*   Logical Minimum (0)                */
  0x25, 0x01,                   /*   Logical Maximum (1)                */
  0x75, 0x01,                   /*   Report Size (1)                    */
  0x81, 0x82,                   /*   Input (Data,Var,Abs,Vol)           */
  0x09, 0x05,                   /*   Usage (Push Button)                */
  0x75, 0x01,                   /*   Report Size (1)                    */
  0xb1, 0x82,                   /*   Feature (Data,Var,Abs,Vol)         */
  0x75, 0x07,                   /*   Report Size (7)                    */
  0x81, 0x83,                   /*   Input (Cnst,Var,Abs,Vol)           */
  0x85, KEY_REPORT_ID,          /*   Report id (2)                      */
  0x75, 0x07,                   /*   Report Size (7)                    */
  0xb1, 0x83,                   /*   Feature (Cnst,Var,Abs,Vol)         */
  0xc0                          /* End Collection                       */
};
/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  usbd_custom_hid_activate
  *         This function is called when insertion of a Custom HID device.
  * @param  hid_instance: Pointer to the hid class instance.
  * @retval none
  */
VOID usbd_custom_hid_activate(VOID *hid_instance)
{
  /* Save the custom HID instance */
  if (p_HID_custom == UX_NULL)
  {
    p_HID_custom = (UX_SLAVE_CLASS_HID *) hid_instance;
  }

  return;
}

/**
  * @brief  usbd_custom_hid_deactivate
  *         This function is called when extraction of a Custom HID device.
  * @param  hid_instance: Pointer to the hid class instance.
  * @retval none
  */
VOID usbd_custom_hid_deactivate(VOID *hid_instance)
{
  /* Reset the custom HID instance */
  if (p_HID_custom == hid_instance)
  {
    p_HID_custom = UX_NULL;
  }

  return;
}

/**
  * @brief  usbd_custom_hid_set_feature
  *         This function is invoked when the host sends a HID SET_REPORT
  *         to the application over Endpoint 0 (Set Feature).
  * @param  hid_instance: Pointer to the hid class instance.
  * @param  hid_event: Pointer to structure of the hid event.
  * @retval status
  */
UINT usbd_custom_hid_set_feature(UX_SLAVE_CLASS_HID *hid_instance,
                                UX_SLAVE_CLASS_HID_EVENT *hid_event)
{
  UINT status = UX_SUCCESS;

  UX_PARAMETER_NOT_USED(hid_instance);

  UINT event_idx, state;

  /* get LED report number */
  event_idx = hid_event->ux_device_class_hid_event_report_id;

  /* get LED states from hid event buffer */
  state = hid_event->ux_device_class_hid_event_buffer[0];

  usbd_custom_hid_led_control(event_idx, state);

  return status;
}

/**
  * @brief  usbd_custom_hid_get_report
  *         This function is invoked when host is requesting event through
  *         control GET_REPORT request.
  * @param  hid_instance: Pointer to the hid class instance.
  * @param  hid_event: Pointer to structure of the hid event.
  * @retval status
  */
UINT usbd_custom_hid_get_report(UX_SLAVE_CLASS_HID *hid_instance,
                                UX_SLAVE_CLASS_HID_EVENT *hid_event)
{
  UINT status = UX_SUCCESS;

  UX_PARAMETER_NOT_USED(hid_instance);
  UX_PARAMETER_NOT_USED(hid_event);

  return status;
}

/**
  * @brief  usbd_custom_hid_set_report
  *         This function is invoked when a HID OUT event is received.
  * @param  hid_instance: Pointer to the hid class instance.
  * @retval none
  */
VOID usbd_custom_hid_set_report(struct UX_SLAVE_CLASS_HID_STRUCT *hid_instance)
{
  UX_PARAMETER_NOT_USED(hid_instance);

  UINT event_idx, state;

  ux_utility_memory_set(&hid_received_event, 0, sizeof(UX_DEVICE_CLASS_HID_RECEIVED_EVENT));

  if(ux_device_class_hid_receiver_event_get(p_HID_custom, &hid_received_event) == UX_SUCCESS)
  {
    /* Get LED report number */
    event_idx = hid_received_event.ux_device_class_hid_received_event_data[0];

    /* Get LED states from hid event buffer */
    state = hid_received_event.ux_device_class_hid_received_event_data[1];

    /* Toggle Leds */
    usbd_custom_hid_led_control(event_idx, state);

    /* Free hid received event */
    ux_device_class_hid_receiver_event_free(p_HID_custom);
  }

  return;
}

/**
  * @brief  usbd_custom_hid_EventMaxNumber
  *         This function to set receiver event max number parameter.
  * @param  none
  * @retval receiver event max number
  */
ULONG usbd_custom_hid_EventMaxNumber(VOID)
{
  ULONG max_number = 0U;

  max_number = 1;

  return max_number;
}

/**
  * @brief  usbd_custom_hid_EventMaxLength
  *         This function to set receiver event max length parameter.
  * @param  none
  * @retval receiver event max length
  */
ULONG usbd_custom_hid_EventMaxLength(VOID)
{
  ULONG max_length = 0U;

  /* Set max event length */
  max_length = USBD_HID_CUSTOM_EPOUT_FS_MPS;
  return max_length;
}
/**
  * @brief  usbd_custom_hid_report_address
  *         Return the device HID MOUSE Report Descriptor
  * @retval Pointer to HID Report Descriptor buffer
  */
UCHAR *usbd_custom_hid_report_address(VOID)
{
  return usbd_custom_hid_report_descriptor;
}

/**
  * @brief  usbd_custom_hid_report_length
  *         Return the device HID Report Descriptor
  * @retval Size of HID Report Descriptor buffer
  */
ULONG usbd_custom_hid_report_length(VOID)
{
  return sizeof(usbd_custom_hid_report_descriptor);
}

/**
  * @brief  usbd_custom_hid_report_id
  *         Return the device HID Report ID Support
  * @retval UX_FALSE / UX_TRUE
  */
ULONG usbd_custom_hid_report_id(VOID)
{
  return UX_TRUE;
}

/**
  * @brief  Function implementing usbd_custom_hid_thread_entry.
  *         Run custom HID task
  * @param  none
  * @retval none
  */
VOID usbd_custom_hid_thread_entry(VOID *args)
{
  UX_SLAVE_DEVICE *device;
  UX_SLAVE_CLASS_HID_EVENT hid_event;

  device = &_ux_system_slave->ux_system_slave_device;

  ux_utility_memory_set(&hid_event, 0, sizeof(UX_SLAVE_CLASS_HID_EVENT));

  while (1)
  {
    /* Check if the device state already configured */
    if ((device->ux_slave_device_state == UX_DEVICE_CONFIGURED) && (p_HID_custom != UX_NULL))
    {
      xQueueReceive(ux_hid_msgqueue, &hid_event, portMAX_DELAY);

      /* Send hid event */
      ux_device_class_hid_event_set(p_HID_custom, &hid_event);
    }
    else
    {
      /* Sleep thread for 10ms */
      ux_utility_thread_sleep(MS_TO_TICK(10));
    }
  }
}

/** Defines the user callback function.
  * This function is triggered when the EXTI interrupt.
  */
void UserDetectedExtiCallback(hal_exti_handle_t *hexti, hal_exti_trigger_t trigger)
{
  /* Set hid event length */
  hid_event.ux_device_class_hid_event_length = 1;

  /* Set button report id */
  hid_event.ux_device_class_hid_event_report_id = KEY_REPORT_ID;

  if(HAL_GPIO_ReadPin(PC13_PORT, PC13_PIN)== HAL_GPIO_PIN_SET)
  {
    hid_event.ux_device_class_hid_event_buffer[0] = 0x01;
  }
  else
  {
    hid_event.ux_device_class_hid_event_buffer[0] = 0x00;
  }

  xQueueSend(ux_hid_msgqueue, &hid_event, 0);
}

/**
  * @brief  usbd_custom_hid_led_control
  * @retval none
  */
VOID usbd_custom_hid_led_control(UINT event_idx, UINT state)
{
  switch (event_idx)
  {
    case LED1_REPORT_ID:
      (state == 1) ? led_on(LED1) : led_off(LED1);
      break;

    default:
      led_off(LED1);
      break;
  }
}
