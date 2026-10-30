/**
  ******************************************************************************
  * @file    data.c
  * @brief   data module.
  *          This file provides examples of Data access.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "data.h"
#include "com.h"
#include "appli_flash_layout.h"
#include "Driver_Common.h"
#include "low_level_obkeys.h"

/* Private define ------------------------------------------------------------*/
#define OBKEY_READ_BUFFER_SIZE      16U
#define OBKEY_DISPLAY_START_INDEX   DATA_IMAGE_HEADER_SIZE
#define OBKEY_DISPLAY_END_INDEX     (OBKEY_DISPLAY_START_INDEX  + OBKEY_READ_BUFFER_SIZE)

#if (DATA_IMAGE_NUMBER == 1)

static void DATA_Print_menu(void);
static void DATA_Display(void);

/**
  * @brief  Display the Data Main Menu choices on HyperTerminal.
  * @param  None.
  * @retval None.
  */
void DATA_Run(void)
{
  uint8_t key = 0;
  uint8_t exit = 0;

  /* Print Data welcome message */
  DATA_Print_menu();

  while (exit == 0U)
  {
    key = 0U;

    /* Clean the user input path */
    COM_Flush();

    /* Receive key */
    if (COM_Receive(&key, 1U, COM_UART_TIMEOUT_MAX) == HAL_OK)
    {
      (void)printf("%c\r\n\n", key);
      switch (key)
      {
        case '1' :
          DATA_Display();
          break;
        case 'x':
          exit = 1;
          break;
        default:
          printf("Invalid number !\r");
          break;
      }
      DATA_Print_menu();
    }
  }
}

/**
  * @brief  Display the Data Menu choices on HyperTerminal.
  * @param  None.
  * @retval None.
  */
static void DATA_Print_menu(void)
{
  printf("\r\n======================== Data image menu ==================\r\n\n");
  printf("  Display data from data image -------------------------- 1\r\n\n");
  printf("  Previous menu ----------------------------------------- x\r\n\n");
  printf("  Selection : ");
}

/**
  * @brief  Read and display OBKey data bytes.
  * @note   Reads OBKey data from the primary data slot and displays the
  *         configured byte range defined by OBKEY_DISPLAY_START_INDEX and
  *         OBKEY_DISPLAY_END_INDEX.
  * @retval None
  */
static void DATA_Display(void)
{
  uint8_t start_buffer[DATA_IMAGE_HEADER_SIZE + OBKEY_READ_BUFFER_SIZE];
  uint8_t i;

  /* Read the configured number of OBKey bytes from the primary data slot */
  if (OBK_Read(FLASH_PRIMARY_DATA_SLOT_OFFSET, start_buffer, sizeof(start_buffer)) != ARM_DRIVER_OK)
  {
    printf("\r\n Error reading data bytes\r\n");
    return;
  }

  /* display the configured range of data bytes */
  printf("  -- Data from OBKey: ");
  for (i = OBKEY_DISPLAY_START_INDEX; i < OBKEY_DISPLAY_END_INDEX; i++)
  {
    printf("%02X", start_buffer[i]);
  }
  printf("..\r\n");
}
#endif /* (DATA_IMAGE_NUMBER == 1) */
