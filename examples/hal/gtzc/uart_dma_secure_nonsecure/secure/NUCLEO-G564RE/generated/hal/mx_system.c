/**
  ******************************************************************************
  * @file           : mx_system.c
  * @brief          : STM32 system program body
  *                   Applicative target system level initialization
  *                   (system clock, cache, TZ...) and system level peripherals
  *                   initialization. mx_system_init() service is called by the main
  *                   before jumping the example entry point.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_stm32g5xx_hal_drivers_license.md file
  * in the same directory as the generated code.
  * If no mx_stm32g5xx_hal_drivers_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "mx_system.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

system_status_t mx_system_init(void)
{
  if (pre_system_init_hook() != SYSTEM_OK)
  {
    return SYSTEM_PRESYSTEM_ERROR;
  }

  /*
    startup system section
  */
  if (HAL_Init() != HAL_OK)
  {
    return SYSTEM_STARTUP_ERROR;
  }

  /* Initialize RCC peripheral. */
  if (mx_rcc_init() != SYSTEM_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  /* Peripherals clock configuration and activation is centralized: no clock activation/deactivation in pppi_init. */
  if (mx_rcc_peripherals_clock_config() != SYSTEM_OK)
  {
    return SYSTEM_CLOCK_ERROR;
  }

  /* Trusted execution environment section. */
  if (mx_tee_setup() != SYSTEM_OK)
  {
    return SYSTEM_RESOURCES_ISOLATION_ERROR;
  }

  /*
    Interruptions section
  */
  if (mx_cortex_nvic_init() != SYSTEM_OK)
  {
    return SYSTEM_INTERRUPTS_ERROR;
  }

  /* CORTEX MPU initialization in case isolation is not activated. */
  if (mx_cortex_mpu_init() != SYSTEM_OK)
  {
    return SYSTEM_RESOURCES_ISOLATION_ERROR;
  }

  /* CORTEX SCB initialization. */
  if (mx_cortex_scb_init() != SYSTEM_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  /*
    Peripheral init section
  */

  if (post_system_init_hook() != SYSTEM_OK)
  {
    return SYSTEM_POSTSYSTEM_ERROR;
  }

  /* Suspend SysTick_S and jump to the non-secure application. */
  mx_start_application(NSECURE_APPLICATION_ADDR);

  return SYSTEM_OK;
}

void mx_start_application(uint32_t app_addr)
{
  typedef void (*funcptr_app_call_t)(void) CMSE_NS_CALL;
  funcptr_app_call_t app_reset_handler;

  /* Configure the non-secure vector table. */
  SCBNS_SetVTOR(app_addr);
  /* Set non-secure main stack into the stack pointer (MSP_NS). */
  __TZ_set_MSP_NS(*((uint32_t *)(app_addr)));
  /* Suspend SysTick_S. */
  HAL_SuspendTick();

  /* Get non-secure reset handler. */
  app_reset_handler = (funcptr_app_call_t)(*((uint32_t *)((app_addr) + 4U)));
  /* Start non-secure state software application. */
  app_reset_handler();
}

/******************************************************************************/
/*                            Systick Handler                                 */
/******************************************************************************/
/**
  * @brief   This function handles SysTick Handler
  * @warning Systick_Handler is generated as NVIC peripheral has not been activated
  *          By default it is generated in HAL.
  *          It must be located into mx_cortex_nvic.c file
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_CORTEX_SYSTICK_IRQHandler();
}

/**
  * @brief  User hook function called before the HAL_Init() function
  * @retval system_status_t System status
  */
__WEAK system_status_t pre_system_init_hook(void)
{
  /* NOTE : This function must not be modified. When the callback is needed,
            the pre_system_init_hook can be implemented in the user file
   */
  return SYSTEM_OK;
}

/**
  * @brief  User hook function called after the HAL_Init() and Peripheral init functions
  * @retval system_status_t System status
  */
__WEAK system_status_t post_system_init_hook(void)
{
  /* NOTE : This function must not be modified. When the callback is needed,
            the post_system_init_hook can be implemented in the user file
   */
  return SYSTEM_OK;
}
