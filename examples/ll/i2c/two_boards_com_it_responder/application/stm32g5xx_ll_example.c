/**
  ******************************************************************************
  * file           : stm32g5xx_ll_example.c
  * brief          : The responder's application handling a data transfer over
  *                  I2C in interrupt mode with LL API.
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
typedef enum
{
  I2C_XFER_NONE = 0U,
  I2C_XFER_TX = 1U,
  I2C_XFER_RX = 2U
} i2c_xfer_t;

/* Private define ------------------------------------------------------------*/
/** Mask used to clear the transfer configuration bits in I2C CR2 while preserving
  * the other control bits. It removes the address, addressing mode, byte count,
  * reload, and transfer direction fields after a transfer completes.
  */
#define I2C_XFER_INFO ~((uint32_t)(I2C_CR2_SADD | I2C_CR2_HEAD10R | I2C_CR2_NBYTES | I2C_CR2_RELOAD | I2C_CR2_RD_WRN))

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static uint8_t *TxData = NULL;
static uint8_t *RxData = NULL;
static uint16_t TxCount;
static uint16_t RxCount;
/* Tracks the current I2C transfer state shared between the main flow and interrupt handlers. */
static volatile i2c_xfer_t I2C_CurrentXfer = I2C_XFER_NONE;

/* Private functions prototype -----------------------------------------------*/
static void I2C_FlushTxDR(void);
static void I2C_ITComplete(uint32_t it_flags, uint32_t it_sources);
void mx_i2cx_ev_irqhandler(void);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
static void I2C_ITError(void);
void mx_i2cx_err_irqhandler(void);
#endif /* USE_LL_APP_ERROR */

/* Private functions ---------------------------------------------------------*/


/** brief:  MX_I2Cx receive an amount of data in non-blocking mode with interrupt.
  * param   rx_buffer: Pointer to reception data buffer.
  * param   size:      Number of bytes to transfer.
  */
inline void I2C_Receive_IT(uint8_t *rx_buffer, uint8_t size)
{
  I2C_CurrentXfer = I2C_XFER_RX;
  RxData = rx_buffer;
  RxCount = size;

  LL_I2C_AcknowledgeEnable(MX_I2Cx);

  /* Enable interrupts */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_I2C_EnableIT(MX_I2Cx, LL_I2C_CR1_ERRIE | LL_I2C_CR1_STOPIE | LL_I2C_CR1_NACKIE
                  | LL_I2C_CR1_ADDRIE | LL_I2C_CR1_RXIE);
#else
  LL_I2C_EnableIT(MX_I2Cx, LL_I2C_CR1_STOPIE | LL_I2C_CR1_NACKIE
                  | LL_I2C_CR1_ADDRIE | LL_I2C_CR1_RXIE);
#endif /* USE_LL_APP_ERROR */
}


/** brief:  MX_I2Cx transmit an amount of data in non-blocking mode with interrupt.
  * param   tx_buffer: Pointer to transmission data buffer.
  * param   size:      Number of bytes to transfer.
  */
inline void I2C_Transmit_IT(uint8_t *tx_buffer, uint8_t size)
{
  I2C_CurrentXfer = I2C_XFER_TX;
  TxData = tx_buffer;
  TxCount = size;

  LL_I2C_AcknowledgeEnable(MX_I2Cx);

  /* Preload TX data if no stretch enable */
  if ((LL_I2C_IsEnabledClockStretching(MX_I2Cx) == 0UL) && (TxCount > 0U))
  {
    LL_I2C_TransmitData8(MX_I2Cx, *TxData);
    TxData++;
    TxCount--;
  }

  /* Enable interrupts */
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  LL_I2C_EnableIT(MX_I2Cx, LL_I2C_CR1_ERRIE | LL_I2C_CR1_STOPIE | LL_I2C_CR1_NACKIE
                  | LL_I2C_CR1_ADDRIE | LL_I2C_CR1_TXIE);
#else
  LL_I2C_EnableIT(MX_I2Cx, LL_I2C_CR1_STOPIE | LL_I2C_CR1_NACKIE | LL_I2C_CR1_ADDRIE
                  | LL_I2C_CR1_TXIE);
#endif /* USE_LL_APP_ERROR */
}


