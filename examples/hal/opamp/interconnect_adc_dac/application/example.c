/**
  ******************************************************************************
  * file           : example.c
  * brief          : OPAMP configured in follower mode, with an internally connected DAC
  *                  and an ADC connected through a shared GPIO pin, using the HAL.
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

/* Assumption: pin Vref+ connected to Vdd at board level */
#define VREFPLUS_APPLI                   (VDD_VALUE)

/* DAC output voltage set on the OPAMP non-inverting input (unit: mV) */
#define DAC_OUTPUT_MILLIVOLT              (VREFPLUS_APPLI / 2U)

/* ADC conversion time out value in milliseconds */
#define ADC_POLL_TIMEOUT_MS               10U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_opamp_handle_t *pOPAMP; /* pointer referencing the OPAMP handle from the generated code */
hal_dac_handle_t    *pDAC;  /* pointer referencing the DAC handle from the generated code */
hal_adc_handle_t    *pADC;  /* pointer referencing the ADC handle from the generated code */

/* Raw DAC value set on the OPAMP non-inverting input (12-bit resolution) */
uint16_t DacRawData = 0U;

/* Raw ADC value acquired on the OPAMP output (12-bit resolution) */
uint16_t AdcRawData = 0U;

/* Private functions prototype -----------------------------------------------*/

/** Configures the DAC, the OPAMP (follower) and the ADC, following the scenario order.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * Configures the DAC to continuously generate a signal on the OPAMP non-inverting input.
    */
  pDAC = mx_example_dac_init();
  if (pDAC == NULL)
  {
    goto _app_init_exit;
  }

  /* The calibration allows a better output voltage precision */
  if (HAL_DAC_CalibrateChannelBuffer(pDAC, MX_DAC_CHANNEL) != HAL_OK)
  {
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 1: DAC configuration COMPLETED.\n");

  /** ########## Step 2 ##########
    * Configures the OPAMP in follower mode, with its non-inverting input internally connected to the DAC
    * and its output connected to the ADC through a shared GPIO pin.
    */
  pOPAMP = mx_example_opamp_init();
  if (pOPAMP == NULL)
  {
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 2: OPAMP configuration COMPLETED.\n");

  /** ########## Step 3 ##########
    * Configures the ADC to acquire the OPAMP output.
    */
  pADC = mx_example_adc_init();
  if (pADC == NULL)
  {
    goto _app_init_exit;
  }

  /* The ADC calibration is a prerequisite to any ADC operation.
     It removes the systematic errors that might vary from chip to chip. */
  if (HAL_ADC_Start(pADC) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_ADC_Calibrate(pADC) != HAL_OK)
  {
    goto _app_init_exit;
  }
  PRINTF("[INFO] Step 3: ADC configuration COMPLETED.\n");

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 4 ##########
  * Starts the DAC, the OPAMP, and the ADC. The DAC updates its output and the ADC samples the resulting
  * OPAMP output, using a software-triggered conversion in polling mode.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  DacRawData = HAL_DAC_CALC_VOLTAGE_TO_DATA(VREFPLUS_APPLI, DAC_OUTPUT_MILLIVOLT, HAL_DAC_DATA_ALIGN_12_BITS_RIGHT);
  if (HAL_DAC_SetChannelData(pDAC, MX_DAC_CHANNEL, DacRawData) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_DAC_StartChannel(pDAC, MX_DAC_CHANNEL) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_OPAMP_Start(pOPAMP) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_ADC_REG_StartConv(pADC) != HAL_OK)
  {
    goto _app_process_exit;
  }
  if (HAL_ADC_REG_PollForConv(pADC, ADC_POLL_TIMEOUT_MS) != HAL_OK)
  {
    PRINTF("[ERROR] Step 4: ADC conversion ERROR.\n");
    goto _app_process_exit;
  }
  AdcRawData = HAL_ADC_REG_ReadConversionData(pADC);

  PRINTF("[INFO] Step 4: DAC, OPAMP and ADC started. DacRawData = %" PRIu16 "  AdcRawData = %" PRIu16 "\n",
         DacRawData, AdcRawData);
  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** Deinitializes the OPAMP, DAC and ADC instances before leaving the scenario.
  * This function is never called and is provided here as reference.
  */
app_status_t app_deinit(void)
{
  mx_example_opamp_deinit();
  mx_example_dac_deinit();
  mx_example_adc_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */
