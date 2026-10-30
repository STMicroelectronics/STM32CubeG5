/**
  ******************************************************************************
  * @file    mx_usbx_device_printer.c
  * @brief   USBX Device Printer applicative source file
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
#include "mx_usbx_device_printer.h"
#include "mx_usbx_device.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Define Printer device ID */
UCHAR usbd_printer_DeviceID [] = {
  "  "                             /* Length */
  "MFG:Generic;"                   /* Manufacturer */
  "MDL:Generic_/_Text_Only;"       /* Model */
  "CMD:1284.4;"                    /* Command Set */
  "CLS:PRINTER;"                   /* Class */
  "DES:Generic text only printer;" /* Description */
};
static volatile UX_DEVICE_CLASS_PRINTER *printer = UX_NULL;
volatile ULONG device_printer_port_status_value = 0x18U;
static UCHAR device_printer_buffer[512];
/* Private functions prototype -----------------------------------------------*/

/**
  * @brief  usbd_printer_get_deviceID
  *         Get the printer device ID.
  * @param  none
  * @retval usbd_printer_DeviceID : Pointer to printer device ID.
  */
UCHAR *usbd_printer_get_deviceID(VOID)
{
  ux_utility_short_put_big_endian(usbd_printer_DeviceID, sizeof(usbd_printer_DeviceID));
  return usbd_printer_DeviceID;
}

/**
  * @brief  usbd_printer_activate
  *         This function is called while inserting a printer device.
  * @param  printer_instance: Pointer to the printer class instance.
  * @retval none
  */
VOID usbd_printer_activate(VOID *printer_instance)
{
  if (printer == UX_NULL)
  {
    UX_DEVICE_CLASS_PRINTER *printer_local;
    ULONG port_status;

    /* Save the Printer instance */
    printer = (UX_DEVICE_CLASS_PRINTER *)printer_instance;
    printer_local = (UX_DEVICE_CLASS_PRINTER *)printer;
    port_status = device_printer_port_status_value;

    /* Set the printer port status */
    ux_device_class_printer_ioctl(printer_local,
                                  UX_DEVICE_CLASS_PRINTER_IOCTL_PORT_STATUS_SET,
                                  &port_status);
  }
  return;
}

/**
  * @brief  usbd_printer_deactivate
  *         This function is called while inserting a printer device.
  * @param  printer_instance: Pointer to the printer class instance.
  * @retval none
  */
VOID usbd_printer_deactivate(VOID *printer_instance)
{

  /* Reset the Printer instance */
  if ((VOID *)printer == printer_instance)
  {
    printer = UX_NULL;
  }

  return;
}

/**
  * @brief  usbd_printer_soft_reset
  *         This function is called while resetting a printer device.
  * @param  printer_instance: Pointer to the printer class instance.
  * @retval none
  */
VOID usbd_printer_soft_reset(VOID *printer_instance)
{
  UX_PARAMETER_NOT_USED(printer_instance);
  return;
}

/**
  * @brief  Function implementing usbd_printer_read_thread_entry.
  * @param  argument: Not used
  * @retval none
  */
VOID usbd_printer_read_thread_entry(void *argument)
{
  UX_PARAMETER_NOT_USED(argument);

  UINT status;
  UX_DEVICE_CLASS_PRINTER *printer_local;
  UX_SLAVE_DEVICE *device;
  ULONG actual_length;
  ULONG i;

  /* Get the pointer to the device */
  device = &_ux_system_slave->ux_system_slave_device;

  while (1)
  {
    printer_local = (UX_DEVICE_CLASS_PRINTER *)printer;
    /* Check if the device state already configured */
    if ((device->ux_slave_device_state == UX_DEVICE_CONFIGURED) && (printer_local != UX_NULL))
    {
      /* Reads from the printer device */
      status = ux_device_class_printer_read(printer_local,
                                            device_printer_buffer,
                                            sizeof(device_printer_buffer), &actual_length);
      if (status != UX_SUCCESS)
      {
        continue;
      }
      if (actual_length == 0)
      {
        continue;
      }

      /* Displaying received data */
      USBD_USRLOG("Printing %lu bytes:", actual_length);

      for (i = 0; i < actual_length; i ++)
      {
        if ((i & 0xF) == 0)
        {
          USBD_USRLOG("\n[%03lx]", i);
        }
        USBD_USRLOG(" %02X", device_printer_buffer[i]);
      }
      USBD_USRLOG("\n");
    }
    else
    {
      /* Sleep thread for 10 ms */
      ux_utility_thread_sleep(MS_TO_TICK(10));
    }
  }
}

/**
  * @brief  Function implementing usbd_printer_write_thread_entry.
  * @param  argument: Not used
  * @retval none
  */
VOID usbd_printer_write_thread_entry(void *argument)
{
  UX_PARAMETER_NOT_USED(argument);

  UX_DEVICE_CLASS_PRINTER *printer_local;
  ULONG port_status = device_printer_port_status_value;
  ULONG port_status_update;
  UX_SLAVE_DEVICE *device;

  /* Get the pointer to the device */
  device = &_ux_system_slave->ux_system_slave_device;

  while (1)
  {
    printer_local = (UX_DEVICE_CLASS_PRINTER *)printer;

    /* Check if the device state already configured and status change check */
    if ((device->ux_slave_device_state == UX_DEVICE_CONFIGURED) && (printer_local != UX_NULL) &&
        (port_status != device_printer_port_status_value))
    {
      port_status_update = device_printer_port_status_value;

      /* Set the printer port status */
      ux_device_class_printer_ioctl(printer_local,
                                    UX_DEVICE_CLASS_PRINTER_IOCTL_PORT_STATUS_SET,
                                    &port_status_update);

      /* Update port status value */
      port_status = port_status_update;
    }
    else
    {
      /* Sleep thread for 10 ms */
      ux_utility_thread_sleep(MS_TO_TICK(10));
    }
  }
}
