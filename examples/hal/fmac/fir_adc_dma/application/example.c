/**
  ******************************************************************************
  * file           : example.c
  * brief          : FMAC FIR filtering of ADC data streamed by DMA.
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
#include <string.h>   /* importing memset function */
#include "example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Number of ADC samples written to the FMAC X1 input buffer. */
#define ADC_INPUT_SAMPLE_COUNT          (COEFF_VECTOR_B_SIZE)

/* Number of filtered samples read from the FMAC Y output buffer.
   Total expected size: input array size - number of feed-forward taps */
#define FMAC_OUTPUT_SAMPLE_COUNT        (ADC_INPUT_SAMPLE_COUNT - COEFF_VECTOR_B_SIZE + 1U)

/* FMAC filter gain */
#define FILTER_GAIN                     (1U)

/* DMA alignment recommendation for STM32 devices */
#define DMA_ACCESS_ALIGNMENT            (8U)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Handles returned by project-generated initialization functions */
hal_fmac_handle_t *pFMAC; /* Pointer referencing the FMAC handle from the generated code */
hal_adc_handle_t  *pADC;  /* Pointer referencing the ADC handle from the generated code */
hal_tim_handle_t  *pTIM;  /* Pointer referencing the timer handle from the generated code */

uint32_t fmac_error_code = 0U; /* Stores the latest FMAC error code */
uint32_t adc_error_code  = 0U; /* Stores the latest ADC error code */

/* Points to the FMAC X1 input FIFO used by ADC DMA to write input samples */
const volatile uint32_t *pFmacWdata = NULL;

/* FIR feed-forward coefficients in Q1.15 format */
static int16_t Coefficients[COEFF_VECTOR_B_SIZE] =
{
  2212, 8848, 13272, 8848, 2212
};

/* Output samples written by FMAC Y DMA */
__attribute__((section("non_cacheable_area"), aligned(DMA_ACCESS_ALIGNMENT)))
static int16_t OutputSamples[FMAC_OUTPUT_SAMPLE_COUNT];

/* Completion and error flags updated from callbacks */
volatile uint8_t AdcTransferComplete; /* Set to 1 when ADC DMA transfer completes */
volatile uint8_t ReadYComplete;       /* Set to 1 when FMAC Y read transfer completes */
volatile uint8_t FmacError;           /* Set to 1 when an FMAC error occurs */
volatile uint8_t AdcError;            /* Set to 1 when an ADC error occurs */

/* Private function prototypes -----------------------------------------------*/
static void ReadYCompleteCallback(hal_fmac_handle_t *handle_fmac);
static void ErrorCallback(hal_fmac_handle_t *handle_fmac);
static void AdcTransfCallback(hal_adc_handle_t *handle_adc);
static void AdcErrorCallback(hal_adc_handle_t *handle_adc);

/* Private functions ---------------------------------------------------------*/

