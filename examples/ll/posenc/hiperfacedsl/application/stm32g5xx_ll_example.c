/**
  ******************************************************************************
  * file           : stm32g5xx_ll_example.c
  * brief          : HiperfaceDSL position acquisition with LL API services
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
#define POSENC_HD_ENABLE                     (0x01UL)
#define POSENC_HD_SYNC_ES                    (0x06UL)
#define POSENC_HD_STARTUP_TIMEOUT_US         (1000000UL)
#define POSENC_HD_READ_TIMEOUT_US            (10000UL)
#define POSENC_HD_LINK_READY_MASK            (0x8FUL)
#define POSENC_HD_FAST_POSITION_MASK         (0x00FFFFFFUL)
#define POSENC_HD_TURN_LOW_MASK              (0xFF000000UL)
#define POSENC_HD_ANGLE_FULL_SCALE           (0xFFFFFFUL)
#define POSENC_HD_DEGREE_FULL_SCALE_MDEG     (360000ULL)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions prototype -----------------------------------------------*/
void mx_posencx_irqhandler(void);
static system_status_t POSENC_HiperfaceDSL_WaitForFlag(uint32_t flag, uint32_t timeout_us);
static system_status_t POSENC_HiperfaceDSL_Read1Data(uint32_t data0_address,
                                                     uint32_t *p_data0);
static system_status_t POSENC_HiperfaceDSL_Read2Data(uint32_t data0_address,
                                                     uint32_t data1_address,
                                                     uint32_t *p_data0,
                                                     uint32_t *p_data1);

/* Private functions ---------------------------------------------------------*/

/**
  *  Starts HiperfaceDSL communication in free-running mode.
  *  SYSTEM_PERIPHERAL_ERROR POSENC error
  *  SYSTEM_OK               POSENC success
  */
system_status_t POSENC_HiperfaceDSL_Start(void)
{
  system_status_t status = SYSTEM_OK;

  LL_POSENC_ClearFlag(MX_POSENCx, LL_POSENC_IT_HDFPOS);
  LL_POSENC_EnableIT(MX_POSENCx, LL_POSENC_IT_HDFPOS);

  LL_POSENC_HD_WRITE_REG(MX_POSENCx, CTRLR, ((POSENC_HD_SYNC_ES << 8UL) | POSENC_HD_ENABLE));

  return status;
}

/**
  *  Waits for the MX_POSENCx HiperfaceDSL communication to start.
  *  SYSTEM_PERIPHERAL_ERROR POSENC error
  *  SYSTEM_OK               POSENC success
  */
system_status_t POSENC_HiperfaceDSL_WaitForStartup(void)
{
  system_status_t status = SYSTEM_OK;
  uint32_t master_qm;

  status = POSENC_HiperfaceDSL_WaitForFlag(LL_POSENC_IT_HDFPOS, POSENC_HD_STARTUP_TIMEOUT_US);
  if (status == SYSTEM_OK)
  {
    status = POSENC_HiperfaceDSL_Read1Data(LL_POSENC_HD_MASTER_QMR, &master_qm);
  }

  if ((status == SYSTEM_OK) && ((master_qm & POSENC_HD_LINK_READY_MASK) != POSENC_HD_LINK_READY_MASK))
  {
    status = SYSTEM_PERIPHERAL_ERROR;
  }

  return status;
}

/**
  *  Gets the MX_POSENCx HiperfaceDSL online status.
  *  SYSTEM_PERIPHERAL_ERROR POSENC error
  *  SYSTEM_OK               POSENC success
  */
system_status_t POSENC_HiperfaceDSL_ReadStatus(uint32_t *p_online_status_d,
                                               uint32_t *p_online_status_1,
                                               uint32_t *p_online_status_2)
{
  system_status_t status = SYSTEM_OK;

  if ((p_online_status_d == NULL) || (p_online_status_1 == NULL) || (p_online_status_2 == NULL))
  {
    status = SYSTEM_PERIPHERAL_ERROR;
  }
  else
  {
    *p_online_status_d = LL_POSENC_GetOnlineStatusD(MX_POSENCx);
    *p_online_status_1 = LL_POSENC_GetOnlineStatus1(MX_POSENCx);
    *p_online_status_2 = LL_POSENC_GetOnlineStatus2(MX_POSENCx);
  }

  return status;
}

/**
  *  Gets the MX_POSENCx HiperfaceDSL encoder position.
  *  SYSTEM_PERIPHERAL_ERROR POSENC error
  *  SYSTEM_OK               POSENC success
  */
