/**
  ******************************************************************************
  * file           : ll_example.h
  * brief          : Header for stm32ynxx_ll_example.c files.
  *                  This file contains example-specific declarations to interface with example.c functions.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef LL_EXAMPLE_H
#define LL_EXAMPLE_H

/* Includes ------------------------------------------------------------------*/
#include "mx_hal_def.h"       /* aliases to the target-specific generated code */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** brief:  Starts the MX_POSENCx HiperfaceDSL communication.
  * retval: system status (see system_status_t)
  */
system_status_t POSENC_HiperfaceDSL_Start(void);

/** brief:  Waits for the MX_POSENCx HiperfaceDSL communication to start.
  * retval: system status (see system_status_t)
  */
system_status_t POSENC_HiperfaceDSL_WaitForStartup(void);

/** brief:  Gets the MX_POSENCx HiperfaceDSL online status.
  * retval: system status (see system_status_t)
  */
system_status_t POSENC_HiperfaceDSL_ReadStatus(uint32_t *p_online_status_d,
                                               uint32_t *p_online_status_1,
                                               uint32_t *p_online_status_2);

/** brief:  Gets the MX_POSENCx HiperfaceDSL encoder position.
  * retval: system status (see system_status_t)
  */
system_status_t POSENC_HiperfaceDSL_GetPosition(uint32_t *p_turn, uint32_t *p_position_degree);

/** brief:  Activates the MX_TIMx instance.
  * retval: system status (see system_status_t)
  */
system_status_t POSENC_TimerStart(void);

/** brief:  Deactivates the MX_TIMx instance.
  * retval: system status (see system_status_t)
  */
system_status_t POSENC_TimerStop(void);

/** brief:  Delays execution for an approximate number of microseconds.
  */
void DelayUs(uint32_t delay_us);

/** brief:  Handles the MX_POSENCx new data callback.
  */
void POSENC_NewDataCallback(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LL_EXAMPLE_H */
