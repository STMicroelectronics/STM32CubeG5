/**
  ******************************************************************************
  * file           : stm32g5xx_ll_example.c
  * brief          : How to use the LL UART API for reception to IDLE event in
  *                  circular DMA mode.
  ******************************************************************************
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "ll_example.h"

/* Private define ------------------------------------------------------------*/
#define UART_RX_EVENT_TC   0U
#define UART_RX_EVENT_IDLE 1U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__attribute__((section(".non_cacheable_variables"), aligned(4)))
static uint32_t UartRxDmaLinkedListRegisters[LL_DMA_NODE_REGISTER_NUM];
static volatile uint32_t UartRxTransferSize;

/* Private functions prototype -----------------------------------------------*/
static void DMA_IRQHandler(DMA_Channel_TypeDef *channel);
static DMA_Channel_TypeDef *UART_GetRxDmaChannel(void);
static uint32_t *UART_GetRxDmaLinkedListRegisters(void);
static system_status_t UART_WaitOnFlagUntilTimeout(uint32_t flag, uint32_t timeout_ms);
void mx_uartx_irqhandler(void);
void LPDMA1_CH0_IRQHandler(void);

/* Private functions ---------------------------------------------------------*/

/** brief: Transmit MX_UARTx data in blocking mode.
  * param: p_data     Pointer to data buffer.
  * param: size_byte  Amount of data elements to be transmitted.
  * param: timeout_ms Timeout duration in milliseconds.
  * retval: system status (see system_status_t)
  */
inline system_status_t UART_Transmit(const void *p_data, uint32_t size_byte, uint32_t timeout_ms)
{
  const uint8_t *p_tx_data = (const uint8_t *)p_data;

  if ((p_data == NULL) || (size_byte == 0U))
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  LL_USART_ClearFlag_TC(MX_UARTx);

  while (size_byte > 0U)
  {
    if (UART_WaitOnFlagUntilTimeout(LL_USART_ISR_TXE_TXFNF, timeout_ms) != SYSTEM_OK)
    {
      return SYSTEM_PERIPHERAL_ERROR;
    }

    LL_USART_TransmitData8(MX_UARTx, *p_tx_data);
    p_tx_data++;
    size_byte--;
  }

  return UART_WaitOnFlagUntilTimeout(LL_USART_ISR_TC, timeout_ms);
}


/** brief: Receive MX_UARTx data in circular DMA mode till either the expected number
  *         of data is received or an IDLE event occurs.
  * param: p_data    Pointer to data buffer.
  * param: size_byte Amount of data elements to be received.
  * retval: system status (see system_status_t)
  */
inline system_status_t UART_ReceiveToIdleCircular_DMA(void *p_data, uint32_t size_byte)
{
  DMA_Channel_TypeDef *rx_channel = UART_GetRxDmaChannel();
  uint32_t *generated_uartx_rx_dma_lli_reg = UART_GetRxDmaLinkedListRegisters();
  uint32_t register_idx;

  if ((p_data == NULL) || (size_byte == 0U))
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }

  UartRxTransferSize = size_byte;

  LL_DMA_DisableChannel(rx_channel);
  LL_USART_DisableDMAReq_RX(MX_UARTx);
  LL_DMA_DisableIT(rx_channel, LL_DMA_IT_ALL);
  LL_DMA_ClearFlag(rx_channel, LL_DMA_FLAG_ALL);

  for (register_idx = 0U; register_idx < LL_DMA_NODE_REGISTER_NUM; register_idx++)
  {
    UartRxDmaLinkedListRegisters[register_idx] = generated_uartx_rx_dma_lli_reg[register_idx];
  }

  /* Update only the linked-list node fields that are specific to the current reception. */
  UartRxDmaLinkedListRegisters[LL_DMA_NODE_CBR1_REG_OFFSET] = size_byte;
  UartRxDmaLinkedListRegisters[LL_DMA_NODE_CSAR_REG_OFFSET] = LL_USART_DMA_GetRegAddr(MX_UARTx,
                                                              LL_USART_DMA_REG_DATA_RECEIVE);
  UartRxDmaLinkedListRegisters[LL_DMA_NODE_CDAR_REG_OFFSET] = (uint32_t)p_data;
  UartRxDmaLinkedListRegisters[LL_DMA_NODE_CLLR_REG_OFFSET] = LL_DMA_UPDATE_ALL
                                                              | ((uint32_t)UartRxDmaLinkedListRegisters
                                                                 & DMA_CLLR_LA);

  LL_DMA_SetLinkedListBaseAddr(rx_channel, (uint32_t)UartRxDmaLinkedListRegisters);
  LL_DMA_ConfigLinkUpdate(rx_channel, LL_DMA_UPDATE_ALL, (uint32_t)UartRxDmaLinkedListRegisters);

  LL_DMA_SetTransferEventMode(rx_channel, LL_DMA_LINKEDLIST_XFER_EVENT_Q);

  LL_USART_ClearFlag_IDLE(MX_UARTx);
  LL_USART_EnableIT_IDLE(MX_UARTx);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_DMA_EnableIT(rx_channel, (LL_DMA_IT_TC | LL_DMA_IT_DTE | LL_DMA_IT_ULE | LL_DMA_IT_USE));
#else
  LL_DMA_EnableIT(rx_channel, LL_DMA_IT_TC);
#endif /* USE_LL_APP_ERROR */

  LL_DMA_SetBlkDataLength(rx_channel, 0U);
  LL_DMA_EnableChannel(rx_channel);
  LL_USART_EnableDMAReq_RX(MX_UARTx);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_USART_EnableIT_ERROR(MX_UARTx);
#endif /* USE_LL_APP_ERROR */

  return SYSTEM_OK;
}


