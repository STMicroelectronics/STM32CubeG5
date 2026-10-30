/**
  ******************************************************************************
  * file           : example.c
  * brief          : DMA transfer using several variant of data handling.
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
#include "example.h"
#include <string.h> /* Used for string/buffer operations like memset(), memcmp() */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE_IN_BYTES 8U
/* @user: The maximum data bus width used by DMA in STM32 devices is 64 bits.
          Therefore, 8-byte alignment is the minimum recommended alignment for DMA buffers across STM32 devices. */
#define DMA_ACCESS_ALIGNMENT        (8U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_dma_handle_t *pDMA;  /* pointer referencing the DMA handle from the generated code */

/** SrcBuffer is declared as const, so that it is placed in Flash.
  * 8-byte alignment covers the maximum STM32 DMA data bus width.
  * @user: It is possible to modify the buffer content and the length, update BUFFER_SIZE_IN_BYTES if necessary.
  *        Expected buffers must then be updated accordingly.
  */
__ALIGNED(DMA_ACCESS_ALIGNMENT) const uint8_t SrcBuffer[BUFFER_SIZE_IN_BYTES] =
{
  0xB0, 0xB1, 0xB2, 0xB3,
  0xB4, 0xB5, 0xB6, 0xB7
};

/** Destination buffer. Its size is twice larger than the source buffer to take into account the padding cases.
  * 8-byte alignment covers the maximum STM32 DMA data bus width.
  * - Non-cacheable memory for data cache consistency.
  * - Placed in the dedicated 1 KB linker/MPU window reserved for CPU/DMA shared data.
  * - Mandatory with data cache enabled, harmless otherwise: portable across STM32 series.
  */
__attribute__((section("non_cacheable_area"), aligned(DMA_ACCESS_ALIGNMENT)))
static uint8_t DstBuffer[BUFFER_SIZE_IN_BYTES * 2U];

/* Right aligned zero padding data expected result buffer */
const uint8_t ExpectedBufferRightAlignZeroPadding[2 * BUFFER_SIZE_IN_BYTES] =
{
  0xB0, 0x00, 0xB1, 0x00,
  0xB2, 0x00, 0xB3, 0x00,
  0xB4, 0x00, 0xB5, 0x00,
  0xB6, 0x00, 0xB7, 0x00
};

/* Right aligned sign extended data expected result buffer*/
const uint8_t ExpectedBufferRightAlignSignExtend[2 * BUFFER_SIZE_IN_BYTES] =
{
  0xB0, 0xFF, 0xB1, 0xFF,
  0xB2, 0xFF, 0xB3, 0xFF,
  0xB4, 0xFF, 0xB5, 0xFF,
  0xB6, 0xFF, 0xB7, 0xFF
};

/* Right aligned left truncated data expected result buffer */
const uint8_t ExpectedBufferRightAlignLeftTrunc[BUFFER_SIZE_IN_BYTES] =
{
  0xB0, 0xB2, 0xB4, 0xB6,
  0x00, 0x00, 0x00, 0x00
};

/* Left aligned right truncated data expected result buffer */
const uint8_t ExpectedBufferLeftAlignRightTrunc[BUFFER_SIZE_IN_BYTES] =
{
  0xB1, 0xB3, 0xB5, 0xB7,
  0x00, 0x00, 0x00, 0x00
};

/* Source byte exchange expected result buffer */
const uint8_t ExpectedBufferSrcByteExchange[BUFFER_SIZE_IN_BYTES] =
{
  0xB0, 0xB2, 0xB1, 0xB3,
  0xB4, 0xB6, 0xB5, 0xB7
};

/* Destination byte exchange expected result buffer */
const uint8_t ExpectedBufferDestByteExchange[BUFFER_SIZE_IN_BYTES] =
{
  0xB1, 0xB0, 0xB3, 0xB2,
  0xB5, 0xB4, 0xB7, 0xB6
};

/* Destination half-word exchange expected result buffer */
const uint8_t ExpectedBufferDestHalfWordExchange[BUFFER_SIZE_IN_BYTES] =
{
  0xB2, 0xB3, 0xB0, 0xB1,
  0xB6, 0xB7, 0xB4, 0xB5
};

