/**
  ******************************************************************************
  * file           : mx_tcpp0203.c
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

/* Includes ------------------------------------------------------------------*/
#include "mx_tcpp0203.h"

static tcpp0203_obj_t  tcpp0203_0_obj;

/**
  * @brief initialize the IO layer.
  * @param  pio      pointer to the part IO structure.
  * @retval error status.
  */
int32_t tcpp0203_io_init(tcpp0203_io_t *pio)
{
  /* Part Status */
  int ret = 0;

  switch (pio->id)
  {
    case TCPP0203_0:
      pio->phadc        = TCPP0203_0_ADC_GETHANDLE();
      pio->phi2c        = TCPP0203_0_I2C_GETHANDLE();
      pio->addr         = (TCPP0203_0_ADDRESS << 1);
      pio->enb_port     = TCPP0203_0_ENB_GPIO_PORT;
      pio->enb_pin      = TCPP0203_0_ENB_PIN;
      pio->active_state = TCPP0203_0_ENB_ACTIVE_STATE;
      pio->phexti       = TCPP0203_0_EXTI_GETHANDLE();
      pio->irq_line     = TCPP0203_0_IRQ_LINE;
      pio->Ra           = TCPP0203_0_VBUS_RA;
      pio->Rb           = TCPP0203_0_VBUS_RB;
      pio->Rs           = TCPP0203_0_IANA_ISENSE_RS;
      pio->Vdd          = TCPP0203_0_VDD;

      break;

    default:
      /* Error -- Unknown ID */
      ret = -1;
      break;
  }

  return ret;
}

/**
  * @brief Function to retrieve the Part object 0.
  * @retval part object.
  */
tcpp0203_obj_t *mx_tcpp0203_0_getobject(void)
{
  return &tcpp0203_0_obj;
}
