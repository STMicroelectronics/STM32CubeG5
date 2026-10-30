/**
  ******************************************************************************
  * @file           : mx_play1.c
  * @brief          : PLAY1 Peripheral initialization
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
#include "mx_play1.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/
static hal_play_handle_t hPLAY1;

/******************************************************************************/
/* Exported functions for PLAY1 in HAL layer */
/******************************************************************************/
hal_play_handle_t *mx_play1_init(void)
{
  HAL_RCC_PLAY1_EnableClock();

  if (HAL_PLAY_Init(&hPLAY1, HAL_PLAY1) != HAL_OK)
  {
    return NULL;
  }

  if (HAL_RCC_PLAY1_SetKernelClkSource(HAL_RCC_PLAY1_CLK_SRC_LSI) != HAL_OK)
  {
    return NULL;
  }

  /* PLAY input configuration */
  hal_play_in_config_t in_config[1];

  /* Input: TIM2 Trigger output connected to PLAY1 input */
  in_config[0].source          = HAL_PLAY1_IN_TIM2_TRGO_CKTIM_MUX0;
  in_config[0].mode            = HAL_PLAY_EDGE_DETECTION_BYPASSED;
  in_config[0].min_pulse_width = 0;

  if (HAL_PLAY_INPUT_SetConfig(&hPLAY1, in_config, 1U) != HAL_OK)
  {
    return NULL;
  }

  /* PLAY LUT configuration */
  hal_play_lut_config_t lut_config[1];

  /* LUT0 configuration */
  lut_config[0].lut             = HAL_PLAY_LUT0;
  lut_config[0].truth_table     = 0xAAAA; /* Logical expression: A */
  lut_config[0].clk_gate_source = HAL_PLAY_LUT_CLK_GATE_OFF;

  lut_config[0].input_source[HAL_PLAY_LUT_INPUT0] = HAL_PLAY_LUT_INPUT_FILTER0; /* A */
  lut_config[0].input_source[HAL_PLAY_LUT_INPUT1] = HAL_PLAY_LUT_INPUT_DEFAULT; /* B - unused: input is SW trigger 15 */
  lut_config[0].input_source[HAL_PLAY_LUT_INPUT2] = HAL_PLAY_LUT_INPUT_DEFAULT; /* C - unused: input is SW trigger 15 */
  lut_config[0].input_source[HAL_PLAY_LUT_INPUT3] = HAL_PLAY_LUT_INPUT_DEFAULT; /* D - unused: input is SW trigger 15 */

  if (HAL_PLAY_LUT_SetConfig(&hPLAY1, lut_config, 1U) != HAL_OK)
  {
    return NULL;
  }

  /* PLAY output configuration */
  hal_play_out_config_t out_config[1];

  /* Output: Routed signal */
  /* Connect LUT0 (direct output) to PLAY output 0 */
  out_config[0].output_mux = HAL_PLAY_OUT0;
  out_config[0].lut_output = HAL_PLAY_LUT0_OUT_DIRECT;

  if (HAL_PLAY_OUTPUT_SetConfig(&hPLAY1, out_config, 1U) != HAL_OK)
  {
    return NULL;
  }

  /* ### PLAY1 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOD_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name]

       PD11    ------>   PLAY1_OUT0
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_NO;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_14;
  HAL_GPIO_Init(HAL_GPIOD, HAL_GPIO_PIN_11, &gpio_config);

  HAL_CORTEX_NVIC_SetPriority(PLAY1_IRQn, HAL_CORTEX_NVIC_PREEMP_PRIORITY_0, HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  HAL_CORTEX_NVIC_EnableIRQ(PLAY1_IRQn);

  return &hPLAY1;
}

void mx_play1_deinit(void)
{
  HAL_CORTEX_NVIC_DisableIRQ(PLAY1_IRQn);

  HAL_PLAY_DeInit(&hPLAY1);

  HAL_RCC_PLAY1_DisableClock();

  HAL_RCC_PLAY1_Reset();

  /* De-initialize all GPIOD pins associated with PLAY1 */
  HAL_GPIO_DeInit(HAL_GPIOD, HAL_GPIO_PIN_11);
}

hal_play_handle_t *mx_play1_gethandle(void)
{
  return &hPLAY1;
}

/******************************************************************************/
/*                     PLAY global non-secure interrupt OR play1_out14 through EXTI line                    */
/******************************************************************************/
void PLAY1_IRQHandler(void)
{
  HAL_PLAY_IRQHandler(&hPLAY1);
}
