/**
  ******************************************************************************
  * @file           : mx_usart2.c
  * @brief          : USART2 Peripheral initialization
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
#include "mx_usart2.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype------------------------------------------------*/
/* Exported variables by reference--------------------------------------------*/

/* Exported function definition ----------------------------------------------*/

/******************************************************************************/
/* Exported functions for UART in LL layer */
/******************************************************************************/

/* DMA nodes must be 8-byte aligned. */
__attribute__((section("non_cacheable_area"), aligned(8)))
static uint32_t USART2_RX_DMA_LLi_reg[LL_DMA_NODE_REGISTER_NUM];

USART_TypeDef *mx_usart2_uart_init(void)
{
  uint32_t reg_temp;

  /* Basic configuration */

  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
  /* LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1); */ /* Configuration matches register reset state at startup. */
  LL_USART_ConfigAsyncMode(USART2);

  reg_temp = (LL_USART_DATAWIDTH_8_BIT | LL_USART_PARITY_NONE
              | LL_USART_DIRECTION_TX_RX | LL_USART_OVERSAMPLING_16);
  LL_USART_ConfigXfer(USART2, reg_temp, LL_USART_STOP_BIT_1);

  /* LL_USART_SetHWFlowCtrl(USART2, LL_USART_HWCONTROL_NONE); */ /* Configuration matches register reset state at startup. */

  /* LL_USART_SetPrescaler(USART2, LL_USART_PRESCALER_DIV1); */ /* Configuration matches register reset state at startup. */

  LL_USART_SetBaudRate(USART2, 140000000, LL_USART_PRESCALER_DIV1, LL_USART_OVERSAMPLING_16,
                       115200);

  /* ### USART2 GPIO Configuration ########################### */

  /* GPIO Clocks activation */
  LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);

  /**
    [GPIO Pin] ------> [Signal Name]

       PA3     ------>   USART2_RX
       PA2     ------>   USART2_TX
    **/

  /* Configure IO output speed (Low, Medium, High or Very-High) */
  /* LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_2, LL_GPIO_SPEED_FREQ_LOW); */ /* Configuration matches register reset state at startup. */

  /* Configure IO output type (Push-Pull or Open-Drain) */
  /* LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_3 | LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL); */ /* Configuration matches register reset state at startup. */

  /* Activate the Pull-up, Pull-down resistor, or No pull for the current IO */
  /* LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */
  /* LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_2, LL_GPIO_PULL_NO); */ /* Configuration matches register reset state at startup. */

  /* Configure the Alternate Function in current IO */
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_7);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_7);

  /* Configure IO direction mode (Input, Output, Alternate or Analog) */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);

  /* Enable USART instance interrupt */
  NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(USART2_IRQn);
  /* ################################################## USART2 RX DMA configuration */

  /* Enable GPDMA1 clock */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPDMA1);

  /* Configure DMA channel control parameters */
  LL_DMA_ConfigControl(GPDMA1_CH0, LL_DMA_PRIORITY_HIGH | LL_DMA_LINK_ALLOCATED_PORT0 | LL_DMA_LINKEDLIST_EXECUTION_Q);

  /* Configure DMA channel transfer event mode */
  LL_DMA_SetTransferEventMode(GPDMA1_CH0, LL_DMA_LINKEDLIST_XFER_EVENT_Q);

  /* Configure DMA channel block data length */
  /* LL_DMA_SetBlkDataLength(GPDMA1_CH0, 0U); */ /* Configuration matches register reset state at startup. */

  /* Configure DMA channel linked-list base address */
  LL_DMA_SetLinkedListBaseAddr(GPDMA1_CH0, (uint32_t)&USART2_RX_DMA_LLi_reg);

  /* Configure DMA channel registers update and node address offset during the link transfer */
  LL_DMA_ConfigLinkUpdate(GPDMA1_CH0, LL_DMA_UPDATE_ALL, (uint32_t)&USART2_RX_DMA_LLi_reg);

  /* CTR1 register : Data width and address increment configuration */
  USART2_RX_DMA_LLi_reg[LL_DMA_NODE_CTR1_REG_OFFSET] = LL_DMA_SRC_ADDR_FIXED | LL_DMA_SRC_DATA_WIDTH_BYTE | \
                                                     LL_DMA_DEST_ADDR_INCREMENTED | LL_DMA_DEST_DATA_WIDTH_BYTE;

  /* CTR2 register : Request and direction configuration */
  USART2_RX_DMA_LLi_reg[LL_DMA_NODE_CTR2_REG_OFFSET] = LL_GPDMA1_REQUEST_USART2_RX | LL_DMA_HARDWARE_REQUEST_BURST | LL_DMA_DIRECTION_PERIPH_TO_MEMORY;

  /* CLLR register */
  if (IS_DMA_2D_ADDRESSING_INSTANCE(GPDMA1_CH0) != 0U)
  {
    USART2_RX_DMA_LLi_reg[LL_DMA_NODE_CLLR_REG_OFFSET] = LL_DMA_UPDATE_ALL | ((uint32_t)&USART2_RX_DMA_LLi_reg & DMA_CLLR_LA);
  }
  else
  {
    USART2_RX_DMA_LLi_reg[LL_DMA_NODE_CLLR_REG_OFFSET - 2U] = LL_DMA_UPDATE_ALL | ((uint32_t)&USART2_RX_DMA_LLi_reg & DMA_CLLR_LA);
  }

  /* Enable the interruption for DMA */
  NVIC_SetPriority(GPDMA1_CH0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
  NVIC_EnableIRQ(GPDMA1_CH0_IRQn);

  LL_USART_Enable(USART2);
  return USART2;
}

