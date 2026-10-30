/**
  ******************************************************************************
  * @file           : mx_usart1.c
  * @brief          : USART1 Peripheral initialization
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
#include "mx_usart1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
/* Handle for USART */
static hal_usart_handle_t hUSART1;

/* Exported function definition ----------------------------------------------*/
/******************************************************************************/
/* Exported functions for USART in HAL layer */
/******************************************************************************/
hal_usart_handle_t *mx_usart1_usart_init(void)
{
  hal_usart_config_t usart_config;

  if (HAL_USART_Init(&hUSART1, HAL_USART1) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_USART1_EnableClock();

  if (HAL_RCC_USART1_SetKernelClkSource(HAL_RCC_USART1_CLK_SRC_PCLK2) != HAL_OK)
  {
    return NULL;
  }

  usart_config.clock_prescaler = HAL_USART_PRESCALER_DIV1;
  usart_config.word_length = HAL_USART_WORD_LENGTH_8_BIT;
  usart_config.stop_bits = HAL_USART_STOP_BIT_1;
  usart_config.parity = HAL_USART_PARITY_NONE;
  usart_config.direction = HAL_USART_DIRECTION_TX_RX;
  usart_config.clock_polarity = HAL_USART_CLOCK_POLARITY_LOW;
  usart_config.clock_phase = HAL_USART_CLOCK_PHASE_1_EDGE;
  usart_config.clock_last_bit = HAL_USART_CLOCK_LAST_BIT_ENABLED;
  usart_config.mode = HAL_USART_MODE_MASTER;
  usart_config.baud_rate = 115200U;

  if (HAL_USART_SetConfig(&hUSART1, &usart_config) != HAL_OK)
  {
    return NULL;
  }

  /* ### USART1 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOA_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name]

       PA7     ------>   USART1_CK
       PA10    ------>   USART1_RX
       PA9     ------>   USART1_TX
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_LOW;
  gpio_config.alternate   = HAL_GPIO_AF_7;
  HAL_GPIO_Init(HAL_GPIOA, HAL_GPIO_PIN_7 | HAL_GPIO_PIN_10 | HAL_GPIO_PIN_9, &gpio_config);

  /* Enable USART instance interrupt */
  HAL_CORTEX_NVIC_SetPriority(USART1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(USART1_IRQn);

  return &hUSART1;
}

void mx_usart1_usart_deinit(void)
{
  /* Disable USART instance interrupt */
  HAL_CORTEX_NVIC_DisableIRQ(USART1_IRQn);

  /* De-initialize the USART instance */
  (void)HAL_USART_DeInit(&hUSART1);

  HAL_RCC_USART1_Reset();

  HAL_RCC_USART1_DisableClock();

  /* De-initialize all GPIOA pins associated with USART1 */
  HAL_GPIO_DeInit(HAL_GPIOA, HAL_GPIO_PIN_7 | HAL_GPIO_PIN_9 | HAL_GPIO_PIN_10);
}

hal_usart_handle_t *mx_usart1_usart_gethandle(void)
{
  return &hUSART1;
}

/******************************************************************************/
/*                          USART1 global interrupt                           */
/******************************************************************************/
void USART1_IRQHandler(void)
{
  HAL_USART_IRQHandler(&hUSART1);
}
