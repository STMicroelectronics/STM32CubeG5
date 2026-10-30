/**
  ******************************************************************************
  * file           : stm32g5xx_ll_example.c
  * brief          : LL POSENC EnDat3 position acquisition example
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
void mx_posencx_irqhandler(void);

/* Private functions ---------------------------------------------------------*/

/**
  *  Configures the MX_POSENCx EnDat3 clock division.
  *  SYSTEM_PERIPHERAL_ERROR POSENC error
  *  SYSTEM_OK               POSENC success
  */
system_status_t POSENC_EnDat3_SetClockDiv(void)
{
  system_status_t status = SYSTEM_OK;

  /* Refer to EnDat3 reference manual for clock division settings */
  LL_POSENC_ED3_WRITE_REG(POSENC1, CONFIG_1, (16U) << 16U);
  LL_POSENC_ED3_WRITE_REG(POSENC1, CONFIG_2, ((18U) << 16U) | (9U));
  LL_POSENC_ED3_WRITE_REG(POSENC1, CONFIG_3, ((26U) << 16U) | (19U));

  return status;
}

/**
  *  Enables the MX_POSENCx EnDat3 hardware strobe.
  *  SYSTEM_PERIPHERAL_ERROR POSENC error
  *  SYSTEM_OK               POSENC success
  */
system_status_t POSENC_EnDat3_EnableHardwareStrobe(void)
{
  system_status_t status = SYSTEM_OK;

  LL_POSENC_ED3_MODIFY_REG(POSENC1, CONFIG_0, 0x1UL, 0x1UL);
  return status;
}

/**
  *  Sets the MX_POSENCx EnDat3 HELLO request.
  *  SYSTEM_PERIPHERAL_ERROR POSENC error
  *  SYSTEM_OK               POSENC success
  */
system_status_t POSENC_EnDat3_SetHello(void)
{
  system_status_t status = SYSTEM_OK;
  LL_POSENC_ED3_MODIFY_REG(POSENC1,
                           FG_REQ_1,
                           0x00FFFFFFUL,
                           (((uint32_t)ED3_FG_HELLO & 0xFFU) << 16U) | (0x2222U & 0xFFFFU));
  LL_POSENC_ED3_MODIFY_REG(POSENC1, FG_REQ_1, 0xFF000000UL, ((0x1U) << 24U));

  return status;
}

/**
  *  Sets the MX_POSENCx EnDat3 foreground request.
  *  SYSTEM_PERIPHERAL_ERROR POSENC error
  *  SYSTEM_OK               POSENC success
  */
system_status_t POSENC_EnDat3_SetRequest_Foreground(uint32_t REQ_CODE, uint32_t REQ_DATA)
{
  system_status_t status = SYSTEM_OK;
  LL_POSENC_ED3_MODIFY_REG(POSENC1,
                           FG_REQ_1, 0x00FFFFFFUL,
                           (((uint32_t)REQ_CODE & 0xFFU) << 16U) | ((uint32_t)REQ_DATA & 0xFFFFU));
  return status;
}


/**
  *  Enables the MX_POSENCx EnDat3 interrupt.
  *  SYSTEM_PERIPHERAL_ERROR POSENC error
  *  SYSTEM_OK               POSENC success
  */
system_status_t POSENC_EnDat3_EnableIT(void)
{
  system_status_t status = SYSTEM_OK;

  LL_POSENC_ED3_WRITE_REG(POSENC1, FG_IRQ0_MASK, (1UL << 16UL) | (0x1UL << 31UL));

  return status;
}

/**
  *  Gets the MX_POSENCx EnDat3 encoder position.
  *  SYSTEM_PERIPHERAL_ERROR POSENC error
  *  SYSTEM_OK               POSENC success
  */
system_status_t POSENC_GetPosition(uint32_t *p_turn, uint32_t *p_position_millidegree)
{
  system_status_t status = SYSTEM_OK;
  uint32_t hpf_low;
  uint32_t hpf_high;
  uint64_t angle_mdeg;

  if ((p_turn == NULL) || (p_position_millidegree == NULL))
  {
    status = SYSTEM_PERIPHERAL_ERROR;
  }
  else
  {
    /* Read result from cycle-based memory */
    hpf_low = LL_POSENC_ED3_ReadCycleBasedMemory(POSENC1, LL_POSENC_ED3_MEM_HPF, LL_POSENC_ED3_MEM_LOW);
    hpf_high = LL_POSENC_ED3_ReadCycleBasedMemory(POSENC1, LL_POSENC_ED3_MEM_HPF, LL_POSENC_ED3_MEM_HIGH);

    *p_turn = hpf_high & 0xFFFFUL;

    /* Convert one-turn raw position to milli-degrees with integer arithmetic */
    angle_mdeg = ((uint64_t)hpf_low * 360000ULL) / 0xFFFFFFFFULL;
    *p_position_millidegree = angle_mdeg;
  }

  return status;
}

/**
  *  Handles the MX_POSENCx EnDat3 interrupts.
  */
void mx_posencx_irqhandler(void)
{
  uint32_t ed3_fg_status = LL_POSENC_ED3_READ_REG(POSENC1, FG_STATUS) & (0xF0000000UL);

  /* FG_IRQ0_STATE */
  if ((ed3_fg_status & (0x01UL << 29U)) != 0UL)
  {
    LL_POSENC_ED3_WRITE_REG(POSENC1, IRQ_CLEAR, (0x01U << 1U));
    POSENC_NewDataCallback();
  }
}


/**
  *  Activates the MX_TIMx instance.
  *  SYSTEM_PERIPHERAL_ERROR MX_TIMx error
  *  SYSTEM_OK               MX_TIMx success
  */
system_status_t POSENC_TimerStart(void)
{
  system_status_t status = SYSTEM_OK;

  LL_TIM_EnableCounter(MX_TIMx);
  LL_TIM_CC_EnableChannel(MX_TIMx, LL_TIM_CHANNEL_CH1);

  return status;
}

/**
  *  Deactivates the MX_TIMx instance.
  *  SYSTEM_PERIPHERAL_ERROR MX_TIMx error
  *  SYSTEM_OK               MX_TIMx success
  */
system_status_t POSENC_TimerStop(void)
{
  system_status_t status = SYSTEM_OK;

  LL_TIM_DisableCounter(MX_TIMx);

  return status;
}

/**
  * Delays execution for an approximate number of microseconds.
  * Calculates the wait loop count based on the CMSIS variable "SystemCoreClock".
  * The delay is approximate and affected by CPU pipeline, branch prediction, cache, and compiler optimizations.
  * For core clock frequencies below 500 kHz, the delay is limited to a few CPU cycles.
  *
  * warning: This function provides only an approximate delay and must not be used for precise timing.
  * param: delay_us Number of microseconds to delay.
  */
void DelayUs(uint32_t delay_us)
{
  volatile uint32_t wait_loop_index = ((delay_us * (SystemCoreClock >> 19U)) >> 2U);
  while (wait_loop_index != 0U)
  {
    wait_loop_index--;
  }
}
