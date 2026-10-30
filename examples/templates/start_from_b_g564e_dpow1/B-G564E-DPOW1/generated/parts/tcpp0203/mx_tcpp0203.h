/**
  ******************************************************************************
  * file           : mx_tcpp0203.h
  * brief          : Code generation for the TCPP0203 part driver.
  ******************************************************************************
  * attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the mx_tcpp0203_license.md file
  * in the same directory as the generated code.
  * If no mx_tcpp0203_license.md file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MX_TCPP0203_H
#define MX_TCPP0203_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "tcpp0203.h"
#include "mx_hal_def.h"

/* Exported macro ------------------------------------------------------------*/
/* Part Config ID */
#define TCPP0203_0                   0

/* No label has been defined for this part driver instance,
 * so no alias is generated for TCPP0203_0 or mx_tcpp0203_0_getobject(). */

/* Part interface Properties */
#define TCPP0203_0_ADC_GETHANDLE      mx_adc3_gethandle
#define TCPP0203_0_I2C_GETHANDLE      mx_i2c4_i2c_gethandle
#define TCPP0203_0_ADDRESS            0x34


/* Part cable plugged pin detection Properties */
#define TCPP0203_0_EXTI_GETHANDLE     mx_gpio_default_exti13_gethandle
#define TCPP0203_0_IRQ_LINE           EXTI13_IRQn


/* Part Enable Pin Properties */
#define TCPP0203_0_ENB_GPIO_PORT      HAL_GPIOH
#define TCPP0203_0_ENB_PIN            HAL_GPIO_PIN_1
#define TCPP0203_0_ENB_ACTIVE_STATE   HAL_GPIO_PIN_SET
/* Part VBUS detection Properties */
/* Value of VBUS voltage divider RA in Ohm */
#define TCPP0203_0_VBUS_RA          330000
/* Value of VBUS voltage divider RB in Ohm */
#define TCPP0203_0_VBUS_RB          49900
/* Value of VDD in mv */
#define TCPP0203_0_VDD              3300
/* Value of current measure shunt resistor in mOhm */
#define TCPP0203_0_IANA_ISENSE_RS   10

/* Functions to retrieve the part object */
tcpp0203_obj_t *mx_tcpp0203_0_getobject(void);

#ifdef __cplusplus
}
#endif

#endif /* MX_TCPP0203_H */
