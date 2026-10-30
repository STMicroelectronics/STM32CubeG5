/**
  ******************************************************************************
  * file           : example.c
  * brief          : ADC single conversion triggered by software in DMA mode
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
#define VREFPLUS_APPLI    (VDD_VALUE)

/* @user: The maximum data bus width used by DMA in STM32 devices is 64 bits.
          Therefore, 8-byte alignment is the minimum recommended alignment for DMA buffers across STM32 devices. */
#define DMA_ACCESS_ALIGNMENT        (8U)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_adc_handle_t *pADC;  /* pointer referencing the ADC handle from the generated code */

/** Variable to store the converted raw data to be transferred through DMA.
  * - Non-cacheable memory for data cache consistency.
  * - Aligned for DMA constraints.
  * - Mandatory with data cache enabled, harmless otherwise: portable across STM32 series.
  * - Variable for the ADC conversion data (a count on the ADC scale).
  */
__attribute__((section("non_cacheable_area"), aligned(DMA_ACCESS_ALIGNMENT)))
uint16_t AdcRawData = 0xFFFFU;

/* Value of voltage calculated from ADC conversion data (unit: mV) */
uint16_t AdcMilliVolt = 0U;

volatile uint8_t AdcTransferComplete = 0U;    /* Set to 1 if the ADC transfer is correctly completed */
volatile uint8_t AdcError = 0U;               /* Set to 1 if an ADC error is detected */

/* Private functions prototype -----------------------------------------------*/
/* Functions allowing the user to configure dynamically the ADC callbacks instead of weak functions */
static void AdcTransfCallback(hal_adc_handle_t *pADC);
static void AdcErrorCallback(hal_adc_handle_t *pADC);

app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 1 ##########
    * The init of the ADC instance is triggered by the applicative code.
    * The user callbacks for ADC group regular data transfer complete and error are registered.
    */
  pADC = mx_example_adc_init();
  if (pADC == NULL)
  {
    goto _app_init_exit;
  }

  /* Register the ADC callbacks */
  if (HAL_ADC_RegisterDataTransferCpltCallback(pADC, AdcTransfCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }
  if (HAL_ADC_RegisterErrorCallback(pADC, AdcErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: ADC initialization COMPLETED.\n");

  /** ########## Step 2 ##########
    * Performs the ADC activation and self calibration to improve measurements accuracy.
    * Calibration removes systematic errors that vary from chip to chip and is performed before
    * starting the regular-group conversion.
    */
  if (HAL_ADC_Start(pADC) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_ADC_Calibrate(pADC) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 2: ADC start and calibration COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 3 ##########
    * Starts the ADC conversion in DMA mode. The conversion is triggered by software.
    */
  AdcTransferComplete = 0;
  if (HAL_ADC_REG_StartConv_DMA(pADC, (uint8_t *)&AdcRawData, sizeof(AdcRawData)) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3: ADC start conversion COMPLETED.\n");

  /** Awaiting the Step 4 ADC conversion and DMA transfer (or error): asynchronous ADC interrupt callback.
    * Print the results: the raw ADC value and the computed corresponding voltage.
    * For this example purpose, we poll for the callback flag
    * but thanks to DMA mode, application can do other things instead of polling.
    */
  while ((AdcTransferComplete != 1U) && (AdcError != 1U))
  {
    /** Put the CPU in Wait For Interrupt state. An ADC interrupt or a SysTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the DMA mode.
      *        It can be replaced by your own code.
      */
    __WFI();
  }

  if (AdcError == 1U)
  {
    PRINTF("[ERROR] Step 4: ADC Error detected \n");
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 4: ADC conversion DONE.  Raw ADC value = 0x%" PRIx16 "   Voltage = %" PRId16 " mV  \n", \
         AdcRawData, AdcMilliVolt);

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** Deinitializes the ADC instance.
  * In this example, app_deinit is never called and it is provided as a reference only.
  */
app_status_t app_deinit(void)
{
  mx_example_adc_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** ########## Step 4 ##########
  * Executed when the DMA transfer is completed.
  * Computes the corresponding voltage.
  * Transfer complete callback in non-blocking mode.
  * @user: the transfer complete callback can be customized according to the application needs.
  */
static void AdcTransfCallback(hal_adc_handle_t *hADC)
{
  /* Compute the ADC conversion raw data to physical value using the proper helper macro. */
  AdcMilliVolt = HAL_ADC_CALC_DATA_TO_VOLTAGE(VREFPLUS_APPLI, AdcRawData, HAL_ADC_RESOLUTION_12_BIT);

  /* Update the status variable of the ADC unitary transfer */
  AdcTransferComplete = 1U;
}


/** ADC error interruption callback
  */
static void AdcErrorCallback(hal_adc_handle_t *hADC)
{
  /* report the error (this terminates the execution loop) */
  AdcError = 1U;
}
