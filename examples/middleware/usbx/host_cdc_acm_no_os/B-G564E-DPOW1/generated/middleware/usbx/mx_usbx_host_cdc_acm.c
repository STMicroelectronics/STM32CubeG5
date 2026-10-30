/**
  ******************************************************************************
  * @file    mx_usbx_host_cdc_acm.c
  * @brief   USBX Host CDC ACM applicative source file
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
#include "mx_usbx_host_cdc_acm.h"
#include "mx_usbx_host.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define APP_RX_DATA_SIZE             2048U
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern UX_HOST_CLASS_CDC_ACM    *cdc_acm;

static struct
{
  UCHAR *buffer;
  ULONG size;
  UINT status;
} cdc_acm_reception_data;

/* UX Host CDC ACM Sending */
UCHAR user_tx_buffer[] = "USBX_STM32_Host_CDC_ACM \n";
volatile uint8_t ButtonState = 0;
volatile ULONG receive_dataflag = 0;
ULONG block_size;
/* UX Host CDC ACM Reception */
UX_HOST_CLASS_CDC_ACM_RECEPTION cdc_acm_reception;
static UCHAR                    UserRxBuffer[APP_RX_DATA_SIZE];
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief  usbh_cdc_acm_send_process
  * @param  none
  * @retval none
  */
void usbh_cdc_acm_send_process(VOID)
{
  UINT status;
  ULONG ux_actual_length;

  /* Check the CDC class state */
  if ((cdc_acm != NULL) && (cdc_acm -> ux_host_class_cdc_acm_state ==  UX_HOST_CLASS_INSTANCE_LIVE))
  {
    /* Wait until the joystick center button is pressed */
    if (ButtonState != 0U)
    {
      /* Start sending data */
      status = ux_host_class_cdc_acm_write(cdc_acm, user_tx_buffer,
                                           ux_utility_string_length_get(user_tx_buffer),
                                           &ux_actual_length);
      if (status == UX_SUCCESS)
      {
        USBH_USRLOG("Data sent successfully");
      }
      else
      {
        USBH_USRLOG("Unable to send data");
      }
      /* Reset the joystick center Button state */
      ButtonState = 0U;

    }
  }
}


/**
  * @brief  usbh_cdc_acm_receive_process
  * @param  none
  * @retval none
  */
void usbh_cdc_acm_receive_process(VOID)
{
  UINT     status;
  ULONG    count = 0;

  /* Check the CDC class state */
  if ((cdc_acm != NULL) && (cdc_acm -> ux_host_class_cdc_acm_state ==  UX_HOST_CLASS_INSTANCE_LIVE))
  {
    if (cdc_acm_reception.ux_host_class_cdc_acm_reception_state != UX_HOST_CLASS_CDC_ACM_RECEPTION_STATE_STARTED)
    {
      block_size = cdc_acm->ux_host_class_cdc_acm_bulk_in_endpoint->ux_endpoint_descriptor.wMaxPacketSize;

      /* Set the block size parameter reception */
      cdc_acm_reception.ux_host_class_cdc_acm_reception_block_size = block_size;

      /* Set the buffer for reception */
      cdc_acm_reception.ux_host_class_cdc_acm_reception_data_buffer = UserRxBuffer;

      /* Set the size of the data reception buffer */
      cdc_acm_reception.ux_host_class_cdc_acm_reception_data_buffer_size = sizeof(UserRxBuffer);

      /* Set the callback for each reception transfer completion */
      cdc_acm_reception.ux_host_class_cdc_acm_reception_callback = usbh_cdc_acm_reception_callback;

      /* Start reception */
      status = ux_host_class_cdc_acm_reception_start(cdc_acm, &cdc_acm_reception);

      /* Check status to starting data reception */
      if (status == UX_SUCCESS)
      {
        USBH_USRLOG(("Ready to receive data \n"));
      }
      else
      {
        USBH_USRLOG("Unable to start reception\n");
      }
    }

    else if (cdc_acm_reception.ux_host_class_cdc_acm_reception_state == UX_HOST_CLASS_CDC_ACM_RECEPTION_STATE_STARTED)
    {
      /* Wait until the requested flag NEW_RECEIVED_DATA is received */
      if (receive_dataflag == 1U)
      {
        /* Display the received data */
        UCHAR *p_buffer = cdc_acm_reception_data.buffer;
        for (count = 0; count < cdc_acm_reception_data.size; count++)
        {
          USBH_USRLOG("%c", *p_buffer);
          p_buffer++;
        }

        receive_dataflag = 0U;
      }
    }
  }
}

/**
  * @brief  usbh_cdc_acm_reception_callback
  * @param  none
  * @retval none
  */
void usbh_cdc_acm_reception_callback(struct UX_HOST_CLASS_CDC_ACM_STRUCT *cdc_acm,
                                     UINT status,
                                     UCHAR *reception_buffer,
                                     ULONG reception_size)
{

  /* check if tail has reached end of user buffer */
  if (cdc_acm_reception.ux_host_class_cdc_acm_reception_data_tail +
      cdc_acm_reception.ux_host_class_cdc_acm_reception_block_size >=
      cdc_acm_reception.ux_host_class_cdc_acm_reception_data_buffer +
      cdc_acm_reception.ux_host_class_cdc_acm_reception_data_buffer_size)
  {
    /* Move back to the beginning  */
    cdc_acm_reception.ux_host_class_cdc_acm_reception_data_tail =
      cdc_acm_reception.ux_host_class_cdc_acm_reception_data_buffer;

  }
  else
  {
    /* Program the tail to be after the current buffer */
    cdc_acm_reception.ux_host_class_cdc_acm_reception_data_tail +=
      cdc_acm_reception.ux_host_class_cdc_acm_reception_block_size;
  }

  cdc_acm_reception_data.buffer = reception_buffer;
  cdc_acm_reception_data.size = reception_size;

  receive_dataflag = 1U;
}

/** Defines the user callback function.
  * This function is triggered when the EXTI interrupt.
  */
void UserDetectedExtiCallback(hal_exti_handle_t *hexti, hal_exti_trigger_t trigger)
{
  STM32_UNUSED(trigger);
  ButtonState = BUTTON_PRESSED;
}
