/**
  ******************************************************************************
  * file           : main.c
  * brief          : Main program body
  *                  Calls target system initialization then loop in main.
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
/* Private functions prototype -----------------------------------------------*/
static void error_handler(void);

/**
  * brief:  The application entry point.
  * retval: none but we specify int to comply with the C99 standard
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
    error_handler();
  }
  else
  {
    /* Add your secure application code here */

    /* Jump to non-secure application */
    mx_start_application(NSECURE_APPLICATION_ADDR);

    /* This point must not be reached */
    error_handler();
  }

  return 0;
} /* end main */

/** brief:  Error notification
  * retval: None (infinite loop)
  */
static void error_handler(void)
{
  while (1);
} /* end error_handler */

/**
  * brief:  This function handles IAC exception.
  * param:  periph
  */
void HAL_RIF_IAC_Callback(hal_rif_iac_periph_t periph)
{
  (void)periph;

  /* Unrecoverable error: infinite loop */
  while (1);
}

/** ----------------------------------------------------------
  * Specific ISR definitions
  * ----------------------------------------------------------
  */

/** Redefines the NMI handler from the startup file.
  * brief:  Non Maskable Interrupt Handler
  * retval: None (infinite loop)
  *
  * The default handler is redefined here so that:
  * You can easily set a breakpoint to investigate the issue.
  */
void NMI_Handler(void)
{
  /* Unrecoverable error: infinite loop */
  while (1);
}

/** Redefines the HardFault handler from the startup file.
  * brief:  Hard Fault Handler
  * retval: None (infinite loop)
  *
  * The default handler is redefined here so that:
  * You can easily set a breakpoint to investigate the issue.
  */
void HardFault_Handler(void)
{
  /* Unrecoverable error: infinite loop */
  while (1);
}

/** Redefines the SecureFault handler from the startup file.
  * brief:  Secure Fault Handler
  * retval: None (infinite loop)
  *
  * The default handler is redefined here so that:
  * You can easily set a breakpoint to investigate the issue.
  */
void SecureFault_Handler(void)
{
  /* Unrecoverable error: infinite loop */
  while (1);
}

/** Redefines the MemManage handler from the startup file.
  * brief:  MemManage Fault Handler
  * retval: None (infinite loop)
  *
  * The default handler is redefined here so that:
  * You can easily set a breakpoint to investigate the issue.
  */
void MemManage_Handler(void)
{
  /* Unrecoverable error: infinite loop */
  while (1);
}

/** Redefines the BusFault handler from the startup file.
  * brief:  Bus Fault Handler
  * retval: None (infinite loop)
  *
  * The default handler is redefined here so that:
  * You can easily set a breakpoint to investigate the issue.
  */
void BusFault_Handler(void)
{
  /* Unrecoverable error: infinite loop */
  while (1);
}

/** Redefines the UsageFault handler from the startup file.
  * brief:  Usage Fault Handler
  * retval: None (infinite loop)
  *
  * The default handler is redefined here so that:
  * You can easily set a breakpoint to investigate the issue.
  */
void UsageFault_Handler(void)
{
  /* Unrecoverable error: infinite loop */
  while (1);
}
