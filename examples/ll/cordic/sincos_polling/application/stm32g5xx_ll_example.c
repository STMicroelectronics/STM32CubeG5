/**
  ******************************************************************************
  * file           : stm32g5xx_ll_example.c
  * brief          : Calculate sines and cosines array in polling mode,
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
/* Private functions prototype -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * brief:  Starts a MX_CORDICx computation in polling mode.
  * param:  p_src_buffer    Pointer to the input data buffer and the buffer size.
  * param:  p_dst_buffer    Pointer to the output data buffer and the buffer size.
  * param:  timeout_ms      Timeout duration in millisecond (This parameter is unused when USE_LL_APP_TIMEOUT == 0).
  * retval: system status (see system_status_t).
  */
inline system_status_t CORDIC_Calculate(const cordic_buffer_desc_t *p_src_buffer,
                                        cordic_buffer_desc_t *p_dst_buffer, uint32_t timeout_ms)
{
  const int32_t *p_in_buff;
  int32_t *p_out_buff;
  uint32_t computed_number_write;
  uint32_t nb_write;
  uint32_t nb_read;
  uint32_t index;
  system_status_t return_status = SYSTEM_OK;

  /* Initialize computed_number_write with the size of the input buffer */
  computed_number_write = p_src_buffer->size_word;

  p_in_buff  = p_src_buffer->p_data;
  p_out_buff = p_dst_buffer->p_data;

  /* Retrieve the additional accesses required by the current CORDIC configuration.
    nb_write indicates that a second 32-bit argument is required.
    nb_read indicates that a second 32-bit result is returned. */
  nb_write = (LL_CORDIC_GetNbWrite(MX_CORDICx) == LL_CORDIC_NBWRITE_2) ? 1U : 0U;
  nb_read  = (LL_CORDIC_GetNbRead(MX_CORDICx)  == LL_CORDIC_NBREAD_2)  ? 1U : 0U;

  /* Write the first input data set to start the first computation.
    The remaining computations are then processed in the loop with interleaved write and read accesses. */
  LL_CORDIC_WriteData(MX_CORDICx, (uint32_t)(*p_in_buff));
  p_in_buff++;
  if (nb_write == 1U)
  {
    LL_CORDIC_WriteData(MX_CORDICx, (uint32_t)(*p_in_buff));
    p_in_buff++;
  }

  index = computed_number_write - 1U;
  while (index > 0U)
  {
    /* Write input data to the Write Data register and increment the pointer */
    LL_CORDIC_WriteData(MX_CORDICx, (uint32_t)(*p_in_buff));
    p_in_buff++;
    if (nb_write == 1U)
    {
      LL_CORDIC_WriteData(MX_CORDICx, (uint32_t)(*p_in_buff));
      p_in_buff++;
    }

    /* Wait result ready */
    while (LL_CORDIC_IsActiveFlag_RRDY(MX_CORDICx) == 0U)
    {
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
      if (SysTick_IsActiveCounterFlag() != 0)
      {
        if (timeout_ms-- == 0U)
        {
          return_status = SYSTEM_PERIPHERAL_ERROR;
          break;
        }
      }
#endif /* USE_LL_APP_TIMEOUT */
    }

    /* Read output data of the Read Data register and increment the pointer */
    *p_out_buff = (int32_t)LL_CORDIC_ReadData(MX_CORDICx);
    p_out_buff++;
    if (nb_read == 1U)
    {
      *p_out_buff = (int32_t)LL_CORDIC_ReadData(MX_CORDICx);
      p_out_buff++;
    }

    index--;
  }

  /* Wait until the final computation result is available.
   This read is handled after the loop, as the last computation remains in progress
   when the loop ends and its output has not yet been read. */
  while (LL_CORDIC_IsActiveFlag_RRDY(MX_CORDICx) == 0U)
  {
#if defined(USE_LL_APP_TIMEOUT) && (USE_LL_APP_TIMEOUT == 1)
    if (SysTick_IsActiveCounterFlag() != 0)
    {
      if (timeout_ms-- == 0U)
      {
        return_status = SYSTEM_PERIPHERAL_ERROR;
        break;
      }
    }
#endif /* USE_LL_APP_TIMEOUT */
  }

  *p_out_buff = (int32_t)LL_CORDIC_ReadData(MX_CORDICx);
  p_out_buff++;
  if (nb_read == 1U)
  {
    *p_out_buff = (int32_t)LL_CORDIC_ReadData(MX_CORDICx);
  }

  return return_status;
}/* CORDIC_Calculate */
