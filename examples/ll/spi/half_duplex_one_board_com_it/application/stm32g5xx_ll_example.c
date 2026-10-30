/**
  ******************************************************************************
  * file           : stm32g5xx_ll_example.c
  * brief          : A half-duplex SPI communication in interrupt mode,
  *                  on a single board using the LL API.
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t  *TxData;
static uint8_t  *RxData;
static uint16_t  TxCount;
static uint16_t  RxCount;

/* Private functions prototype -----------------------------------------------*/
static inline void SPI_HalfDuplexController_IT(uint8_t *buffer, uint16_t size, uint32_t direction);
static inline void SPI_HalfDuplexResponder_IT(uint8_t *buffer, uint16_t size, uint32_t direction);
void mx_spix_controller_irqhandler(void);
void mx_spix_responder_irqhandler(void);

/* Private functions ---------------------------------------------------------*/
/** brief: Starts  a half-duplex Controller transfer using interrupts.
  * param  buffer:    Pointer to the transmission or reception buffer.
  * param  size:      Number of bytes to transfer.
  * param  direction: Transfer direction, transmission or reception mode.
  */
static inline void SPI_HalfDuplexController_IT(uint8_t *buffer, uint16_t size, uint32_t direction)
{
  uint32_t direction_flag;

  if (direction == LL_SPI_HALF_DUPLEX_TX)
  {
    direction_flag = LL_SPI_IT_TXP;
  }
  else
  {
    direction_flag = LL_SPI_IT_RXP;
  }

  /* Set half-duplex direction */
  LL_SPI_SetHalfDuplexDirection(MX_SPIx_controller, direction);

  /* Set the number of data at current transfer */
  LL_SPI_SetTransferSize(MX_SPIx_controller, (uint32_t)size);

  LL_SPI_Enable(MX_SPIx_controller);

  /* Fill in the TxFIFO */
  while ((direction == LL_SPI_HALF_DUPLEX_TX)
         && (LL_SPI_IsActiveFlag_TXP(MX_SPIx_controller) != 0UL) && (TxCount > 0U))
  {
    LL_SPI_TransmitData8(MX_SPIx_controller, *TxData);
    TxData++;
    TxCount--;
  }

  /* Enable interrupts */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_SPI_EnableIT(MX_SPIx_controller,
                  (direction_flag | LL_SPI_IT_EOT | LL_SPI_IT_UDR | LL_SPI_IT_OVR));
#else
  LL_SPI_EnableIT(MX_SPIx_controller, (direction_flag | LL_SPI_IT_EOT));
#endif /* USE_LL_APP_ERROR */

  LL_SPI_StartMasterTransfer(MX_SPIx_controller);
}


/** brief: Starts a half-duplex Responder transfer using interrupts.
  * param  buffer:    Pointer to the transmission or reception buffer.
  * param  size:      Number of bytes to transfer.
  * param  direction: Transfer direction, transmission or reception mode.
  */