/** ########## Step 1 ##########
  * Configures FMAC, ADC, DMA and timer.
  * Registers the FMAC and ADC callbacks required by the example.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pFMAC = mx_example_fmac_init();

  if (pFMAC == NULL)
  {
    goto _app_init_exit;
  }

  pADC = mx_example_adc_init();

  if (pADC == NULL)
  {
    goto _app_init_exit;
  }

  pTIM = mx_example_tim_init();

  if (pTIM == NULL)
  {
    goto _app_init_exit;
  }

  /* Register callback executed when FMAC Y read completes */
  if (HAL_FMAC_RegisterReadYCpltCallback(pFMAC, ReadYCompleteCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Register callback executed in case of FMAC error */
  if (HAL_FMAC_RegisterErrorCallback(pFMAC, ErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Register the ADC callbacks */
  if (HAL_ADC_RegisterDataTransferCpltCallback(pADC, AdcTransfCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Register callback executed in case of ADC or ADC DMA error */
  if (HAL_ADC_RegisterErrorCallback(pADC, AdcErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;
  PRINTF("[INFO] Step 1: FMAC, ADC, DMA and timer initialization COMPLETED.\n");

  /** ########## Step 2 ##########
    *
    * Performs the ADC activation and self calibration to improve measurements accuracy.
    * Start the timer in polling mode
    * The calibration is a prerequisite to any ADC operation. It removes the systematic errors that
    * might vary from chip to chip.
  */
  if (HAL_ADC_Start(pADC) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_ADC_Calibrate(pADC) != HAL_OK)
  {
    goto _app_init_exit;
  }

  if (HAL_TIM_Start(pTIM) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 2: ADC start and calibration COMPLETED. TIM start COMPLETED.\n");
  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  FmacError      = 0U;
  AdcError       = 0U;
  ReadYComplete  = 0U;
  AdcTransferComplete = 0U;

  /* Clear previous FMAC output samples before starting a new DMA transfer */
  (void)memset(OutputSamples, 0, sizeof(OutputSamples));

  /** ########## Step 3 ##########
    * Preloads the FMAC X2 buffer with the FIR filter coefficients.
    */
  hal_fmac_preload_x2_t preloadX2_data;
  preloadX2_data.p_coeff_b = Coefficients;
  preloadX2_data.coeff_b_size_half_word = COEFF_VECTOR_B_SIZE;
  preloadX2_data.coeff_a_size_half_word = 0U; /* No feedback coefficients for FIR */

  if (HAL_FMAC_PreloadX2(pFMAC, &preloadX2_data) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3: FMAC X2 buffer preloaded with FIR filter coefficients.\n");

  /** ########## Step 4 ##########
    * Gets the FMAC X1 input register address and locks the X1 write access
    * so the ADC DMA can write directly into the FMAC input buffer.
    */
  /* Get FMAC X1 buffer address so that an external peripheral can write to it */
  pFmacWdata = HAL_FMAC_GetX1Address(pFMAC);

  if (pFmacWdata == NULL)
  {
    goto _app_process_exit;
  }

  /* Lock X1 write access for an external peripheral such as ADC DMA */
  if (HAL_FMAC_LockX1WriteAccessForExternPeriph(pFMAC) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 4: FMAC X1 input address retrieved and X1 write access locked for ADC DMA.\n");

  /** ########## Step 5 ##########
    * Starts DMA transfer from FMAC Y output buffer to memory.
    */
  if (HAL_FMAC_ReadY_DMA(pFMAC, OutputSamples, FMAC_OUTPUT_SAMPLE_COUNT) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 5: FMAC Y DMA transfer configured toward the output buffer.\n");

  /** ########## Step 6 ##########
    * Starts FMAC FIR filter.
    */
  if (HAL_FMAC_StartFilterFIR(pFMAC, COEFF_VECTOR_B_SIZE, FILTER_GAIN,
                              HAL_FMAC_OPT_IT_FILTER_SATURATION_ERROR) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 6: FMAC FIR filter started.\n");

  /** ########## Step 7 ##########
    * Starts ADC DMA toward FMAC X1 and waits until FMAC FIR filtering completes.
    * The filtered output samples are stored in OutputSamples.
    */

  /* Start ADC DMA transfer directly to FMAC X1 buffer */
  if (HAL_ADC_REG_StartConv_DMA(pADC, (uint8_t *)pFmacWdata, ADC_INPUT_SAMPLE_COUNT * sizeof(uint16_t)) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Wait for ADC DMA completion or FMAC/ADC error. */
  while ((AdcTransferComplete != 1U)
         && (FmacError != 1U)
         && (AdcError != 1U))
  {
    __WFI();
  }

  if (FmacError != 0U)
  {
    fmac_error_code = HAL_FMAC_GetLastErrorCodes(pFMAC);
    goto _app_process_exit;
  }

  if (AdcError != 0U)
  {
    adc_error_code = HAL_ADC_GetLastErrorCodes(pADC);
    goto _app_process_exit;
  }

  /* Wait for FMAC output DMA completion or FMAC/ADC error. */
  while ((ReadYComplete != 1U)
         && (FmacError != 1U)
         && (AdcError != 1U))
  {
    __WFI();
  }

  if (FmacError != 0U)
  {
    fmac_error_code = HAL_FMAC_GetLastErrorCodes(pFMAC);
    goto _app_process_exit;
  }

  if (AdcError != 0U)
  {
    adc_error_code = HAL_ADC_GetLastErrorCodes(pADC);
    goto _app_process_exit;
  }

  if (ReadYComplete != 1U)
  {
    PRINTF("[ERROR] FMAC Y output DMA transfer did not complete.\n");
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 7: ADC DMA started toward FMAC X1 while TIM trigger paces conversions. "
         "FMAC FIR filtering completed and output samples are available in memory or OutputSamples.\n");

  /* Stop the filter once processing is complete */
  if (HAL_FMAC_StopFilter(pFMAC) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Release external write access to X1 */
  if (HAL_FMAC_UnlockX1WriteAccessForExternPeriph(pFMAC) != HAL_OK)
  {
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 8 ##########
  * Deinitializes FMAC, ADC and timer peripherals.
  */
app_status_t app_deinit(void)
{
  mx_example_fmac_deinit();
  mx_example_adc_deinit();
  mx_example_tim_deinit();

  PRINTF("[INFO] Step 8: Peripherals de-initialized: FMAC, ADC and timer.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** FMAC Y read complete callback.
  * @user This implementation of the FMAC callback can be customized.
  * This function is executed when the FMAC Y read DMA transfer completes.
  */
static void ReadYCompleteCallback(hal_fmac_handle_t *handle_fmac)
{
  ReadYComplete = 1U;
}

/** FMAC error callback.
  * @user This implementation of the FMAC callback can be customized.
  * This function is executed if an FMAC error interrupt is generated.
  */
static void ErrorCallback(hal_fmac_handle_t *handle_fmac)
{
  FmacError = 1U;
}

/** ADC DMA transfer completion callback.
  * @user This implementation of the ADC callback can be customized.
  * This function is executed when the ADC DMA transfer completes.
  */
static void AdcTransfCallback(hal_adc_handle_t *handle_adc)
{
  AdcTransferComplete = 1U;
}

/** ADC error callback.
  * @user This implementation of the ADC callback can be customized.
  * This function is executed if an ADC or DMA error occurs.
  */
static void AdcErrorCallback(hal_adc_handle_t *handle_adc)
{
  AdcError = 1U;
}
