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
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define APP_RX_DATA_SIZE             2048U
#define BLOCK_SIZE                   64U
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern UX_HOST_CLASS_CDC_ACM    *cdc_acm;
extern UX_HOST_CLASS_CDC_ACM    *cdc_acm_control;
extern UX_EVENT_FLAGS_GROUP ux_app_EventFlag;
volatile uint8_t ButtonState = 0;

/* UX Host CDC ACM Sending */
UCHAR                           UserTxBuffer[] = "USBX_STM32_Host_CDC_ACM \n";
ULONG                           tx_actual_length;
/* UX Host CDC ACM Reception */
UX_HOST_CLASS_CDC_ACM_RECEPTION cdc_acm_reception;
ULONG                           block_reception_count;
uint8_t                         block_reception_size[APP_RX_DATA_SIZE / BLOCK_SIZE];
uint16_t                        RxSzeIdx;
static UCHAR                    UserRxBuffer[APP_RX_DATA_SIZE];
/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  usbh_cdc_acm_send_thread_entry
  * @param  none
  * @retval none
  */
void usbh_cdc_acm_send_thread_entry(void *args)
{
  UINT status;
  ULONG send_dataflag = 0;

  while (1)
  {

    if ((cdc_acm != NULL) && (cdc_acm -> ux_host_class_cdc_acm_state ==  UX_HOST_CLASS_INSTANCE_LIVE))
    {
      /* Wait until the requested flag NEW_DATA_TO_SEND is received */
      if (_ux_os_utility_event_flags_get(&ux_app_EventFlag, 2, UX_OR_CLEAR,
                                         &send_dataflag, UX_WAIT_FOREVER) != UX_SUCCESS)
      {
        USBH_ERRLOG("Error: Failed to get the event flag.\n");
        vTaskDelay(pdMS_TO_TICKS(10));
      }

      if (ButtonState != 0U)
      {
        /* Start sending data */
        status = _ux_host_class_cdc_acm_write(cdc_acm, UserTxBuffer,
                                              ux_utility_string_length_get(UserTxBuffer),
                                              &tx_actual_length);
        if (status == UX_SUCCESS)
        {
          USBH_USRLOG("Data sent successfully");
        }
        else
        {
          USBH_USRLOG("Unable to send data");
        }
        /* Reset User Button state */
        ButtonState = 0U;
      }

    }
    else
    {
      /* Pause execution for a while */
      vTaskDelay(pdMS_TO_TICKS(10));
    }
  }
}


/**
  * @brief  usbh_cdc_acm_receive_thread_entry
  * @param  none
  * @retval none
  */
