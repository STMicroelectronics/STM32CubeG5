/**
  ******************************************************************************
  * file           : main.c
  * brief          : Main program body
  *                  Initializes the system, console, LED, button
  *                  and potentiometer, then uses a button callback to toggle
  *                  the LED and print the potentiometer level in the main loop.
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
button_t *pButton;                /* pointer referencing the Button object instance */
volatile uint32_t EventFlag = 0 ; /* Event flag set by the button callback and polled in main loop */
pot_obj_t *pPot;                  /* pointer referencing the potentiometer object instance */
uint32_t PotLevel;                /* potentiometer level in percent */

/* Private functions prototype -----------------------------------------------*/
static void ButtonCb(button_t *pbutton, void *arg);

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

  /* Retrieve and store the BUTTON object pointer */
  pButton = mx_button_0_getobject();

  /* Retrieve and store the potentiometer object pointer */
  pPot = mx_potentiometer_0_getobject();

  /* Parts initialization */
  if (button_init(pButton, BUTTON_0) != BUTTON_OK)
  {
    return (-1);
  }

  if (pot_init(pPot, POTENTIOMETER_0) != POT_OK)
  {
    return (-1);
  }
  /* Register a callback for BUTTON_EVENT_PRESSED event */
  if (button_register_callback(pButton, ButtonCb, BUTTON_EVENT_PRESSED, NULL) != BUTTON_OK)
  {
    return (-1);
  }
  /* Enable button interrupt/event generation */
  if (button_enableit(pButton) != BUTTON_OK)
  {
    return (-1);
  }
  printf("B-G564E-MC1S Parts initialization COMPLETED.\n");

  /* Turn on the LED (initial status) */
  led_on(MX_STATUS_LED);
  printf("LED is turned on.\n");
  printf("Press the user button to toggle the LED and read the potentiometer level.\n");
  while (1)
  {
    /* Toggle the LED each time a button pressed event is detected */
    if (EventFlag == 1)
    {
      /* Clear the event flag to detect future button events */
      EventFlag = 0;
      led_toggle(MX_STATUS_LED);
      printf("LED toggled.\n");

      /* Read the potentiometer level */
      if (pot_get_level(pPot, &PotLevel) == POT_OK)
      {
        printf("Read potentiometer level COMPLETED.\n");
        printf("Potentiometer Level = %"PRIu32 "\n", PotLevel);
      }
    }
  }
} /* end main */

/**
  * brief: Button event callback.
  * param: pbutton Button object pointer
  * param: arg     Generic argument, unused in this template
  */
static void ButtonCb(button_t *pbutton, void *arg)
{
  /* Notify the main loop that a button event occurred */
  EventFlag = 1;
}
