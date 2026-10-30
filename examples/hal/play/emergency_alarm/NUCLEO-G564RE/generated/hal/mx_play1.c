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

static hal_exti_handle_t hEXTI51;

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

  /* Input: User button */
  in_config[0].source          = HAL_PLAY1_IN_IN4_MUX4;
  in_config[0].mode            = HAL_PLAY_EDGE_DETECTION_RISING;
  in_config[0].min_pulse_width = 0;

  if (HAL_PLAY_INPUT_SetConfig(&hPLAY1, in_config, 1U) != HAL_OK)
  {
    return NULL;
  }

  /* PLAY LUT configuration */
  hal_play_lut_config_t lut_config[13];

  /* LUT0 configuration */
  lut_config[0].lut             = HAL_PLAY_LUT0;
  lut_config[0].truth_table     = 0x5555; /* Logical expression: !A */
  lut_config[0].clk_gate_source = HAL_PLAY_LUT_CLK_GATE_LUT15_OUT_REGISTERED;

  lut_config[0].input_source[HAL_PLAY_LUT_INPUT0] = HAL_PLAY_LUT_INPUT_LUT0_OUT_REGISTERED; /* A */
  lut_config[0].input_source[HAL_PLAY_LUT_INPUT1] = HAL_PLAY_LUT_INPUT_DEFAULT; /* B - unused: input is SW trigger 15 */
  lut_config[0].input_source[HAL_PLAY_LUT_INPUT2] = HAL_PLAY_LUT_INPUT_DEFAULT; /* C - unused: input is SW trigger 15 */
  lut_config[0].input_source[HAL_PLAY_LUT_INPUT3] = HAL_PLAY_LUT_INPUT_DEFAULT; /* D - unused: input is SW trigger 15 */

  /* LUT1 configuration */
  lut_config[1].lut             = HAL_PLAY_LUT1;
  lut_config[1].truth_table     = 0x6666; /* Logical expression: A ^ B */
  lut_config[1].clk_gate_source = HAL_PLAY_LUT_CLK_GATE_LUT15_OUT_REGISTERED;

  lut_config[1].input_source[HAL_PLAY_LUT_INPUT0] = HAL_PLAY_LUT_INPUT_LUT1_OUT_REGISTERED; /* A */
  lut_config[1].input_source[HAL_PLAY_LUT_INPUT1] = HAL_PLAY_LUT_INPUT_LUT0_OUT_REGISTERED; /* B */
  lut_config[1].input_source[HAL_PLAY_LUT_INPUT2] = HAL_PLAY_LUT_INPUT_DEFAULT; /* C - unused: input is SW trigger 15 */
  lut_config[1].input_source[HAL_PLAY_LUT_INPUT3] = HAL_PLAY_LUT_INPUT_DEFAULT; /* D - unused: input is SW trigger 15 */

  /* LUT2 configuration */
  lut_config[2].lut             = HAL_PLAY_LUT2;
  lut_config[2].truth_table     = 0x6A6A; /* Logical expression: A ^ (B & C) */
  lut_config[2].clk_gate_source = HAL_PLAY_LUT_CLK_GATE_LUT15_OUT_REGISTERED;

  lut_config[2].input_source[HAL_PLAY_LUT_INPUT0] = HAL_PLAY_LUT_INPUT_LUT2_OUT_REGISTERED; /* A */
  lut_config[2].input_source[HAL_PLAY_LUT_INPUT1] = HAL_PLAY_LUT_INPUT_LUT1_OUT_REGISTERED; /* B */
  lut_config[2].input_source[HAL_PLAY_LUT_INPUT2] = HAL_PLAY_LUT_INPUT_LUT0_OUT_REGISTERED; /* C */
  lut_config[2].input_source[HAL_PLAY_LUT_INPUT3] = HAL_PLAY_LUT_INPUT_DEFAULT; /* D - unused: input is SW trigger 15 */

  /* LUT3 configuration */
  lut_config[3].lut             = HAL_PLAY_LUT3;
  lut_config[3].truth_table     = 0x6AAA; /* Logical expression: A ^ (B & C & D) */
  lut_config[3].clk_gate_source = HAL_PLAY_LUT_CLK_GATE_LUT15_OUT_REGISTERED;

  lut_config[3].input_source[HAL_PLAY_LUT_INPUT0] = HAL_PLAY_LUT_INPUT_LUT3_OUT_REGISTERED; /* A */
  lut_config[3].input_source[HAL_PLAY_LUT_INPUT1] = HAL_PLAY_LUT_INPUT_LUT2_OUT_REGISTERED; /* B */
  lut_config[3].input_source[HAL_PLAY_LUT_INPUT2] = HAL_PLAY_LUT_INPUT_LUT1_OUT_REGISTERED; /* C */
  lut_config[3].input_source[HAL_PLAY_LUT_INPUT3] = HAL_PLAY_LUT_INPUT_LUT0_OUT_REGISTERED; /* D */

  /* LUT4 configuration */
  lut_config[4].lut             = HAL_PLAY_LUT4;
  lut_config[4].truth_table     = 0x8000; /* Logical expression: A & B & C & D */
  lut_config[4].clk_gate_source = HAL_PLAY_LUT_CLK_GATE_ON;

  lut_config[4].input_source[HAL_PLAY_LUT_INPUT0] = HAL_PLAY_LUT_INPUT_LUT3_OUT_REGISTERED; /* A */
  lut_config[4].input_source[HAL_PLAY_LUT_INPUT1] = HAL_PLAY_LUT_INPUT_LUT2_OUT_REGISTERED; /* B */
  lut_config[4].input_source[HAL_PLAY_LUT_INPUT2] = HAL_PLAY_LUT_INPUT_LUT1_OUT_REGISTERED; /* C */
  lut_config[4].input_source[HAL_PLAY_LUT_INPUT3] = HAL_PLAY_LUT_INPUT_LUT0_OUT_REGISTERED; /* D */

  /* LUT5 configuration */
  lut_config[5].lut             = HAL_PLAY_LUT5;
  lut_config[5].truth_table     = 0x6666; /* Logical expression: A ^ B */
  lut_config[5].clk_gate_source = HAL_PLAY_LUT_CLK_GATE_LUT15_OUT_REGISTERED;

  lut_config[5].input_source[HAL_PLAY_LUT_INPUT0] = HAL_PLAY_LUT_INPUT_LUT5_OUT_REGISTERED; /* A */
  lut_config[5].input_source[HAL_PLAY_LUT_INPUT1] = HAL_PLAY_LUT_INPUT_LUT4_OUT_DIRECT; /* B */
  lut_config[5].input_source[HAL_PLAY_LUT_INPUT2] = HAL_PLAY_LUT_INPUT_DEFAULT; /* C - unused: input is SW trigger 15 */
  lut_config[5].input_source[HAL_PLAY_LUT_INPUT3] = HAL_PLAY_LUT_INPUT_DEFAULT; /* D - unused: input is SW trigger 15 */

  /* LUT8 configuration */
  lut_config[6].lut             = HAL_PLAY_LUT8;
  lut_config[6].truth_table     = 0x1151; /* Logical expression: (!B & !A) | (!D & C & !A) */
  lut_config[6].clk_gate_source = HAL_PLAY_LUT_CLK_GATE_OFF;

  lut_config[6].input_source[HAL_PLAY_LUT_INPUT0] = HAL_PLAY_LUT_INPUT_LUT0_OUT_REGISTERED; /* A */
  lut_config[6].input_source[HAL_PLAY_LUT_INPUT1] = HAL_PLAY_LUT_INPUT_LUT2_OUT_REGISTERED; /* B */
  lut_config[6].input_source[HAL_PLAY_LUT_INPUT2] = HAL_PLAY_LUT_INPUT_LUT3_OUT_REGISTERED; /* C */
  lut_config[6].input_source[HAL_PLAY_LUT_INPUT3] = HAL_PLAY_LUT_INPUT_LUT5_OUT_REGISTERED; /* D */

  /* LUT9 configuration */
  lut_config[7].lut             = HAL_PLAY_LUT9;
  lut_config[7].truth_table     = 0x31; /* Logical expression: (!D & !B & !A) | (!D & C & !B) */
  lut_config[7].clk_gate_source = HAL_PLAY_LUT_CLK_GATE_OFF;

  lut_config[7].input_source[HAL_PLAY_LUT_INPUT0] = HAL_PLAY_LUT_INPUT_LUT0_OUT_REGISTERED; /* A */
  lut_config[7].input_source[HAL_PLAY_LUT_INPUT1] = HAL_PLAY_LUT_INPUT_LUT1_OUT_REGISTERED; /* B */
  lut_config[7].input_source[HAL_PLAY_LUT_INPUT2] = HAL_PLAY_LUT_INPUT_LUT3_OUT_REGISTERED; /* C */
  lut_config[7].input_source[HAL_PLAY_LUT_INPUT3] = HAL_PLAY_LUT_INPUT_LUT5_OUT_REGISTERED; /* D */

  /* LUT10 configuration */
  lut_config[8].lut             = HAL_PLAY_LUT10;
  lut_config[8].truth_table     = 0xFFFE; /* Logical expression: A | B | C | D */
  lut_config[8].clk_gate_source = HAL_PLAY_LUT_CLK_GATE_OFF;

  lut_config[8].input_source[HAL_PLAY_LUT_INPUT0] = HAL_PLAY_LUT_INPUT_LUT8_OUT_DIRECT; /* A */
  lut_config[8].input_source[HAL_PLAY_LUT_INPUT1] = HAL_PLAY_LUT_INPUT_LUT9_OUT_DIRECT; /* B */
  lut_config[8].input_source[HAL_PLAY_LUT_INPUT2] = HAL_PLAY_LUT_INPUT_LUT12_OUT_DIRECT; /* C */
  lut_config[8].input_source[HAL_PLAY_LUT_INPUT3] = HAL_PLAY_LUT_INPUT_LUT13_OUT_DIRECT; /* D */

  /* LUT11 configuration */
  lut_config[9].lut             = HAL_PLAY_LUT11;
  lut_config[9].truth_table     = 0x8888; /* Logical expression: A & B */
  lut_config[9].clk_gate_source = HAL_PLAY_LUT_CLK_GATE_OFF;

  lut_config[9].input_source[HAL_PLAY_LUT_INPUT0] = HAL_PLAY_LUT_INPUT_LUT15_OUT_REGISTERED; /* A */
  lut_config[9].input_source[HAL_PLAY_LUT_INPUT1] = HAL_PLAY_LUT_INPUT_LUT10_OUT_DIRECT; /* B */
  lut_config[9].input_source[HAL_PLAY_LUT_INPUT2] = HAL_PLAY_LUT_INPUT_DEFAULT; /* C - unused: input is SW trigger 15 */
  lut_config[9].input_source[HAL_PLAY_LUT_INPUT3] = HAL_PLAY_LUT_INPUT_DEFAULT; /* D - unused: input is SW trigger 15 */

  /* LUT12 configuration */
  lut_config[10].lut             = HAL_PLAY_LUT12;
  lut_config[10].truth_table     = 0x100; /* Logical expression: D & !C & !B & !A */
  lut_config[10].clk_gate_source = HAL_PLAY_LUT_CLK_GATE_OFF;

  lut_config[10].input_source[HAL_PLAY_LUT_INPUT0] = HAL_PLAY_LUT_INPUT_LUT1_OUT_REGISTERED; /* A */
  lut_config[10].input_source[HAL_PLAY_LUT_INPUT1] = HAL_PLAY_LUT_INPUT_LUT2_OUT_REGISTERED; /* B */
  lut_config[10].input_source[HAL_PLAY_LUT_INPUT2] = HAL_PLAY_LUT_INPUT_LUT3_OUT_REGISTERED; /* C */
  lut_config[10].input_source[HAL_PLAY_LUT_INPUT3] = HAL_PLAY_LUT_INPUT_LUT5_OUT_REGISTERED; /* D */

  /* LUT13 configuration */
  lut_config[11].lut             = HAL_PLAY_LUT13;
  lut_config[11].truth_table     = 0x400; /* Logical expression: D & !C & B & !A */
  lut_config[11].clk_gate_source = HAL_PLAY_LUT_CLK_GATE_OFF;

  lut_config[11].input_source[HAL_PLAY_LUT_INPUT0] = HAL_PLAY_LUT_INPUT_LUT0_OUT_REGISTERED; /* A */
  lut_config[11].input_source[HAL_PLAY_LUT_INPUT1] = HAL_PLAY_LUT_INPUT_LUT1_OUT_REGISTERED; /* B */
  lut_config[11].input_source[HAL_PLAY_LUT_INPUT2] = HAL_PLAY_LUT_INPUT_LUT3_OUT_REGISTERED; /* C */
  lut_config[11].input_source[HAL_PLAY_LUT_INPUT3] = HAL_PLAY_LUT_INPUT_LUT5_OUT_REGISTERED; /* D */

  /* LUT15 configuration */
  lut_config[12].lut             = HAL_PLAY_LUT15;
  lut_config[12].truth_table     = 0x5555; /* Logical expression: !A */
  lut_config[12].clk_gate_source = HAL_PLAY_LUT_CLK_GATE_FILTER4;

  lut_config[12].input_source[HAL_PLAY_LUT_INPUT0] = HAL_PLAY_LUT_INPUT_LUT15_OUT_REGISTERED; /* A */
  lut_config[12].input_source[HAL_PLAY_LUT_INPUT1] = HAL_PLAY_LUT_INPUT_DEFAULT; /* B - unused: input is SW trigger 15 */
  lut_config[12].input_source[HAL_PLAY_LUT_INPUT2] = HAL_PLAY_LUT_INPUT_DEFAULT; /* C - unused: input is SW trigger 15 */
  lut_config[12].input_source[HAL_PLAY_LUT_INPUT3] = HAL_PLAY_LUT_INPUT_DEFAULT; /* D - unused: input is SW trigger 15 */

  if (HAL_PLAY_LUT_SetConfig(&hPLAY1, lut_config, 13U) != HAL_OK)
  {
    return NULL;
  }

  /* PLAY output configuration */
  hal_play_out_config_t out_config[2];

  /* Output: SOS signal output */
  /* Connect LUT11 (direct output) to PLAY output 0 */
  out_config[0].output_mux = HAL_PLAY_OUT0;
  out_config[0].lut_output = HAL_PLAY_LUT11_OUT_DIRECT;

  /* Output: Alarm off signal to EXTI for wakeup */
  /* Connect LUT15 (registered output) to PLAY output 14 */
  out_config[1].output_mux = HAL_PLAY_OUT14;
  out_config[1].lut_output = HAL_PLAY_LUT15_OUT_REGISTERED;

  if (HAL_PLAY_OUTPUT_SetConfig(&hPLAY1, out_config, 2U) != HAL_OK)
  {
    return NULL;
  }

  /* ### PLAY1 GPIO Configuration ########################### */
  /* GPIO Clocks activation */
  HAL_RCC_GPIOC_EnableClock();

  HAL_RCC_GPIOD_EnableClock();

  hal_gpio_config_t  gpio_config;

  /**
    [GPIO Pin] ------> [Signal Name]

       PC13    ------>   PLAY1_IN4
    **/
  gpio_config.mode        = HAL_GPIO_MODE_ALTERNATE;
  gpio_config.output_type = HAL_GPIO_OUTPUT_PUSHPULL;
  gpio_config.pull        = HAL_GPIO_PULL_DOWN;
  gpio_config.speed       = HAL_GPIO_SPEED_FREQ_HIGH;
  gpio_config.alternate   = HAL_GPIO_AF_14;
  HAL_GPIO_Init(HAL_GPIOC, HAL_GPIO_PIN_13, &gpio_config);

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

  hal_exti_config_t exti_config;

  /* Initialize the EXTI for line 51 */
  HAL_EXTI_Init(&hEXTI51, HAL_EXTI_LINE_51);

  /* Set the trigger as FALLING */
  exti_config.trigger = HAL_EXTI_TRIGGER_FALLING;
  HAL_EXTI_SetConfig(&hEXTI51, &exti_config);

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

  /* De-initialize all GPIOC pins associated with PLAY1 */
  HAL_GPIO_DeInit(HAL_GPIOC, HAL_GPIO_PIN_13);

  /* De-initialize all GPIOD pins associated with PLAY1 */
  HAL_GPIO_DeInit(HAL_GPIOD, HAL_GPIO_PIN_11);

  /* De-initialize the EXTI for line51 */
  HAL_EXTI_DeInit(&hEXTI51);
}

hal_play_handle_t *mx_play1_gethandle(void)
{
  return &hPLAY1;
}

hal_exti_handle_t *mx_play1_exti51_gethandle(void)
{
  return &hEXTI51;
}

/******************************************************************************/
/*                     PLAY global non-secure interrupt OR play1_out14 through EXTI line                    */
/******************************************************************************/
void PLAY1_IRQHandler(void)
{
  HAL_PLAY_IRQHandler(&hPLAY1);

  HAL_EXTI_IRQHandler(&hEXTI51);
}
