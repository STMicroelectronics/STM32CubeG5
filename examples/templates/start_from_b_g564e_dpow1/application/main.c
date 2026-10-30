/**
  ******************************************************************************
  * file           : main.c
  * brief          : Main program body
  *                  Initializes the system, console, LED and user button, then
  *                  uses a button callback to toggle the LED in the main loop.
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
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
joy_adc_t *pJoyAdc;
rgbled_obj_t *pRgb;
tcpp0203_obj_t *pTcpp03Obj;

/* debounce timeout (ms) */
#define JOYSTICK_ADC_KEY_DEBOUNCE_TIMEOUT    100

/* RGB color sequence size */
#define RGB_COLOR_SEQUENCE_SIZE  (4U)
/* RGB brightness sequence expressed in percent for red, green, and blue. */
static const rgbled_color_t color_sequence[RGB_COLOR_SEQUENCE_SIZE] =
{
  {0, 30, 0},  /* Green  */
  {35, 0, 0},  /* Red    */
  {0, 0, 30},  /* Blue   */
  {35, 30, 0}  /* Yellow */
};

/* temporary key state used for polling */
joy_adc_key_t key_pressed = JOY_ADC_KEY_NONE;
/* current color index for RGB sequence; green is the initial color */
static uint32_t color_index = 0U;

/* Private functions prototype -----------------------------------------------*/
/**
  * brief:  The application entry point.
  * retval: none but we specify int to comply with C99 standard
  */
int main(void)
{
  /** System Init: this code placed in targets folder initializes your system.
    * It calls the initialization (and sets the initial configuration) of the peripherals.
    * You can use STM32CubeMX to generate and call this code or not in this project.
    * It also contains the HAL initialization and the initial clock configuration.
    */
  if (mx_system_init() != SYSTEM_OK)
  {
    return (-1);
  }

  /* Initialize basic stdio (for console traces) */
  if (mx_basic_stdio_init() != SYSTEM_OK)
  {
    return (-1);
  }

  /* Retrieve and store the JOYSTICK object pointer */
  pJoyAdc = mx_joystick_adc_0_getobject();

  /* Retrieve and store the RGBLED object pointer */
  pRgb = mx_rgbled_0_getobject();

  /* Retrieve and store the tcpp0203 object pointer */
  pTcpp03Obj = mx_tcpp0203_0_getobject();

  /* Parts initialization */
  if (joy_adc_init(pJoyAdc, JOYSTICK_ADC_0) != JOY_ADC_OK)
  {
    return (-1);
  }
  if (rgbled_init(pRgb, MX_RGBLED_0) != RGBLED_OK)
  {
    return (-1);
  }
  if (tcpp0203_init(pTcpp03Obj, TCPP0203_0) != TCPP0203_OK)
  {
    return (-1);
  }
  if (rgbled_set_color(pRgb, &color_sequence[color_index]) != RGBLED_OK)
  {
    return (-1);
  }
  printf("B-G564E-DPOW1 Parts initialization COMPLETED.\n");

  /* Start ADC instance */
  if (HAL_ADC_Start(mx_example_adc1_gethandle()) != HAL_OK)
  {
    return (-1);
  }
  if (HAL_ADC_Calibrate(mx_example_adc1_gethandle()) != HAL_OK)
  {
    return (-1);
  }

  /* Turn on the LED (initial status) */
  led_on(MX_STATUS_LED);
  printf("LED is turned on.\n");
  printf("Press the joystick.\n");

  /* previous key to detect press edges */
  joy_adc_key_t prev_key = JOY_ADC_KEY_NONE;

  while (1)
  {
    /* Poll joystick state */
    if (joy_adc_get_state(pJoyAdc, &key_pressed) == JOY_ADC_OK)
    {
      /* act only on transitions (press edge) to avoid repeated actions while held */
      if ((key_pressed != prev_key) && (key_pressed != JOY_ADC_KEY_NONE))
      {
        if (key_pressed == JOY_ADC_KEY_CENTER)
        {
          printf("joystick ADC KEY - CENTER\n");
          led_toggle(MX_STATUS_LED);
          printf("LED toggled.\n");
        }
        if (key_pressed == JOY_ADC_KEY_UP)
        {
          /* next color */
          color_index = (color_index + 1U) % RGB_COLOR_SEQUENCE_SIZE;
          if (rgbled_set_color(pRgb, &color_sequence[color_index]) != RGBLED_OK)
          {
            return (-1);
          }
          printf("joystick ADC KEY - UP\n");
          printf("RGB color index -> %lu\n", (unsigned long)color_index);
        }
        if (key_pressed == JOY_ADC_KEY_DOWN)
        {
          /* previous color */
          color_index = (color_index + RGB_COLOR_SEQUENCE_SIZE - 1U) % RGB_COLOR_SEQUENCE_SIZE;
          if (rgbled_set_color(pRgb, &color_sequence[color_index]) != RGBLED_OK)
          {
            return (-1);
          }
          printf("joystick ADC KEY - DOWN\n");
          printf("RGB color index -> %lu\n", (unsigned long)color_index);
        }
        if (key_pressed == JOY_ADC_KEY_LEFT)
        {
          /* set red */
          color_index = 1U;
          if (rgbled_set_color(pRgb, &color_sequence[color_index]) != RGBLED_OK)
          {
            return (-1);
          }
          printf("joystick ADC KEY - LEFT\n");
          printf("RGB color index -> %lu\n", (unsigned long)color_index);
        }
        if (key_pressed == JOY_ADC_KEY_RIGHT)
        {
          /* set blue */
          color_index = 2U;
          if (rgbled_set_color(pRgb, &color_sequence[color_index]) != RGBLED_OK)
          {
            return (-1);
          }
          printf("joystick ADC KEY - RIGHT\n");
          printf("RGB color index -> %lu\n", (unsigned long)color_index);
        }
      }

      /* remember last read key */
      prev_key = key_pressed;
    }

    /* simple debounce */
    HAL_Delay(JOYSTICK_ADC_KEY_DEBOUNCE_TIMEOUT);
  }
} /* end main */

