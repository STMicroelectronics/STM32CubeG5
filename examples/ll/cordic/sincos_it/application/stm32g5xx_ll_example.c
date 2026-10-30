/**
  ******************************************************************************
  * file           : stm32g5xx_ll_example.c
  * brief          : Calculate sines and cosines array in IT mode,
  *                  using the CORDIC LL APIs.
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
static const int32_t *pInBuff; /* Next input word to send to CORDIC. */
static int32_t *pOutBuff; /* Next location where a CORDIC result is stored. */
static uint32_t RemainingCalculationCount; /* Calculations left after the current one. */
static uint32_t NbWrite; /* Extra input word required by the current configuration. */
static uint32_t NbRead; /* Extra output word returned by the current configuration. */
static volatile uint8_t CordicTransferInProgress = 0U; /* IT transfer state flag. */

/* Private functions prototype -----------------------------------------------*/
void mx_cordicx_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * brief:  Starts a MX_CORDICx computation in IT mode.
  * param:  p_src_buffer    Pointer to the input data buffer and the buffer size.
  * param:  p_dst_buffer    Pointer to the output data buffer and the buffer size.
  * retval: system status (see system_status_t).
  */
system_status_t CORDIC_Calculate_IT(const cordic_buffer_desc_t *p_src_buffer,
                                    cordic_buffer_desc_t *p_dst_buffer)
{
  uint32_t calculation_count;
  int32_t input_data;

  /* Retrieve the additional accesses required by the current CORDIC configuration.
    NbWrite indicates that a second 32-bit argument is required.
    NbRead indicates that a second 32-bit result is returned. */
  NbWrite = (LL_CORDIC_GetNbWrite(MX_CORDICx) == LL_CORDIC_NBWRITE_2) ? 1U : 0U;
  NbRead = (LL_CORDIC_GetNbRead(MX_CORDICx) == LL_CORDIC_NBREAD_2) ? 1U : 0U;

#if defined(USE_LL_APP_ERROR) && (USE_LL_APP_ERROR == 1)
  if ((CordicTransferInProgress != 0U) || (LL_CORDIC_IsActiveFlag_RRDY(MX_CORDICx) != 0U))
  {
    CORDIC_ErrorCallback();
    return SYSTEM_PERIPHERAL_ERROR;
  }
#endif /* USE_LL_APP_ERROR */

  calculation_count = p_src_buffer->size_word / (NbWrite + 1U);

  pInBuff = p_src_buffer->p_data;
  pOutBuff = p_dst_buffer->p_data;
  RemainingCalculationCount = calculation_count - 1U;

  CordicTransferInProgress = 1U;

  /* Write the first input data set to start the first computation. */
  LL_CORDIC_EnableIT(MX_CORDICx);
  input_data = *pInBuff;
  pInBuff++;
  LL_CORDIC_WriteData(MX_CORDICx, (uint32_t)input_data);
  if (NbWrite == 1U)
  {
    input_data = *pInBuff;
    pInBuff++;
    LL_CORDIC_WriteData(MX_CORDICx, (uint32_t)input_data);
  }

  return SYSTEM_OK;
} /* CORDIC_Calculate_IT */


void mx_cordicx_irqhandler(void)
{
  /* Read output data of the Read Data register and increment the pointer */
  *pOutBuff = (int32_t)LL_CORDIC_ReadData(MX_CORDICx);
  pOutBuff++;
  if (NbRead == 1U)
  {
    *pOutBuff = (int32_t)LL_CORDIC_ReadData(MX_CORDICx);
    pOutBuff++;
  }

  if (RemainingCalculationCount > 0U)
  {
    RemainingCalculationCount--;

    /* Write input data to the Write Data register and increment the pointer */
    LL_CORDIC_WriteData(MX_CORDICx, (uint32_t)(*pInBuff));
    pInBuff++;
    if (NbWrite == 1U)
    {
      LL_CORDIC_WriteData(MX_CORDICx, (uint32_t)(*pInBuff));
      pInBuff++;
    }
  }
  else
  {
    LL_CORDIC_DisableIT(MX_CORDICx);
    CordicTransferInProgress = 0U;
    CORDIC_CalculateCpltCallback();
  }
}