void usbh_cdc_acm_receive_thread_entry(void *args)
{
  UINT     status;
  ULONG    receive_dataflag = 0;
  UCHAR    *read_data_pointer = NULL;
  ULONG    read_block_count = 0;
  uint16_t read_data_block_count = 0;
  uint16_t count = 0;

  while (1)
  {
    /* Check the CDC class state */
    if ((cdc_acm != NULL) && (cdc_acm -> ux_host_class_cdc_acm_state ==  UX_HOST_CLASS_INSTANCE_LIVE))
    {
      if (cdc_acm_reception.ux_host_class_cdc_acm_reception_state != UX_HOST_CLASS_CDC_ACM_RECEPTION_STATE_STARTED)
      {
        /* Get a pointer to the USB user buffer reception */
        read_data_pointer = UserRxBuffer;

        /* Set the block size parameter reception */
        cdc_acm_reception.ux_host_class_cdc_acm_reception_block_size = BLOCK_SIZE;

        /* Set the buffer for reception */
        cdc_acm_reception.ux_host_class_cdc_acm_reception_data_buffer = (UCHAR *)UserRxBuffer;

        /* Set the size of the data reception buffer */
        cdc_acm_reception.ux_host_class_cdc_acm_reception_data_buffer_size = APP_RX_DATA_SIZE;

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
          vTaskDelay(pdMS_TO_TICKS(10));
        }
      }

      else if (cdc_acm_reception.ux_host_class_cdc_acm_reception_state == UX_HOST_CLASS_CDC_ACM_RECEPTION_STATE_STARTED)
      {
        /* Wait until the requested flag NEW_RECEIVED_DATA is received */
        if (_ux_os_utility_event_flags_get(&ux_app_EventFlag, 1, UX_OR_CLEAR,
                                           &receive_dataflag, UX_WAIT_FOREVER) != UX_SUCCESS)
        {
          USBH_ERRLOG("Error: Failed to get the event flag.\n");

          vTaskDelay(pdMS_TO_TICKS(10));
        }
        while ((read_block_count < block_reception_count) && (cdc_acm != NULL))
        {
          /* Check if read_data_pointer reached end of user buffer */
          if (read_data_pointer >=
              cdc_acm_reception.ux_host_class_cdc_acm_reception_data_buffer +
              cdc_acm_reception.ux_host_class_cdc_acm_reception_data_buffer_size)
          {
            read_data_pointer = cdc_acm_reception.ux_host_class_cdc_acm_reception_data_buffer;

            /* Reinitialize block reception size index */
            read_data_block_count = 0;
          }

          /* Display the received data */
          for (count = 0; count < block_reception_size[read_data_block_count]; count++)
          {
            USBH_USRLOG("%c", *read_data_pointer);
            read_data_pointer++;
          }

          /* Move to next block reception */
          read_block_count++;

          /* Move to the next block reception buffer */
          read_data_pointer += (BLOCK_SIZE - count);

          /* Move to the next block reception size */
          read_data_block_count++;
        }
      }
    }
    else
    {

      if (cdc_acm_reception.ux_host_class_cdc_acm_reception_state == UX_HOST_CLASS_CDC_ACM_RECEPTION_STATE_STOPPED)
      {
        read_data_pointer = cdc_acm_reception.ux_host_class_cdc_acm_reception_data_buffer;

        /* Reinitialize block reception size index */
        read_data_block_count = 0;
      }
      /* Pause execution for a while */
      vTaskDelay(pdMS_TO_TICKS(10));
    }
  }
}

/**
  * @brief  usbh_cdc_acm_reception_callback
  * @param  none
  * @retval none
  */
void usbh_cdc_acm_reception_callback(struct UX_HOST_CLASS_CDC_ACM_STRUCT *cdc_acm,
                                     UINT status, UCHAR *reception_buffer, ULONG reception_size)
{
  /* Block reception count */
  block_reception_count++;

  /* Save block reception size */
  block_reception_size[RxSzeIdx] = reception_size;

  /* Move to the next block reception size */
  RxSzeIdx++;

  /* check if tail has reached end of user buffer */
  if (cdc_acm_reception.ux_host_class_cdc_acm_reception_data_tail +
      cdc_acm_reception.ux_host_class_cdc_acm_reception_block_size >=
      cdc_acm_reception.ux_host_class_cdc_acm_reception_data_buffer +
      cdc_acm_reception.ux_host_class_cdc_acm_reception_data_buffer_size)
  {
    /* Move back to the beginning  */
    cdc_acm_reception.ux_host_class_cdc_acm_reception_data_tail =
      cdc_acm_reception.ux_host_class_cdc_acm_reception_data_buffer;

    /* Reinitialize reception block size index */
    RxSzeIdx = 0U;
  }
  else
  {
    /* Program the tail to be after the current buffer */
    cdc_acm_reception.ux_host_class_cdc_acm_reception_data_tail +=
      cdc_acm_reception.ux_host_class_cdc_acm_reception_block_size;
  }

  /* Set NEW_RECEIVED_DATA flag */
  if (_ux_os_utility_event_flags_set(&ux_app_EventFlag, 1, UX_OR) != UX_SUCCESS)
  {

    USBH_ERRLOG("Error: Failed to set the event flag.\n");

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

/** Defines the user callback function.
  * This function is triggered when the EXTI interrupt.
  */
void UserDetectedExtiCallback(hal_exti_handle_t *hexti, hal_exti_trigger_t trigger)
{
  STM32_UNUSED(trigger);
  /* Wait until the requested flag NEW_DATA_TO_SEND is received */
  if (_ux_os_utility_event_flags_set(&ux_app_EventFlag, NEW_DATA_TO_SEND, UX_OR) != UX_SUCCESS)
  {
    USBH_ERRLOG("Error: Failed to get the event flag.\n");
    vTaskDelay(pdMS_TO_TICKS(10));
  }
  ButtonState = BUTTON_PRESSED;
}
