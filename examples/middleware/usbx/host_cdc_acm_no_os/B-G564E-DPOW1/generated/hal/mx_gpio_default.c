/**
  ******************************************************************************
  * @file           : mx_gpio_default.c
  * @brief          : gpio_default Peripheral initialization
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
#include "mx_gpio_default.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported variables by reference -------------------------------------------*/
static hal_exti_handle_t hEXTI0;

/******************************************************************************/
/* Exported functions for GPIO in HAL layer                                   */
/******************************************************************************/
system_status_t mx_gpio_default_init(void)
{
  hal_gpio_config_t  gpio_config;

  HAL_RCC_GPIOA_EnableClock();

  /*
    GPIO pin labels :
    PA0   ---------> PA0, MX_EXAMPLE_EXTI_TRIGGER
    */
  /* Configure PA0 GPIO pin in input mode */
  gpio_config.mode            = HAL_GPIO_MODE_INPUT;
  gpio_config.pull            = HAL_GPIO_PULL_DOWN;
  if (HAL_GPIO_Init(PA0_PORT, PA0_PIN, &gpio_config) != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  hal_exti_config_t exti_config;

  /* Initialize the EXTI for line 0 */
  HAL_EXTI_Init(&hEXTI0, HAL_EXTI_LINE_0);

  /* Set the trigger as RISING for the GPIOA */
  exti_config.trigger   = HAL_EXTI_TRIGGER_RISING;
  exti_config.gpio_port = HAL_EXTI_GPIOA;
  HAL_EXTI_SetConfig(&hEXTI0, &exti_config);

  /* Set line 0 Interrupt priority */
  HAL_CORTEX_NVIC_SetPriority(EXTI0_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(EXTI0_IRQn);

  /*
    GPIO pin labels :
    PA5   ---------> PA5
    */
  /* Configure PA5 GPIO pin in output mode */
  gpio_config.mode            = HAL_GPIO_MODE_OUTPUT;
  gpio_config.speed           = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.pull            = HAL_GPIO_PULL_NO;
  gpio_config.output_type     = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.init_state      = PA5_INIT_STATE;
  if (HAL_GPIO_Init(PA5_PORT, PA5_PIN, &gpio_config) != HAL_OK)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  return SYSTEM_OK;
}

system_status_t mx_gpio_default_deinit(void)
{
  /* De-initialize the EXTI for GPIOA line0 */
  HAL_EXTI_DeInit(&hEXTI0);

  /* set line 0 Interrupt priority */
  HAL_CORTEX_NVIC_DisableIRQ(EXTI0_IRQn);

  /* De-initialize pins of GPIOA port */
  HAL_GPIO_DeInit(HAL_GPIOA, PA0_PIN | PA5_PIN);

  return SYSTEM_OK;
}

hal_exti_handle_t *mx_gpio_default_exti0_gethandle(void)
{
  return &hEXTI0;
}

/******************************************************************************/
/*                            EXTI Line0 interrupt                            */
/******************************************************************************/
void EXTI0_IRQHandler(void)
{
  HAL_EXTI_IRQHandler(&hEXTI0);
}