/**
  *  Handles the MX_I2Cx event interrupts.
  */
void mx_i2cx_ev_irqhandler(void)
{
  uint32_t it_flags = LL_I2C_READ_REG(MX_I2Cx, ISR);
  uint32_t it_sources = LL_I2C_READ_REG(MX_I2Cx, CR1);

  /* Check if STOPF is set */
  if (((it_flags & LL_I2C_ISR_STOPF) != 0U) && ((it_sources & LL_I2C_CR1_STOPIE) != 0U))
  {
    /* Call I2C slave complete process */
    I2C_ITComplete(it_flags, it_sources);
  }
  else if (((it_flags & LL_I2C_ISR_NACKF) != 0U) && ((it_sources & LL_I2C_CR1_NACKIE) != 0U))
  {
    LL_I2C_ClearFlag_NACK(MX_I2Cx);
#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    /* A NACK is expected at the end of a responder TX transfer. */
    if ((I2C_CurrentXfer == I2C_XFER_TX) && (TxCount != 0U))
    {
      I2C_ITError();
      return;
    }
#endif /* USE_LL_APP_ERROR */
  }
  else if (((it_flags & LL_I2C_ISR_RXNE) != 0U) && ((it_sources & LL_I2C_CR1_RXIE) != 0U))
  {
    if ((I2C_CurrentXfer == I2C_XFER_RX) && (RxCount > 0U))
    {
      *RxData = LL_I2C_ReceiveData8(MX_I2Cx);
      RxData++;
      RxCount--;
    }
  }
  else if (((it_flags & LL_I2C_ISR_ADDR) != 0U) && ((it_sources & LL_I2C_CR1_ADDRIE) != 0U))
  {
    /* Address matched: clear ADDR flag */
    LL_I2C_ClearFlag_ADDR(MX_I2Cx);

  }
  else if (((it_flags & LL_I2C_ISR_TXIS) != 0U) && ((it_sources & LL_I2C_CR1_TXIE) != 0U))
  {
    /* Write data to TXDR only if TxCount not reached 0: ignore dummy TXIS events during STOP treatment */
    if ((I2C_CurrentXfer == I2C_XFER_TX) && (TxCount > 0U))
    {
      LL_I2C_TransmitData8(MX_I2Cx, *TxData);
      TxData++;
      TxCount--;
    }
  }
  else
  {
    /* Nothing to do */
  }
}


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  *  Handles the MX_I2Cx error interrupts.
  */
void mx_i2cx_err_irqhandler(void)
{
  uint32_t it_flags = LL_I2C_READ_REG(MX_I2Cx, ISR);

  /* MX_I2Cx bus error interrupt occurred */
  if ((it_flags & LL_I2C_ISR_BERR) != 0U)
  {
    LL_I2C_ClearFlag_BERR(MX_I2Cx);
  }

  /* MX_I2Cx over-run/under-run interrupt occurred */
  if ((it_flags & LL_I2C_ISR_OVR) != 0U)
  {
    LL_I2C_ClearFlag_OVR(MX_I2Cx);
  }

  /* MX_I2Cx arbitration loss error interrupt occurred */
  if ((it_flags & LL_I2C_ISR_ARLO) != 0U)
  {
    LL_I2C_ClearFlag_ARLO(MX_I2Cx);
  }

  I2C_ITError();
}


/**
  * brief: This function provides delay (in milliseconds) based on SysTick counter flag.
  * param: delay_ms Number of milliseconds to delay.
  */
inline void DelayMs(uint32_t delay_ms)
{
  LL_Delay_NoISR(delay_ms);
}
#endif /* USE_LL_APP_ERROR */


/**
  *  MX_I2Cx complete transmit/receive process.
  */
