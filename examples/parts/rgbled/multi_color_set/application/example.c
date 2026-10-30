/**
  ******************************************************************************
  * file           : example.c
  * brief          : multi-color sequence example based on the RGBLED part API
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* @user: configure the wait delay in milliseconds after the RGB color is changed */
#define EXAMPLE_WAIT_DELAY_ON_MS (500U)
#define EXAMPLE_COLOR_SEQUENCE_SIZE  (5U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RGBLED object provided by the generated bindings for the selected board. */
rgbled_obj_t *pRGBLEDObj;

/* RGB brightness sequence expressed in percent for red, green, and blue. */
static const rgbled_color_t color_sequence[EXAMPLE_COLOR_SEQUENCE_SIZE] =
{
  {30, 30, 30},
  {35, 00, 30},
  {35, 30, 00},
  {00, 30, 30},
  {00, 00, 00}
};

/* Private functions prototype -----------------------------------------------*/
/** ########## Step 1 ##########
  * The applicative code initializes the RGBLED part driver.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Retrieve the board-specific RGBLED object exposed by generated code. */
  pRGBLEDObj = mx_status_rgbled_getobject();

  /* Initialize the RGBLED part driver before any color update is attempted. */
  if (rgbled_init(pRGBLEDObj, MX_STATUS_RGBLED) == RGBLED_OK)
  {
    return_status = EXEC_STATUS_INIT_OK;
  }

  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  /** ########## Step 2 ##########
    * Cycles white, magenta, yellow, cyan, and off indefinitely.
    */
  for (uint32_t color_idx = 0U; color_idx < EXAMPLE_COLOR_SEQUENCE_SIZE; color_idx++)
  {
    /* Apply the next RGB brightness tuple from the demonstration sequence. */
    if (rgbled_set_color(pRGBLEDObj, &color_sequence[color_idx]) != RGBLED_OK)
    {
      return EXEC_STATUS_ERROR;
    }

    /* Keep the current color visible long enough to observe the transition. */
    HAL_Delay(EXAMPLE_WAIT_DELAY_ON_MS);
  }

  return EXEC_STATUS_OK;
} /* end app_process */


/** Deinitializes the RGBLED part driver.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status;

  /* Release the RGBLED instance resources if the application ever stops. */
  if (rgbled_deinit(pRGBLEDObj) == RGBLED_ERROR)
  {
    return_status = EXEC_STATUS_ERROR;
  }
  else
  {
    return_status = EXEC_STATUS_OK;
  }

  return return_status;
} /* end app_deinit */
