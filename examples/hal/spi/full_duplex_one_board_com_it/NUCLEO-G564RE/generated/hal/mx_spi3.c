/**
  ******************************************************************************
  * @file           : mx_spi3.c
  * @brief          : SPI3 Peripheral initialization
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
#include "mx_spi3.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_spi_handle_t hSPI3;

/******************************************************************************/
/* Exported functions for SPI in HAL layer */
/******************************************************************************/
hal_spi_handle_t *mx_spi3_init(void)
{
  hal_spi_config_t spi_config;

  if (HAL_SPI_Init(&hSPI3, HAL_SPI3) != HAL_OK)
  {
    return NULL;
  }

  HAL_RCC_SPI3_EnableClock();

  if (HAL_RCC_SPI3_SetKernelClkSource(HAL_RCC_SPI3_CLK_SRC_PCLK1) != HAL_OK)
  {
    return NULL;
  }

  spi_config.mode = HAL_SPI_MODE_SLAVE;
  spi_config.direction = HAL_SPI_DIRECTION_FULL_DUPLEX;
  spi_config.data_width = HAL_SPI_DATA_WIDTH_8_BIT;
  spi_config.clock_polarity = HAL_SPI_CLOCK_POLARITY_LOW;
  spi_config.clock_phase = HAL_SPI_CLOCK_PHASE_1_EDGE;
  spi_config.baud_rate_prescaler = HAL_SPI_BAUD_RATE_PRESCALER_2;
  spi_config.first_bit = HAL_SPI_MSB_FIRST;
  spi_config.nss_pin_management = HAL_SPI_NSS_PIN_MGMT_INTERNAL;

  if (HAL_SPI_SetConfig(&hSPI3, &spi_config) != HAL_OK)
  {
    return NULL;
  }

  /* ### SPI3 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOB_EnableClock();

  HAL_RCC_GPIOC_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name]

       PB0     ------>   SPI3_SCK
       PB1     ------>   SPI3_MISO
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_MEDIUM;
  gpio_config.alternate   = HAL_GPIO_AF_9;
  HAL_GPIO_Init(HAL_GPIOB, HAL_GPIO_PIN_0 | HAL_GPIO_PIN_1, &gpio_config);

  /**
    [GPIO Pin] ------> [Signal Name]

       PC12    ------>   SPI3_MOSI
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_MEDIUM;
  gpio_config.alternate   = HAL_GPIO_AF_6;
  HAL_GPIO_Init(HAL_GPIOC, HAL_GPIO_PIN_12, &gpio_config);

  /* Enable the interrupt for SPI */
  HAL_CORTEX_NVIC_SetPriority(SPI3_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(SPI3_IRQn);

  return &hSPI3;
}

void mx_spi3_deinit(void)
{
  /* Disable the interrupt for SPI */
  HAL_CORTEX_NVIC_DisableIRQ(SPI3_IRQn);

  (void)HAL_SPI_DeInit(&hSPI3);

  HAL_RCC_SPI3_Reset();

  HAL_RCC_SPI3_DisableClock();

  /* De-initialize all GPIOB pins associated with SPI3 */
  HAL_GPIO_DeInit(HAL_GPIOB, HAL_GPIO_PIN_0 | HAL_GPIO_PIN_1);

  /* De-initialize all GPIOC pins associated with SPI3 */
  HAL_GPIO_DeInit(HAL_GPIOC, HAL_GPIO_PIN_12);
}

hal_spi_handle_t *mx_spi3_gethandle(void)
{
  return &hSPI3;
}

/******************************************************************************/
/*                           SPI3 global interrupt                            */
/******************************************************************************/
void SPI3_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&hSPI3);
}