static void I2C_ITComplete(uint32_t it_flags, uint32_t it_sources)
{
  uint32_t tmp_it_flags = it_flags;
  i2c_xfer_t completed_xfer = I2C_CurrentXfer;

  LL_I2C_ClearFlag_STOP(MX_I2Cx);

  /* Disable transfer-related interrupts */
  LL_I2C_DisableIT(MX_I2Cx, LL_I2C_CR1_ERRIE | LL_I2C_CR1_STOPIE | LL_I2C_CR1_NACKIE
                   | LL_I2C_CR1_ADDRIE | LL_I2C_CR1_TXIE | LL_I2C_CR1_RXIE);

  LL_I2C_WRITE_REG(MX_I2Cx, CR2, (LL_I2C_READ_REG(MX_I2Cx, CR2) & I2C_XFER_INFO));

  /* Flush any remaining TX state left by the completed transfer, especially
     after NACK/STOP, so stale TXIS/TXE conditions do not interfere with the next one. */
  I2C_FlushTxDR();

  /* Store last receive data if any */
  if ((tmp_it_flags & LL_I2C_ISR_RXNE) != 0U)
  {
    /* Remove RXNE flag on temporary variable as read done */
    tmp_it_flags &= ~LL_I2C_ISR_RXNE;

    if ((completed_xfer == I2C_XFER_RX) && (RxCount > 0U))
    {
      /* Read data from RXDR */
      *RxData = LL_I2C_ReceiveData8(MX_I2Cx);
      RxData++;
      RxCount--;
    }
  }

  if (((tmp_it_flags & LL_I2C_ISR_NACKF) != 0U) && ((it_sources & LL_I2C_CR1_NACKIE) != 0U))
  {
    LL_I2C_ClearFlag_NACK(MX_I2Cx);

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
    if ((completed_xfer == I2C_XFER_TX) && (TxCount != 0U))
    {
      I2C_ITError();
      return;
    }
#endif /* USE_LL_APP_ERROR */
  }

  I2C_CurrentXfer = I2C_XFER_NONE;

  if (completed_xfer == I2C_XFER_RX)
  {
    I2C_RxTransferCpltCallback();
  }
  else if (completed_xfer == I2C_XFER_TX)
  {
    I2C_TxTransferCpltCallback();
  }
}


#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
/**
  * brief:  MX_I2Cx Tx interrupts error process.
  */
static void I2C_ITError(void)
{
  I2C_CurrentXfer = I2C_XFER_NONE;

  /* Disable all interrupts */
  LL_I2C_DisableIT(MX_I2Cx, LL_I2C_CR1_ERRIE | LL_I2C_CR1_STOPIE | LL_I2C_CR1_NACKIE
                   | LL_I2C_CR1_TXIE | LL_I2C_CR1_RXIE);

  /* Flush any pending TX request so the peripheral does not keep a residual
     TXIS/TXE state when leaving the error path. */
  I2C_FlushTxDR();

  /* Check if a STOPF is detected */
  if (LL_I2C_IsActiveFlag_STOP(MX_I2Cx) != 0U)
  {
    if (LL_I2C_IsActiveFlag_NACK(MX_I2Cx) != 0U)
    {
      LL_I2C_ClearFlag_NACK(MX_I2Cx);
    }

    LL_I2C_ClearFlag_STOP(MX_I2Cx);
  }

  I2C_TransferErrorCallback();
}
#endif /* USE_LL_APP_ERROR */


/**
  * brief:  MX_I2Cx Tx data register flush process.
  */
static void I2C_FlushTxDR(void)
{
  /* If a pending TXIS flag is set, write a dummy data in TXDR to clear it */
  if (LL_I2C_IsActiveFlag_TXIS(MX_I2Cx) != 0U)
  {
    LL_I2C_TransmitData8(MX_I2Cx, 0x00U);
  }

  if (LL_I2C_IsActiveFlag_TXE(MX_I2Cx) == 0U)
  {
    LL_I2C_ClearFlag_TXE(MX_I2Cx);
  }
}