static inline void SPI_HalfDuplexResponder_IT(uint8_t *buffer, uint16_t size, uint32_t direction)
{
  uint32_t direction_flag;

  if (direction == LL_SPI_HALF_DUPLEX_TX)
  {
    direction_flag = LL_SPI_IT_TXP;
  }
  else
  {
    direction_flag = LL_SPI_IT_RXP;
  }

  /* Set half-duplex direction */
  LL_SPI_SetHalfDuplexDirection(MX_SPIx_responder, direction);

  /* Set the number of data at current transfer */
  LL_SPI_SetTransferSize(MX_SPIx_responder, (uint32_t)size);

  LL_SPI_Enable(MX_SPIx_responder);

  while ((direction == LL_SPI_HALF_DUPLEX_TX)
         && (LL_SPI_IsActiveFlag_TXP(MX_SPIx_responder) != 0UL) && (TxCount > 0U))
  {
    LL_SPI_TransmitData8(MX_SPIx_responder, *TxData);
    TxData++;
    TxCount--;
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_SPI_EnableIT(MX_SPIx_responder,
                  (direction_flag | LL_SPI_IT_EOT | LL_SPI_IT_UDR | LL_SPI_IT_OVR));
#else
  LL_SPI_EnableIT(MX_SPIx_responder, (direction_flag | LL_SPI_IT_EOT));
#endif /* USE_LL_APP_ERROR */

}

/** brief: Transmits data with the Controller SPI instance using interrupts.
  * param  tx_buf: Pointer to transmission data buffer.
  * param  size:   Number of bytes to transfer.
  */
void SPI_HalfDuplexControllerTransmit_IT(uint8_t *tx_buf, uint16_t size)
{
  TxData  = tx_buf;
  TxCount = size;
  SPI_HalfDuplexController_IT(tx_buf, size, LL_SPI_HALF_DUPLEX_TX);
}

/** brief: Transmits data with the Responder SPI instance using interrupts.
  * param  tx_buf: Pointer to transmission data buffer.
  * param  size:   Number of bytes to transfer.
  */
void SPI_HalfDuplexResponderTransmit_IT(uint8_t *tx_buf, uint16_t size)
{
  TxData  = tx_buf;
  TxCount = size;
  SPI_HalfDuplexResponder_IT(tx_buf, size, LL_SPI_HALF_DUPLEX_TX);
}

/** brief: Receives data with the Controller SPI instance using interrupts.
  * param  rx_buf: Pointer to reception data buffer.
  * param  size:   Number of bytes to receive.
  */
void SPI_HalfDuplexControllerReceive_IT(uint8_t *rx_buf, uint16_t size)
{
  RxData  = rx_buf;
  RxCount = size;
  SPI_HalfDuplexController_IT(rx_buf, size, LL_SPI_HALF_DUPLEX_RX);
}

/** brief: Receives data with the Responder SPI instance using interrupts.
  * param  rx_buf: Pointer to reception data buffer.
  * param  size:   Number of bytes to receive.
  */
void SPI_HalfDuplexResponderReceive_IT(uint8_t *rx_buf, uint16_t size)
{
  RxData  = rx_buf;
  RxCount = size;
  SPI_HalfDuplexResponder_IT(rx_buf, size, LL_SPI_HALF_DUPLEX_RX);
}


/**
  * brief: Handles MX_SPIx_controller interrupts.
  */
void mx_spix_controller_irqhandler(void)
{
  uint32_t it_source     = LL_SPI_READ_REG(MX_SPIx_controller, IER);
  uint32_t pending_flags = it_source & LL_SPI_READ_REG(MX_SPIx_controller, SR);

  /* SPI data */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if (((pending_flags & (SPI_SR_OVR | SPI_SR_UDR)) == 0UL)
      && ((pending_flags & SPI_SR_TXP) != 0UL))
#else
  if ((pending_flags & SPI_SR_TXP) != 0UL)
#endif /* USE_LL_APP_ERROR */
  {
    if (TxCount > 0U)
    {
      LL_SPI_TransmitData8(MX_SPIx_controller, *TxData);
      TxData++;
      TxCount--;
      return;
    }
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if (((pending_flags & (SPI_SR_OVR | SPI_SR_UDR)) == 0UL)
      && ((pending_flags & SPI_SR_RXP) != 0UL))
#else
  if ((pending_flags & SPI_SR_RXP) != 0UL)
#endif /* USE_LL_APP_ERROR */
  {
    if (RxCount > 0U)
    {
      *RxData = LL_SPI_ReceiveData8(MX_SPIx_controller);
      RxData++;
      RxCount--;
      return;
    }
  }

  /* SPI end of Tx transfer */
  if ((pending_flags & SPI_SR_EOT) != 0UL)
  {
    /* Clear EOT and data-ready flags */
    LL_SPI_ClearFlag(MX_SPIx_controller, (LL_SPI_FLAG_EOT | LL_SPI_FLAG_TXP | LL_SPI_FLAG_TXTF | LL_SPI_FLAG_RXP));

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    LL_SPI_DisableIT(MX_SPIx_controller, (LL_SPI_IT_TXP | LL_SPI_IT_EOT | LL_SPI_IT_UDR | LL_SPI_IT_OVR));
#else
    LL_SPI_DisableIT(MX_SPIx_controller, (LL_SPI_IT_TXP | LL_SPI_IT_EOT));
#endif /* USE_LL_APP_ERROR */

    LL_SPI_Disable(MX_SPIx_controller);

    SPI_ControllerCpltCallback();
    return;
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if ((pending_flags & SPI_SR_OVR) != 0UL)
  {
    LL_SPI_ClearFlag_OVR(MX_SPIx_controller);
  }
  if ((pending_flags & SPI_SR_UDR) != 0UL)
  {
    LL_SPI_ClearFlag_UDR(MX_SPIx_controller);
  }

  /* Check error flags */
  if ((pending_flags & (SPI_SR_OVR | SPI_SR_UDR)) != 0UL)
  {
    /* Disable SPI peripheral */
    LL_SPI_Disable(MX_SPIx_controller);

    /* Disable interrupts */
    LL_SPI_DisableIT(MX_SPIx_controller, (LL_SPI_IT_EOT | LL_SPI_IT_TXP | LL_SPI_IT_OVR | LL_SPI_IT_UDR));

    SPI_ControllerErrorCallback();
  }
#endif /* USE_LL_APP_ERROR */
}

/**
  * brief: Handles MX_SPIx_responder interrupts.
  */
void mx_spix_responder_irqhandler(void)
{
  uint32_t it_source     = LL_SPI_READ_REG(MX_SPIx_responder, IER);
  uint32_t pending_flags = it_source & LL_SPI_READ_REG(MX_SPIx_responder, SR);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if (((pending_flags & (SPI_SR_OVR | SPI_SR_UDR)) == 0UL)
      && ((pending_flags & SPI_SR_TXP) != 0UL))
#else
  if ((pending_flags & SPI_SR_TXP) != 0UL)
#endif /* USE_LL_APP_ERROR */
  {
    if (TxCount > 0U)
    {
      LL_SPI_TransmitData8(MX_SPIx_responder, *TxData);
      TxData++;
      TxCount--;
      return;
    }
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if (((pending_flags & (SPI_SR_OVR | SPI_SR_UDR)) == 0UL)
      && ((pending_flags & SPI_SR_RXP) != 0UL))
#else
  if ((pending_flags & SPI_SR_RXP) != 0UL)
#endif /* USE_LL_APP_ERROR */
  {
    if (RxCount > 0U)
    {
      *RxData = LL_SPI_ReceiveData8(MX_SPIx_responder);
      RxData++;
      RxCount--;
      return;
    }
  }

  if ((pending_flags & SPI_SR_EOT) != 0UL)
  {
    LL_SPI_ClearFlag(MX_SPIx_responder,
                     (LL_SPI_FLAG_EOT | LL_SPI_FLAG_TXP | LL_SPI_FLAG_TXTF | LL_SPI_FLAG_RXP));

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    LL_SPI_DisableIT(MX_SPIx_responder, (LL_SPI_IT_TXP | LL_SPI_IT_RXP | LL_SPI_IT_EOT
                                         | LL_SPI_IT_UDR | LL_SPI_IT_OVR));
#else
    LL_SPI_DisableIT(MX_SPIx_responder, (LL_SPI_IT_TXP | LL_SPI_IT_RXP | LL_SPI_IT_EOT));
#endif /* USE_LL_APP_ERROR */

    /* Disable SPI peripheral */
    LL_SPI_Disable(MX_SPIx_responder);

    SPI_ResponderCpltCallback();
    return;
  }

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if ((pending_flags & SPI_SR_OVR) != 0UL)
  {
    LL_SPI_ClearFlag_OVR(MX_SPIx_responder);
  }
  if ((pending_flags & SPI_SR_UDR) != 0UL)
  {
    LL_SPI_ClearFlag_UDR(MX_SPIx_responder);
  }

  if ((pending_flags & (SPI_SR_OVR | SPI_SR_UDR)) != 0UL)
  {
    /* Disable SPI peripheral */
    LL_SPI_Disable(MX_SPIx_responder);

    /* Disable interrupts */
    LL_SPI_DisableIT(MX_SPIx_responder,
                     (LL_SPI_IT_EOT | LL_SPI_IT_TXP | LL_SPI_IT_RXP | LL_SPI_IT_OVR | LL_SPI_IT_UDR));

    SPI_ResponderErrorCallback();
  }
#endif /* USE_LL_APP_ERROR */
}


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  * brief: This function provides delay (in milliseconds) based on SysTick counter flag.
  * param: delay_ms Number of milliseconds to delay.
  */
inline void DelayMs(uint32_t delay_ms)
{
  LL_Delay_NoISR(delay_ms);
}
#endif /* USE_LL_APP_ERROR */
