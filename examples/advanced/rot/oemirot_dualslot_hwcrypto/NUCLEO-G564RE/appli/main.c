/**
  ******************************************************************************
  * @file    main.c
  * @brief   Main application file.
  *          This application demonstrates Secure Services
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
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include "mx_hal_def.h"
#include "mx_system.h"
#include "main.h"
#include "com.h"
#include "common.h"
#include "data.h"
#include "Driver_Flash.h"
#include "mx_basic_stdio_app.h"
#include "fw_update_app.h"
#include "appli_flash_layout.h"

extern ARM_DRIVER_FLASH FLASH_PRIMARY_DEV_NAME;
extern ARM_DRIVER_FLASH FLASH_SECONDARY_DEV_NAME;

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define MPU_REGIONCOUNT 12U

/* Private variables ---------------------------------------------------------*/
uint8_t *pUserAppId;
const uint8_t UserAppId = 'A';
version_t ver;
/* External References -------------------------------------------------------*/
extern const VECTOR_TABLE_Type __VECTOR_TABLE[];
extern void SystemInit(void);
extern __NO_RETURN void __PROGRAM_START(void);
extern uint32_t __STACK_LIMIT;

/* Private function prototypes -----------------------------------------------*/
void FW_APP_PrintMainMenu(void);
void FW_APP_Run(void);
void read_version(void);
void display_version(const char *label, const version_t *ver);
static void mpu_disable_all_regions(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /**
    * When OEMIROT_FAST_WAKE_UP is enabled, the OEMiRoT relies on the hardware SBF (standby flag)
    * to skip firmware image verification. The SBF flag remains set when the application is entered
    * after a wake-up from standby mode. This allows the application to handle its own software
    * context restoration. To maintain a secure execution environment, the user application must
    * clear the SBF flag after it is processed.
    *
    * In this example, the SBF flag is simply cleared without any processing.
    */
  HAL_PWR_CleanPreviousSystemPowerMode();

  /* Set example to const : this const changes in binary without rebuild */
  pUserAppId = (uint8_t *)&UserAppId;

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  if (mx_system_init() != SYSTEM_OK)
  {
    return (-1);
  }

  /* Initialize basic_stdio separately, but after system init. */
  mx_basic_stdio_init();

  /* Initialize Flash */
  if (FLASH_PRIMARY_DEV_NAME.Initialize(NULL) != ARM_DRIVER_OK)
  {
    printf("Driver Flash Init : Failed");
  }

#if (FLASH_PRIMARY_DEV_NAME != FLASH_SECONDARY_DEV_NAME)
  if (FLASH_SECONDARY_DEV_NAME.Initialize(NULL) != ARM_DRIVER_OK)
  {
    printf("Driver External Flash Init : Failed");
  }
#endif /* FLASH_PRIMARY_DEV_NAME != FLASH_SECONDARY_DEV_NAME */

  printf("\r\n===========================================================");
  printf("\r\n=          (C) COPYRIGHT 2026 STMicroelectronics          =");
  printf("\r\n=                                                         =");
  printf("\r\n=                      Application #%c                     =", *pUserAppId);
  printf("\r\n===========================================================");
  printf("\r\n\r\n");

  /* Print versions of appli image and, if enabled, appli data image */
  read_version();

  printf("App SystemCoreClock: %" PRIu32 " Hz\r\n", SystemCoreClock);

  /* User App firmware runs*/
  FW_APP_Run();

  /* Infinite loop */

  while (1)
  {
  }
}

/**
  * @brief  Display the TEST Main Menu choices on HyperTerminal.
  * @param  None.
  * @retval None.
  */
void FW_APP_PrintMainMenu(void)
{
  printf("\r\n======================== Main menu ========================\r\n\n");
  printf("  New firmware image ------------------------------------ 1\r\n\n");
#if (DATA_IMAGE_NUMBER == 1)
  printf("  Read data image --------------------------------------- 2\r\n\n");
#endif /* (DATA_IMAGE_NUMBER == 1) */
  printf("  Selection : ");
}

/**
  * @brief  Display the TEST Main Menu choices on HyperTerminal.
  * @param  None.
  * @retval None.
  */
void FW_APP_Run(void)
{
  uint8_t key = 0U;

  /*##1- Print Main Menu message*/
  FW_APP_PrintMainMenu();

  while (1U)
  {
    /* Clean the input path */
    COM_Flush();

    /* Receive key */
    if (COM_Receive(&key, 1U, RX_TIMEOUT) == HAL_OK)
    {
      (void)printf("%c\r\n", key);
      switch (key)
      {
        case '1' :
          FW_UPDATE_Run();
          break;
#if (DATA_IMAGE_NUMBER == 1)
        case '2' :
          DATA_Run();
          break;
#endif /* (DATA_IMAGE_NUMBER == 1) */
        default:
          printf("Invalid number !\r");
          break;
      }

      /* Print Main Menu message */
      FW_APP_PrintMainMenu();
    }
  }
}

/**
  * @brief  Read the application image version and, if enabled, the data image version.
  * @param  None
  * @retval None
  */
void read_version(void)
{
  if (Driver_FLASH0.ReadData(FLASH_PRIMARY_APP_VERSION_OFFSET, &ver, sizeof(ver)) != ARM_DRIVER_OK)
  {
    printf("Failed to read app image version\r\n");
  }
  else
  {
    display_version("App image", &ver);
  }

#if (DATA_IMAGE_NUMBER == 1)
  if (Driver_FLASH0.ReadData(FLASH_PRIMARY_DATA_VERSION_OFFSET, &ver, sizeof(ver)) != ARM_DRIVER_OK)
  {
    printf("Failed to read data image version\r\n");
  }
  else
  {
    display_version("Data image", &ver);
  }
#endif /* (DATA_IMAGE_NUMBER == 1) */
}

/**
  * @brief  Display image version using the provided label.
  * @param  label Pointer to the string identifying the image.
  * @param  ver   Pointer to the version structure to display.
  * @retval None
  */
void display_version(const char *label, const version_t *ver)
{
  printf("%s version: %" PRIu32 ".%" PRIu32 ".%" PRIu32 "\r\n",
         label,
         (uint32_t)ver->iv_major,
         (uint32_t)ver->iv_minor,
         (uint32_t)ver->iv_revision);
}

/**
  * @brief  Disable all available MPU regions.
  * @param  None
  * @retval None
  */
static void mpu_disable_all_regions(void)
{
  uint32_t i;

  /* Disable MPU */
  HAL_CORTEX_MPU_Disable();

  /* Disable all regions */
  for (i = 0U; i < (uint32_t)MPU_REGIONCOUNT; i++)
  {
    HAL_CORTEX_MPU_DisableRegion((hal_cortex_mpu_region_idx_t)i);
  }
}

/**
  * @brief  Override the default post-system initialization hook.
  * @param  None
  * @retval SYSTEM_OK
  */
system_status_t pre_system_init_hook(void)
{
  /* Disable all MPU regions. */
  mpu_disable_all_regions();

  return SYSTEM_OK;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
__NO_RETURN void Error_Handler(void)
{
  while (1);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }

}
#endif /* USE_FULL_ASSERT */