system_status_t POSENC_HiperfaceDSL_GetPosition(uint32_t *p_turn, uint32_t *p_position_millidegree)
{
  system_status_t status = SYSTEM_OK;
  uint32_t position_low;
  uint32_t position_high;
  uint64_t angle_mdeg;

  if ((p_turn == NULL) || (p_position_millidegree == NULL))
  {
    status = SYSTEM_PERIPHERAL_ERROR;
  }
  else
  {
    status = POSENC_HiperfaceDSL_Read2Data(LL_POSENC_HD_FPOSMT1,
                                           LL_POSENC_HD_FPOSMT2,
                                           &position_low,
                                           &position_high);
    if (status == SYSTEM_OK)
    {
      *p_turn = (position_high << 8U) | ((position_low & POSENC_HD_TURN_LOW_MASK) >> 24U);

      angle_mdeg = ((uint64_t)(position_low & POSENC_HD_FAST_POSITION_MASK) * POSENC_HD_DEGREE_FULL_SCALE_MDEG)
                   / POSENC_HD_ANGLE_FULL_SCALE;
      *p_position_millidegree = angle_mdeg;
    }
  }

  return status;
}

/**
  *  Waits for a MX_POSENCx flag with a timeout.
  *  SYSTEM_PERIPHERAL_ERROR POSENC error
  *  SYSTEM_OK               POSENC success
  */
static system_status_t POSENC_HiperfaceDSL_WaitForFlag(uint32_t flag, uint32_t timeout_us)
{
  system_status_t status = SYSTEM_OK;
  uint32_t remaining_time_us = timeout_us;

  while ((LL_POSENC_GetFlag(MX_POSENCx, flag) != 1UL) && (remaining_time_us > 0UL))
  {
    DelayUs(1U);
    remaining_time_us--;
  }

  if (remaining_time_us == 0UL)
  {
    status = SYSTEM_PERIPHERAL_ERROR;
  }

  return status;
}

/**
  *  Reads data from the MX_POSENCx HiperfaceDSL registers.
  *  SYSTEM_PERIPHERAL_ERROR POSENC error
  *  SYSTEM_OK               POSENC success
  */
static system_status_t POSENC_HiperfaceDSL_Read1Data(uint32_t data0_address,
                                                     uint32_t *p_data0)
{
  system_status_t status = SYSTEM_OK;

  if (p_data0 == NULL)
  {
    status = SYSTEM_PERIPHERAL_ERROR;
  }
  else
  {
    LL_POSENC_ClearFlag(MX_POSENCx, LL_POSENC_IT_D0RTC);

    LL_POSENC_RequestReadData0(MX_POSENCx, data0_address);

    if (POSENC_HiperfaceDSL_WaitForFlag(LL_POSENC_IT_D0RTC, POSENC_HD_READ_TIMEOUT_US) != SYSTEM_OK)
    {
      status = SYSTEM_PERIPHERAL_ERROR;
    }
    else
    {
      *p_data0 = LL_POSENC_ReadRequestedData0(MX_POSENCx);
    }
  }

  return status;
}

/**
  *  Reads data from the MX_POSENCx HiperfaceDSL registers.
  *  SYSTEM_PERIPHERAL_ERROR POSENC error
  *  SYSTEM_OK               POSENC success
  */
static system_status_t POSENC_HiperfaceDSL_Read2Data(uint32_t data0_address,
                                                     uint32_t data1_address,
                                                     uint32_t *p_data0,
                                                     uint32_t *p_data1)
{
  system_status_t status = SYSTEM_OK;

  if ((p_data0 == NULL) || (p_data1 == NULL))
  {
    status = SYSTEM_PERIPHERAL_ERROR;
  }
  else
  {
    LL_POSENC_ClearFlag(MX_POSENCx, LL_POSENC_IT_D0RTC | LL_POSENC_IT_D1RTC);

    LL_POSENC_RequestReadData0(MX_POSENCx, data0_address);
    LL_POSENC_RequestReadData1(MX_POSENCx, data1_address);

    if (POSENC_HiperfaceDSL_WaitForFlag(LL_POSENC_IT_D0RTC, POSENC_HD_READ_TIMEOUT_US) != SYSTEM_OK)
    {
      status = SYSTEM_PERIPHERAL_ERROR;
    }
    else
    {
      *p_data0 = LL_POSENC_ReadRequestedData0(MX_POSENCx);
    }

    if (POSENC_HiperfaceDSL_WaitForFlag(LL_POSENC_IT_D1RTC, POSENC_HD_READ_TIMEOUT_US) != SYSTEM_OK)
    {
      status = SYSTEM_PERIPHERAL_ERROR;
    }
    else
    {
      *p_data1 = LL_POSENC_ReadRequestedData1(MX_POSENCx);
    }
  }

  return status;
}

/**
  *  Handles the MX_POSENCx interrupts.
  */
void mx_posencx_irqhandler(void)
{
  if (LL_POSENC_GetFlag(MX_POSENCx, LL_POSENC_IT_HDFPOS) == 1UL)
  {
    LL_POSENC_ClearFlag(MX_POSENCx, LL_POSENC_IT_HDFPOS);
    POSENC_NewDataCallback();
  }
}

/**
  *  Starts the MX_TIMx counter.
  *  SYSTEM_PERIPHERAL_ERROR MX_TIMx error
  *  SYSTEM_OK               MX_TIMx success
  */
system_status_t POSENC_TimerStart(void)
{
  system_status_t status = SYSTEM_OK;

  LL_TIM_EnableCounter(MX_TIMx);

  return status;
}

/**
  *  Stops the MX_TIMx counter.
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
