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
typedef enum
{
  ED3_FG_DATA0   = 0x00, /* Activate LPF Send list 0 */
  ED3_FG_DATA1   = 0x01, /* Activate LPF Send list 1 */
  ED3_FG_DATA2   = 0x02, /* Activate LPF Send list 2 */
  ED3_FG_DATA3   = 0x03, /* Activate LPF Send list 3 */
  ED3_FG_DATA4   = 0x04, /* Activate LPF Send list 4 */
  ED3_FG_DATA5   = 0x05, /* Activate LPF Send list 5 */
  ED3_FG_DATA6   = 0x06, /* Activate LPF Send list 6 */
  ED3_FG_DATA7   = 0x07, /* Activate LPF Send list 7 */
  ED3_FG_DATA    = 0x08, /* - */
  ED3_FG_DATANOP = 0x09, /* DATA without BGD */
  ED3_FG_RESET   = 0x0B, /* Encoder reset */
  ED3_FG_CLEAR   = 0x0C, /* Resetting of states */
  ED3_FG_ECHO    = 0x0E, /* Echo for measuring the propagation time */
  ED3_FG_RATE    = 0x10, /* Set data transfer rate */
  ED3_FG_HELLO   = 0x22, /* Switch to EnDat 3 mode */
} ED3_FG_request_t;

typedef enum
{
  ED3_FID_NOP             = 0x00, /* Empty content */
  ED3_FID_POS1            = 0x01, /* Position 1 */
  ED3_FID_POS2            = 0x02, /* Position 2 */
  ED3_FID_TOUCHPROBE      = 0x03, /* Probe signal and timestamp for touch probes */
  ED3_FID_POS_ABS         = 0x04, /* Absolute position for incremental encoders */
  ED3_FID_ZERODATA        = 0x05, /* Data values 0 */
  ED3_FID_ERRMSG          = 0x0A, /* Error messages and warning messages */
  ED3_FID_EVALNUM         = 0x11, /* Valuation numbers */
  ED3_FID_MOUNT0          = 0x12, /* Mounting parameters */
  ED3_FID_MOUNT1          = 0x13, /* Mounting parameters */
  ED3_FID_COMMU           = 0x1A, /* Commutation and limit position */
  ED3_FID_SENSOR_TEMP_MAX = 0x20, /* Maximum value from 'motor winding temperature external 1, 2, and 3' */
  ED3_FID_SENSOR_TEMP_INT = 0x21, /* Internal temperature of the encoder */
  ED3_FID_SENSOR_TEMP_M1  = 0x22, /* Motor winding temperature external 1 */
  ED3_FID_SENSOR_TEMP_M2  = 0x23, /* Motor winding temperature external 2 */
  ED3_FID_SENSOR_TEMP_M3  = 0x24, /* Motor winding temperature external 3 */
  ED3_FID_SF_POS1         = 0x50, /* Safety frame for validation of the FID POS1 (0x01) */
  ED3_FID_BGRSP           = 0x60, /* Most recently generated BG response */
  ED3_FID_BGREQ           = 0x68, /* Most recently processed BG request */
} ED3_FID_t;

typedef enum
{
  ED3_BG_NOP         = 0x01, /* NOP does not perform any function. */
  ED3_BG_READ        = 0x02, /* READ reads from the encoder memory. */
  ED3_BG_WRITE       = 0x03, /* WRITE writes directly to the encoder memory. */
  ED3_BG_RECONFIGURE = 0x04, /* RECONFIGURE activates the configurations located in non-volatile memories SET, XSET, and
                                LPF_SET at run-time*/
  ED3_BG_AUTH        = 0x80, /* AUTH serves to authenticate the user via an enabling code that is transmitted in plain
                                language. This can switch the current user level (see "Access protection", Page 63). */
  ED3_BG_PROTECT     = 0x81, /* PROTECT controls the protection of memory areas. */
  ED3_BG_SETPASS     = 0x83, /* SETPASS changes the enabling code that is required for user authentication with AUTH. */
  ED3_BG_LOCATE      = 0x84, /* LOCATE helps the user to find the installed encoder (e.g. with a blinking light). */
} ED3_BG_OpCodes_t;

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** brief:  Configures the MX_POSENCx EnDat3 clock division.
  * retval: system status (see system_status_t)
  */
system_status_t POSENC_EnDat3_SetClockDiv(void);

/** brief:  Sets the MX_POSENCx EnDat3 HELLO request.
  * retval: system status (see system_status_t)
  */
system_status_t POSENC_EnDat3_SetHello(void);

/** brief:  Enables the MX_POSENCx EnDat3 hardware strobe.
  * retval: system status (see system_status_t)
  */
system_status_t POSENC_EnDat3_EnableHardwareStrobe(void);

/** brief:  Sets the MX_POSENCx EnDat3 foreground request.
  * retval: system status (see system_status_t)
  */
system_status_t POSENC_EnDat3_SetRequest_Foreground(uint32_t REQ_CODE, uint32_t REQ_DATA);

/** brief:  Activates the MX_TIMx instance.
  * retval: system status (see system_status_t)
  */
system_status_t POSENC_TimerStart(void);

/** brief:  Deactivates the MX_TIMx instance.
  * retval: system status (see system_status_t)
  */
system_status_t POSENC_TimerStop(void);

/** brief:  Enables the MX_POSENCx EnDat3 interrupt.
  * retval: system status (see system_status_t)
  */
system_status_t POSENC_EnDat3_EnableIT(void);

/** brief:  Gets the MX_POSENCx EnDat3 encoder position.
  * retval: system status (see system_status_t)
  */
system_status_t POSENC_GetPosition(uint32_t *p_turn, uint32_t *p_position_millidegree);

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