volatile uint8_t TransferError;    /* Set to 1 if a transfer error is detected */
volatile uint8_t TransferComplete; /* Set to 1 if the transfer is correctly completed */

/* Private functions prototype -----------------------------------------------*/
static app_status_t CheckDmaAndBuffer(const uint8_t *expected_buffer, uint16_t size);
static void TransferCompleteCallback(hal_dma_handle_t *handle_dma);
static void TransferErrorCallback(hal_dma_handle_t *handle_dma);

static app_status_t TestDma(hal_dma_handle_t *(*dma_init_func)(void), uint16_t src_size,
                            const uint8_t *expected_buffer, uint16_t dst_size);


/** Nothing to do: DMA is initialized for each data handling configuration in step 1
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_INIT_OK;

  return return_status;
} /* end app_init */


/** Demonstrates the following variants of DMA data handling configuration:
  *   - Right aligned zero padding
  *   - Right aligned sign extended data
  *   - Right aligned left truncated data
  *   - Left aligned right truncated data
  *   - Source byte exchanged
  *   - Destination byte exchanged
  *   - Destination half-word exchanged
  */
app_status_t app_process(void)
{
  app_status_t return_status;

  /** In this configuration the DMA source datalength is byte, whereas the destination datalength is half-word.
    * Consequently, the destination buffer is twice larger than the source size.
    */
  return_status = TestDma(&mx_example_dma_init_right_align_zero_padding, BUFFER_SIZE_IN_BYTES,
                          ExpectedBufferRightAlignZeroPadding, 2 * BUFFER_SIZE_IN_BYTES);
  if (return_status != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }

  /** In this configuration the DMA source datalength is byte, whereas the destination datalength is half-word.
    * Consequently, the destination buffer is twice larger than the source size.
    */
  return_status = TestDma(&mx_example_dma_init_right_align_sign_extend, BUFFER_SIZE_IN_BYTES,
                          ExpectedBufferRightAlignSignExtend, 2 * BUFFER_SIZE_IN_BYTES);
  if (return_status != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }

  /** In this configuration the DMA source datalength is half-word, whereas the destination datalength is byte.
    * Consequently, the number of significant bytes in the expected buffer is half of the original size.
    */
  return_status = TestDma(&mx_example_dma_init_right_align_left_trunc, BUFFER_SIZE_IN_BYTES,
                          ExpectedBufferRightAlignLeftTrunc, BUFFER_SIZE_IN_BYTES);
  if (return_status != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }

  /** In this configuration the DMA source datalength is half-word, whereas the destination datalength is byte.
    * Consequently, the number of significant bytes in the expected buffer is half of the original size.
    */
  return_status = TestDma(&mx_example_dma_init_left_align_right_trunc, BUFFER_SIZE_IN_BYTES,
                          ExpectedBufferLeftAlignRightTrunc, BUFFER_SIZE_IN_BYTES);
  if (return_status != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }

  /** In this configuration the DMA source and destination datalength are word.
    * Only data order is changed.
    */
  return_status = TestDma(&mx_example_dma_init_src_byte_exchange, BUFFER_SIZE_IN_BYTES,
                          ExpectedBufferSrcByteExchange, BUFFER_SIZE_IN_BYTES);
  if (return_status != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }

  /** In this configuration the DMA source and destination datalength are word.
    * Only data order is changed.
    */
  return_status = TestDma(&mx_example_dma_init_dest_byte_exchange, BUFFER_SIZE_IN_BYTES,
                          ExpectedBufferDestByteExchange, BUFFER_SIZE_IN_BYTES);
  if (return_status != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }

  /** In this configuration the DMA source and destination datalength are word.
    * Only data order is changed.
    */
  return_status = TestDma(&mx_example_dma_init_dest_halfword_exchange, BUFFER_SIZE_IN_BYTES,
                          ExpectedBufferDestHalfWordExchange, BUFFER_SIZE_IN_BYTES);
  if (return_status != EXEC_STATUS_OK)
  {
    goto _app_process_exit;
  }

_app_process_exit:
  return return_status;
} /* end app_process */