/** brief: Handles the MX_UARTx interrupts.
  */
void mx_uartx_irqhandler(void)
{
  uint32_t isr_flags = LL_USART_READ_REG(MX_UARTx, ISR);
  uint32_t cr1_its   = LL_USART_READ_REG(MX_UARTx, CR1);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  uint32_t cr3_its   = LL_USART_READ_REG(MX_UARTx, CR3);
#endif /* USE_LL_APP_ERROR */

  if (((isr_flags & LL_USART_ISR_IDLE) != 0U) && ((cr1_its & LL_USART_CR1_IDLEIE) != 0U))
  {
    DMA_Channel_TypeDef *rx_channel = UART_GetRxDmaChannel();
    uint32_t nb_remaining_rx_data = LL_DMA_GetBlkDataLength(rx_channel);
    uint32_t nb_received_rx_data = UartRxTransferSize - nb_remaining_rx_data;

    LL_USART_DisableIT_IDLE(MX_UARTx);
    LL_USART_ClearFlag_IDLE(MX_UARTx);
    LL_USART_DisableDMAReq_RX(MX_UARTx);
    LL_DMA_DisableChannel(rx_channel);

    if ((nb_remaining_rx_data > 0U) && (nb_remaining_rx_data < UartRxTransferSize))
    {
      UART_RxTransferCpltCallback(nb_received_rx_data, UART_RX_EVENT_IDLE);
    }

    return;
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if (((isr_flags & (LL_USART_ISR_NE | LL_USART_ISR_ORE | LL_USART_ISR_FE)) != 0U)
      && ((cr3_its & LL_USART_CR3_EIE) != 0U))
  {
    DMA_Channel_TypeDef *rx_channel = UART_GetRxDmaChannel();

    LL_USART_DisableIT_IDLE(MX_UARTx);
    LL_USART_DisableIT_ERROR(MX_UARTx);
    LL_USART_DisableDMAReq_RX(MX_UARTx);
    LL_DMA_DisableChannel(rx_channel);

    LL_USART_ClearFlag(MX_UARTx, (LL_USART_ISR_NE | LL_USART_ISR_ORE | LL_USART_ISR_FE));
    LL_USART_ClearFlag_NE(MX_UARTx);

    UART_TransferErrorCallback();
  }
#endif /* USE_LL_APP_ERROR */
}


/** brief: Handles the MX_LPDMAx_CH2y interrupts.
  */
void LPDMA1_CH0_IRQHandler(void)
{
  DMA_Channel_TypeDef *rx_channel = UART_GetRxDmaChannel();

  DMA_IRQHandler(rx_channel);

  LL_USART_DisableIT_IDLE(MX_UARTx);
  LL_USART_DisableDMAReq_RX(MX_UARTx);
  LL_DMA_DisableChannel(rx_channel);
}


/** brief: This function provides delay (in milliseconds) based on SysTick counter flag.
  * param: delay_ms Number of milliseconds to delay.
  */
inline void DelayMs(uint32_t delay_ms)
{
  LL_Delay_NoISR(delay_ms);
}


/** brief: Wait until the requested UART flag becomes active or the timeout expires.
  * param: flag       UART ISR flag to wait for.
  * param: timeout_ms Timeout duration in milliseconds.
  * retval: system status (see system_status_t)
  */
static system_status_t UART_WaitOnFlagUntilTimeout(uint32_t flag, uint32_t timeout_ms)
{
  uint32_t timeout = timeout_ms;

  while ((LL_USART_READ_REG(MX_UARTx, ISR) & flag) == 0U)
  {
    if (SysTick_IsActiveCounterFlag() != 0)
    {
      if (timeout-- == 0U)
      {
        return SYSTEM_PERIPHERAL_ERROR;
      }
    }
  }

  return SYSTEM_OK;
}


/** brief: Returns the DMA channel used by the generated UART RX linked-list transfer.
  * retval: DMA channel pointer
  */
static DMA_Channel_TypeDef *UART_GetRxDmaChannel(void)
{
  return GPDMA1_CH0;
}


/** brief: Returns the generated linked-list register array used by UART RX DMA.
  * retval: Pointer to linked-list register values
  */
static uint32_t *UART_GetRxDmaLinkedListRegisters(void)
{
  return mx_usart2_rx_dma_get_lli_reg();
}


/** brief: Handles the DMA channel interrupts.
  * param: channel Pointer to the DMA channel.
  */
static inline void DMA_IRQHandler(DMA_Channel_TypeDef *channel)
{
  DMA_TypeDef *instance = LL_DMA_GET_INSTANCE(channel);
  uint32_t dma_channel = LL_DMA_GET_CHANNEL_IDX(channel);
  uint32_t its = LL_DMA_READ_REG(channel, CCR);
  uint32_t flags;

  if (LL_DMA_IsActiveFlag_MIS(instance, dma_channel) == 0U)
  {
    return;
  }

  flags = LL_DMA_READ_REG(channel, CSR);

  if (((flags & its) & LL_DMA_FLAG_TC) != 0U)
  {
    LL_DMA_ClearFlag_TC(channel);

    UART_RxTransferCpltCallback(UartRxTransferSize, UART_RX_EVENT_TC);
    return;
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_DMA_ClearFlag(channel, LL_DMA_FLAG_ALL);

  if (((flags & its) & (LL_DMA_FLAG_DTE | LL_DMA_FLAG_ULE | LL_DMA_FLAG_USE)) != 0U)
  {
    LL_DMA_ResetChannel(channel);
    LL_DMA_DisableIT(channel, LL_DMA_IT_ALL);

    UART_TransferErrorCallback();
  }
#endif /* USE_LL_APP_ERROR */
}
