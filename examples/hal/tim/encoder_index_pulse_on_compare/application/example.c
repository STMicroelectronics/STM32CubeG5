/**
  ******************************************************************************
  * file           : example.c
  * brief          : TIM encoder interface with index reset and pulse on compare using the HAL API
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
/* Private macro -------------------------------------------------------------*/
#define DELAY   40U
/* Private variables ---------------------------------------------------------*/
/* Pointer referencing the TIM handle from the generated code used as encoder interface. */
hal_tim_handle_t *pTIM_EncoderInterface;
/* Pointer referencing the TIM handle from the generated code used to emulate a quadrature encoder. */
hal_tim_handle_t *pTIM_EncoderEmulator;
/* Pointer referencing the button object from the generated code used to emulate index pulse request. */
button_t *pUserButton;

/** Variable to check the button press detection via the button callback.
  * Declared as volatile because updated from an external event (interrupt).
  */
volatile uint8_t IndexEmulation = 0U;
volatile uint8_t IndexEmulation_trigger = 0U;
button_status_t ButtonStatus;

/* Private functions prototype -----------------------------------------------*/
static void PeriodElapsedCallback(hal_tim_handle_t *htim);
static void ButtonCallback(button_t *pbutton, void *arg);

/** ########## Step 1 ##########
  * Initializes the GPIO, TIM for emulation and TIM for encoder interface.
  * The init of the TIMs and the GPIO is triggered by the applicative code.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  hal_tim_pulse_generator_config_t pulse_generator_config =
  {
    .prescaler = HAL_TIM_PULSE_DIV128,
    .pulse_width = 0xFF
  };

  pTIM_EncoderEmulator = mx_example_tim_emulator_encoder_init();
  if (pTIM_EncoderEmulator == NULL)
  {
    goto _app_init_exit;
  }

  pTIM_EncoderInterface = mx_example_tim_encoder_interface_init();
  if (pTIM_EncoderInterface == NULL)
  {
    goto _app_init_exit;
  }

  pUserButton = mx_button_getobject();
  ButtonStatus = button_init(pUserButton, MX_BUTTON);

  if ((pUserButton == NULL) || (ButtonStatus == BUTTON_ERROR))
  {
    goto _app_init_exit;
  }

  button_register_callback(pUserButton, ButtonCallback, BUTTON_EVENT_PRESSED, NULL);
  button_enableit(pUserButton);

  /* Register the period elapsed (update) callback on the encoder emulator timer. */
  if (HAL_TIM_RegisterUpdateCallback(pTIM_EncoderEmulator, PeriodElapsedCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  /** ########## Step 2 ##########
    * Starts both timers once, then keeps them running.
    */
  if (HAL_TIM_OC_StartChannel(pTIM_EncoderEmulator, MX_TIM_CHANNEL_ENCODER_EMULATOR_1) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_TIM_OC_StartChannel(pTIM_EncoderEmulator, MX_TIM_CHANNEL_ENCODER_EMULATOR_2) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_TIM_Start_IT(pTIM_EncoderEmulator) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_TIM_IC_StartChannel(pTIM_EncoderInterface, MX_TIM_CHANNEL_ENCODER_INTERFACE_1) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_TIM_IC_StartChannel(pTIM_EncoderInterface, MX_TIM_CHANNEL_ENCODER_INTERFACE_2) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_TIM_OC_SetPulseGenerator(pTIM_EncoderInterface, &pulse_generator_config) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_TIM_OC_StartChannel(pTIM_EncoderInterface, HAL_TIM_CHANNEL_3) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_TIM_Start(pTIM_EncoderInterface) != HAL_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 3 ##########
    * Pressing the user button toggles the index emulation.
    * While the index emulation is inactive, the encoder interface counts up
    * freely and reaches the compare value, so a pulse is generated on compare
    * and can be observed on the pulse-on-compare output pin.
    * When the index emulation is active, generate an index pulse on
    * MX_EXAMPLE_TIM_INDEX_PIN (wired to the encoder interface ETR input) on each
    * emulator update event. This resets the encoder counter before it reaches the
    * compare value, so the pulse on compare is suppressed.
    */
  if (IndexEmulation_trigger == 1U)
  {
    /* Generate an additional software pulse on MX_EXAMPLE_TIM_INDEX_PIN when button-triggered. */
    HAL_GPIO_WritePin(MX_EXAMPLE_TIM_INDEX_PORT, MX_EXAMPLE_TIM_INDEX_PIN, MX_EXAMPLE_TIM_INDEX_ACTIVE_STATE);
    HAL_Delay(DELAY);
    HAL_GPIO_WritePin(MX_EXAMPLE_TIM_INDEX_PORT, MX_EXAMPLE_TIM_INDEX_PIN, MX_EXAMPLE_TIM_INDEX_INACTIVE_STATE);

    IndexEmulation_trigger = 0U;
  }

  return_status = EXEC_STATUS_OK;

  return return_status;
} /* end app_process */


/** Deinitializes the GPIO and TIM peripherals before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  app_status_t return_status = EXEC_STATUS_OK;

  button_disableit(pUserButton);
  mx_gpio_button_deinit();
  mx_example_tim_encoder_interface_deinit();
  mx_example_tim_emulator_encoder_deinit();

  return return_status;
} /* end app_deinit */


/** Period elapsed callback in non-blocking mode
  */
static void PeriodElapsedCallback(hal_tim_handle_t *htim)
{
  /* Prevent unused argument(s) compilation warning */
  STM32_UNUSED(htim);
  if (IndexEmulation == 1U)
  {
    /* Trig index emulation  */
    IndexEmulation_trigger = 1U;
  }

}

/**  Defines the user callback function.
  *  This function is triggered when the button press event is detected.
  */
static void ButtonCallback(button_t *pbutton, void *arg)
{
  /* Prevent unused argument(s) compilation warning */
  STM32_UNUSED(pbutton);
  STM32_UNUSED(arg);
  if (IndexEmulation == 0U)
  {
    IndexEmulation = 1U;
  }
  else
  {
    IndexEmulation = 0U;
  }
}