/** Launches the DMA test
  * dma_init_func: dma initialization function
  * src_size: size of the DMA source buffer in bytes
  * expected_buffer: expected buffer
  * dst_size: size of the DMA destination buffer in bytes
  */
static app_status_t TestDma(hal_dma_handle_t *(*dma_init_func)(void), uint16_t src_size,
                            const uint8_t *expected_buffer, uint16_t dst_size)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Clear destination buffer and reset transfer flags before starting a new DMA transfer */
  memset((void *)DstBuffer, 0U, sizeof(DstBuffer));
  TransferError = 0;
  TransferComplete = 0;

  /** ########## Step 1 ##########
    * Initializes the DMA according to the selected configuration,
    * including the data handling configuration.
    */
  pDMA = dma_init_func();
  if (pDMA == NULL)
  {
    goto _TestDma_exit;
  }

  /* Select Callbacks functions called after Transfer complete and Transfer error */
  if (HAL_DMA_RegisterXferCpltCallback(pDMA, TransferCompleteCallback) != HAL_OK)
  {
    goto _TestDma_exit;
  }
  if (HAL_DMA_RegisterXferErrorCallback(pDMA, TransferErrorCallback) != HAL_OK)
  {
    goto _TestDma_exit;
  }

  /** ########## Step 2 ##########
    * Starts the DMA transfer in interrupt mode.
    */
  if (HAL_DMA_StartDirectXfer_IT(pDMA, (uint32_t)SrcBuffer, (uint32_t)DstBuffer, src_size)  != HAL_OK)
  {
    goto _TestDma_exit;
  }

  /** ########## Step 3 ##########
    * Waits for the DMA transfer completion and verifies the destination buffer.
    */
  if (CheckDmaAndBuffer(expected_buffer, dst_size) != EXEC_STATUS_OK)
  {
    goto _TestDma_exit;
  }

  /** ########## Step 4 ##########
    * Deinitialize the DMA to be ready for the next DMA configuration
    */
  if (mx_example_dma_deinit()  != -1)
  {
    return_status = EXEC_STATUS_OK;
  }

_TestDma_exit:
  return return_status;
}


/** Waits for end of the DMA transfer and checks the destination buffer matches to the expected one.
  * expected_buffer: pointer to the expected data buffer
  * size: size of the expected buffer in bytes
  */
static app_status_t CheckDmaAndBuffer(const uint8_t *expected_buffer, uint16_t size)
{
  app_status_t return_status  = EXEC_STATUS_ERROR;

  /* Wait for the DMA transfer complete or a DMA error */
  while (!TransferComplete && !TransferError)
  {
    /** Put the CPU in Wait For Interrupt state. A DMA interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the DMA. It can be replaced by your own code.
      */
    __WFI();
  }

  if ((TransferComplete != 1) || (TransferError == 1))
  {
    goto _CheckDmaAndBuffer_exit;
  }

  /* Check the destination buffer */
  if (memcmp(DstBuffer, expected_buffer, size) != 0)
  {
    goto _CheckDmaAndBuffer_exit;
  }

  return_status = EXEC_STATUS_OK;

_CheckDmaAndBuffer_exit:
  return return_status;
}

app_status_t app_deinit(void)
{
  /* Nothing to do: DMA is already deinitilized */
  return EXEC_STATUS_OK;
} /* end app_deinit */


/** DMA transfer complete callback
  * @user This implementation of the DMA complete callback can be customized.
  * This function is executed when the DMA transfer complete interrupt is generated.
  */
static void TransferCompleteCallback(hal_dma_handle_t *handle_dma)
{
  /* asynchronous processing related to step 3 */
  TransferComplete = 1;
}


/** DMA transfer error callback
  * @user This implementation of the DMA error callback can be customized.
  * This function is executed when the DMA transfer error interrupt is generated during the DMA transfer.
  */
static void TransferErrorCallback(hal_dma_handle_t *handle_dma)
{
  /* asynchronous processing related to step 3 */
  TransferError = 1;
}