void mx_usart2_uart_deinit(void)
{
  LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_USART2);
  LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_USART2);

  LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART2);

  /* Disable USART instance interrupt */
  NVIC_DisableIRQ(USART2_IRQn);
  LL_USART_Disable(USART2);

  /* ### GPIO deinitialization of USART2: USART2_RX,USART2_TX ########################### */

  /* Configure IO in Analog Mode */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ANALOG);
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ANALOG);

  /* Configure the default Alternate Function in current IO */
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_0);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_0);

  /* Configure the default value for IO Speed */
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_LOW);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_2, LL_GPIO_SPEED_FREQ_LOW);

  /* Configure the default value IO Output Type */
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_3 | LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL);

  /* Deactivate the Pull-up and Pull-down resistor for the current IO */
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_2, LL_GPIO_PULL_NO);

  /* Reset the IO output state */
  LL_GPIO_WriteOutputPin(GPIOA, LL_GPIO_PIN_3 | LL_GPIO_PIN_2, LL_GPIO_PIN_RESET);

  /* De-initialize the DMA channel */
  LL_DMA_DisableChannel(GPDMA1_CH0);
  LL_DMA_ClearFlag(GPDMA1_CH0, LL_DMA_FLAG_ALL);

  LL_DMA_WRITE_REG(GPDMA1_CH0, CCR, 0U);
  LL_DMA_WRITE_REG(GPDMA1_CH0, CTR1, 0U);
  LL_DMA_WRITE_REG(GPDMA1_CH0, CTR2, 0U);
  LL_DMA_WRITE_REG(GPDMA1_CH0, CBR1, 0U);
  LL_DMA_WRITE_REG(GPDMA1_CH0, CTR3, 0U);
  LL_DMA_WRITE_REG(GPDMA1_CH0, CBR2, 0U);
  LL_DMA_WRITE_REG(GPDMA1_CH0, CLLR, 0U);

  /* Disable the interruption for DMA */
  NVIC_DisableIRQ(GPDMA1_CH0_IRQn);
}

uint32_t *mx_usart2_rx_dma_get_lli_reg(void)
{
  return USART2_RX_DMA_LLi_reg;
}

/******************************************************************************/
/*         USART2 global interrupt is managed directly in user code.          */
/******************************************************************************/
/* void USART2_IRQHandler(void)
{
}
  */

/******************************************************************************/
/*    GPDMA1 channel 0 global interrupt is managed directly in user code.     */
/******************************************************************************/
/* void GPDMA1_CH0_IRQHandler(void)
{
}
  */
