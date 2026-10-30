/**
  ******************************************************************************
  * file           : example.c
  * brief          : FMAC IIR filtering data buffer from interrupt to DAC through DMA.
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
#include <arm_math.h> /* Required for conversion function arm_float_to_q15() and PI definition */
#include "example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Size of the input data arrays written during calculation */
#define INPUT_SAMPLES_SIZE     512U

/* Size of the output data arrays read during calculation.
  For FMAC IIR filtering, the first output sample is calculated after COEFF_VECTOR_B_SIZE input samples. */
#define OUTPUT_SAMPLES_SIZE    (INPUT_SAMPLES_SIZE - COEFF_VECTOR_B_SIZE + 1U)

#define COEFF_VECTOR_SIZE      (COEFF_VECTOR_B_SIZE + COEFF_VECTOR_A_SIZE)

#define FILTER_GAIN  1

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_fmac_handle_t *pFMAC; /* pointer referencing the FMAC handle from the generated code */
hal_dac_handle_t  *pDAC;  /* pointer referencing the DAC handle from the generated code */
hal_tim_handle_t  *pTIM;  /* pointer referencing the timer handle from the generated code */

uint32_t fmac_error_code = 0U;              /* Stores the latest FMAC error code */
uint32_t dac_error_code  = 0U;              /* Stores the latest DAC error code */

/* DMA nodes must be 8-byte aligned. */
__attribute__((section("non_cacheable_area"), aligned(8)))
const volatile uint32_t *Fmac_Rdata = NULL;  /* Points to the FMAC Y-output FIFO used to read filtered samples */

/* Array of filter coefficients in Q1.15 format: feed-forward B coefficients followed by feedback A coefficients. */
static int16_t Coefficients[COEFF_VECTOR_SIZE] =
{
  828, 1170, 2360, 2293, 2360, 1170, 828,
  10432, -17296, 17696, -12512, 5536, -1296
};

/* Array of input real values between -1 and 1 */
static const float32_t InputSamplesFloat[INPUT_SAMPLES_SIZE] =
{
  0.0, 0.16101074, -0.04724121, 0.42248535, 0.00021362305, 0.535614, 0.17706299, 0.49261475,
  0.43328857, 0.3664856, 0.6599121, 0.26483154, 0.75, 0.26464844, 0.6595154, 0.36590576,
  0.43249512, 0.4916687, 0.17593384, 0.5343628, -0.0011291504, 0.42105103, -0.04876709, 0.15945435,
  -0.0015563965, -0.16256714, 0.045715332, -0.4239502, -0.0015869141, -0.53686523, -0.17816162, -0.4935913,
  -0.4340515, -0.36709595, -0.66030884, -0.26504517, -0.75, -0.2644348, -0.65908813, -0.3652954,
  -0.43170166, -0.49072266, -0.1748352, -0.5331116, 0.0025024414, -0.41958618, 0.05026245, -0.15789795,
  0.0031433105, 0.16412354, -0.04421997, 0.42538452, 0.0029296875, 0.53808594, 0.17926025, 0.49453735,
  0.43484497, 0.36767578, 0.66070557, 0.26522827, 0.7499695, 0.2642212, 0.6586609, 0.36468506,
  0.4309082, 0.4897461, 0.17370605, 0.53186035, -0.0038757324, 0.41812134, -0.05178833, 0.15631104,
  -0.004699707, -0.16567993, 0.04269409, -0.42684937, -0.0043029785, -0.53933716, -0.18035889, -0.4954834,
  -0.4356079, -0.36828613, -0.6611023, -0.2654419, -0.7499695, -0.26400757, -0.65826416, -0.3640747,
  -0.43011475, -0.48876953, -0.1725769, -0.53060913, 0.005218506, -0.416687, 0.05331421, -0.15475464,
  0.006286621, 0.16723633, -0.041168213, 0.4282837, 0.005645752, 0.54055786, 0.18145752, 0.49642944,
  0.43637085, 0.36886597, 0.661499, 0.265625, 0.74993896, 0.26376343, 0.6578369, 0.36343384,
  0.42929077, 0.48779297, 0.17144775, 0.5293579, -0.006591797, 0.41522217, -0.054840088, 0.15319824,
  -0.007843018, -0.1687622, 0.03967285, -0.42971802, -0.0069885254, -0.5418091, -0.18255615, -0.49734497,
  -0.4371338, -0.3694458, -0.66186523, -0.2658081, -0.74993896, -0.2635498, -0.65740967, -0.3628235,
  -0.4284973, -0.4868164, -0.1703186, -0.5280762, 0.007965088, -0.41375732, 0.056365967, -0.15164185,
  0.009429932, 0.1703186, -0.038146973, 0.43115234, 0.008331299, 0.5430298, 0.18365479, 0.49829102,
  0.43789673, 0.36999512, 0.66226196, 0.2659607, 0.74990845, 0.26330566, 0.6569519, 0.36218262,
  0.42767334, 0.48583984, 0.16918945, 0.52682495, -0.009338379, 0.41229248, -0.057891846, 0.15008545,
  -0.010986328, -0.171875, 0.03665161, -0.4326172, -0.009674072, -0.5442505, -0.1847229, -0.49920654,
  -0.43865967, -0.37057495, -0.6626282, -0.2661438, -0.7498474, -0.26306152, -0.65652466, -0.36154175,
  -0.42687988, -0.48486328, -0.1680603, -0.5255432, 0.0107421875, -0.41082764, 0.059417725, -0.14852905,
  0.012573242, 0.1734314, -0.03515625, 0.4340515, 0.011016846, 0.5454712, 0.18582153, 0.5001526,
  0.4394226, 0.3711548, 0.6629944, 0.2662964, 0.7498169, 0.26281738, 0.6560669, 0.36090088,
  0.4260559, 0.48388672, 0.16693115, 0.524292, -0.0121154785, 0.4093628, -0.060943604, 0.14697266,
  -0.014129639, -0.1749878, 0.03363037, -0.43548584, -0.012359619, -0.5466919, -0.18688965, -0.5010681,
  -0.44015503, -0.3717041, -0.6633606, -0.2664795, -0.7497864, -0.26257324, -0.65563965, -0.36026,
  -0.42523193, -0.48287964, -0.16577148, -0.52301025, 0.0134887695, -0.40789795, 0.062469482, -0.14538574,
  0.015716553, 0.17654419, -0.03213501, 0.43692017, 0.013702393, 0.5479126, 0.18798828, 0.50198364,
  0.44088745, 0.37225342, 0.6637268, 0.26663208, 0.74972534, 0.2623291, 0.6551819, 0.35961914,
  0.42440796, 0.48187256, 0.16464233, 0.5217285, -0.014892578, 0.4064331, -0.06399536, 0.14382935,
  -0.01727295, -0.17810059, 0.030639648, -0.43832397, -0.015045166, -0.5491333, -0.1890564, -0.50289917,
  -0.4416504, -0.37280273, -0.6640625, -0.26678467, -0.7496643, -0.26205444, -0.6547241, -0.35897827,
  -0.42355347, -0.480896, -0.16348267, -0.5204468, 0.01626587, -0.40493774, 0.06552124, -0.14227295,
  0.018859863, 0.17962646, -0.029144287, 0.4397583, 0.016357422, 0.5503235, 0.19012451, 0.5038147,
  0.4423828, 0.37335205, 0.6644287, 0.26693726, 0.7496033, 0.2618103, 0.65423584, 0.35830688,
  0.4227295, 0.47988892, 0.162323, 0.51916504, -0.017669678, 0.4034729, -0.06707764, 0.14071655,
  -0.02041626, -0.18118286, 0.027618408, -0.44119263, -0.017700195, -0.5515442, -0.19119263, -0.5046997,
  -0.44311523, -0.37390137, -0.6647644, -0.26705933, -0.74954224, -0.26153564, -0.6537781, -0.35766602,
  -0.421875, -0.47888184, -0.16119385, -0.5178833, 0.019042969, -0.40200806, 0.068603516, -0.13912964,
  0.021972656, 0.18273926, -0.026123047, 0.44262695, 0.019012451, 0.5527344, 0.19223022, 0.50561523,
  0.44381714, 0.37445068, 0.6651001, 0.2672119, 0.7494812, 0.261261, 0.6533203, 0.35699463,
  0.42105103, 0.47787476, 0.16003418, 0.51660156, -0.020446777, 0.4005127, -0.070129395, 0.13757324,
  -0.02355957, -0.18426514, 0.024627686, -0.44403076, -0.020355225, -0.5539551, -0.19329834, -0.50650024,
  -0.44454956, -0.37496948, -0.6654358, -0.26733398, -0.74942017, -0.26098633, -0.65283203, -0.35632324,
  -0.42019653, -0.47683716, -0.158844, -0.5153198, 0.021850586, -0.39904785, 0.07165527, -0.13601685,
  0.025115967, 0.18582153, -0.023132324, 0.4454651, 0.02166748, 0.55514526, 0.19436646, 0.50738525,
  0.44525146, 0.37548828, 0.6657715, 0.26745605, 0.7493286, 0.26068115, 0.65234375, 0.35565186,
  0.41934204, 0.47583008, 0.15768433, 0.51400757, -0.023223877, 0.3975525, -0.07321167, 0.13442993,
  -0.02670288, -0.18737793, 0.021636963, -0.4468689, -0.022979736, -0.55633545, -0.19540405, -0.5083008,
  -0.4459839, -0.37600708, -0.6661072, -0.26757812, -0.74923706, -0.2604065, -0.65185547, -0.35498047,
  -0.41848755, -0.474823, -0.15652466, -0.51272583, 0.024627686, -0.39608765, 0.07473755, -0.13287354,
  0.028259277, 0.18890381, -0.020141602, 0.44830322, 0.024291992, 0.55752563, 0.19644165, 0.5091858,
  0.4466858, 0.37652588, 0.66641235, 0.2677002, 0.7491455, 0.26010132, 0.6513672, 0.35427856,
  0.41760254, 0.4737854, 0.15533447, 0.5114136, -0.026031494, 0.3945923, -0.076293945, 0.13131714,
  -0.029815674, -0.1904602, 0.018676758, -0.44970703, -0.025604248, -0.5587158, -0.19750977, -0.5100403,
  -0.4473877, -0.37704468, -0.6667175, -0.26782227, -0.74905396, -0.25982666, -0.6508789, -0.35360718,
  -0.41674805, -0.4727478, -0.1541748, -0.5101013, 0.027435303, -0.39312744, 0.077819824, -0.12973022,
  0.031402588, 0.19198608, -0.017181396, 0.45111084, 0.026916504, 0.559906, 0.19854736, 0.5109253,
  0.4480896, 0.37756348, 0.6670532, 0.26791382, 0.7489624, 0.25952148, 0.6503601, 0.35290527,
  0.41589355, 0.4717102, 0.15298462, 0.5088196, -0.028869629, 0.39163208, -0.0793457, 0.12817383,
  -0.032958984, -0.19354248, 0.015686035, -0.45251465, -0.02822876, -0.5610962, -0.19958496, -0.5118103
};

/* Input samples converted to Q1.15 fixed-point format for FMAC processing. */
static q15_t InputSamplesQ15[INPUT_SAMPLES_SIZE];

volatile uint8_t WriteX1Complete;     /* Set to 1 if the write operation is correctly completed */
volatile uint8_t FmacError;           /* Set to 1 if an FMAC error occurred */
volatile uint8_t DacTransferComplete; /* Set to 1 when DAC DMA transfer completes */
volatile uint8_t DacError;            /* Set to 1 when DAC signals an error */

/* Private functions prototype -----------------------------------------------*/
static void WriteX1CompleteCallback(hal_fmac_handle_t *handle_fmac);
static void ErrorCallback(hal_fmac_handle_t *handle_fmac);

/* DAC callbacks registered by the application */
static void DacConvCpltCallback(hal_dac_handle_t *hdac, hal_dac_channel_t channel);
static void DacErrorCallback(hal_dac_handle_t *hdac);

/** ########## Step 1 ##########
  * FMAC, DAC and timer initialization.
  * Registers the FMAC and DAC callbacks required by the example.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pFMAC = mx_example_fmac_init();

  if (pFMAC == NULL)
  {
    goto _app_init_exit;
  }

  pDAC  = mx_example_dac_init();

  if (pDAC == NULL)
  {
    goto _app_init_exit;
  }

  pTIM  = mx_example_tim_init();

  if (pTIM == NULL)
  {
    goto _app_init_exit;
  }

  /* Select Callback function called after WriteX1 complete */
  if (HAL_FMAC_RegisterWriteX1CpltCallback(pFMAC, WriteX1CompleteCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Select Callback function called after Error interrupt */
  if (HAL_FMAC_RegisterErrorCallback(pFMAC, ErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Select Callback function used to know when DMA transfer is done */
  if (HAL_DAC_RegisterConvCpltCallback(pDAC, DacConvCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Select Callback function used to detect DAC/DMA failures */
  if (HAL_DAC_RegisterErrorCallback(pDAC, DacErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;
  PRINTF("[INFO] Step 1: FMAC, DAC and timer initialization COMPLETED.\n");

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  FmacError = 0U;
  WriteX1Complete = 0U;
  DacTransferComplete = 0U;
  DacError = 0U;

  /** ########## Step 2 ##########
    * Preloads X2 with filter coefficients and writes X1 input samples in interrupt mode.
    */
  hal_fmac_preload_x2_t preloadX2_data;
  preloadX2_data.p_coeff_b = Coefficients;
  preloadX2_data.p_coeff_a = &Coefficients[COEFF_VECTOR_B_SIZE];
  preloadX2_data.coeff_b_size_half_word = COEFF_VECTOR_B_SIZE;
  preloadX2_data.coeff_a_size_half_word = COEFF_VECTOR_A_SIZE;

  if (HAL_FMAC_PreloadX2(pFMAC, &preloadX2_data) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Convert the floating-point input samples to Q1.15 fixed-point format for FMAC processing. */
  arm_float_to_q15(InputSamplesFloat, InputSamplesQ15, INPUT_SAMPLES_SIZE);

  /* Write X1 input buffer in interrupt mode */
  if (HAL_FMAC_WriteX1_IT(pFMAC, InputSamplesQ15, INPUT_SAMPLES_SIZE) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2: FMAC inputs configured.\n");

  /** ########## Step 3 ##########
    * Gets the FMAC Y output register address.
    * Locks Y read access for the DAC DMA transfer.
    */
  Fmac_Rdata = HAL_FMAC_GetYAddress(pFMAC);

  /* Ensure the FMAC Y-output FIFO address is valid before starting DMA requests from it. */
  if (Fmac_Rdata == NULL)
  {
    goto _app_process_exit;
  }

  if (HAL_FMAC_LockYReadAccessForExternPeriph(pFMAC) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 3: FMAC Y output address retrieved and Y read access is locked for DAC DMA.\n");

  /** ########## Step 4 ##########
    * Starts DAC DMA from the FMAC Y-output FIFO address and uses TIM to pace DAC conversions.
    */
  if (HAL_DAC_StartChannel_DMA(pDAC, MX_DAC_CHANNEL, (void const *)Fmac_Rdata,
                               OUTPUT_SAMPLES_SIZE * sizeof(uint16_t)) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 4: DAC DMA started from FMAC output while TIM trigger paces conversions.\n");

  /** ########## Step 5 ##########
    * Starts FMAC IIR filter, then starts the timer to pace DAC conversions.
    * The timer is started AFTER the filter to ensure FMAC Y output is populated
    * before the first DAC DMA request. Starting the timer before the filter would
    * trigger DAC DMA reads from an empty Y buffer, causing an FMAC underflow error.
    */
  if (HAL_FMAC_StartFilterIIR(pFMAC, COEFF_VECTOR_B_SIZE, COEFF_VECTOR_A_SIZE, FILTER_GAIN,
                              HAL_FMAC_OPT_IT_FILTER_NONE) != HAL_OK)
  {
    goto _app_process_exit;
  }

  if (HAL_TIM_Start(pTIM) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 5: FMAC IIR filter started.\n");

  while ((DacTransferComplete == 0U)
         && (FmacError == 0U)
         && (DacError == 0U))
  {
    __WFI();
  }

  if (FmacError != 0U)
  {
    fmac_error_code = HAL_FMAC_GetLastErrorCodes(pFMAC);
    PRINTF("[ERROR] FMAC error, HAL_FMAC_GetLastErrorCodes = %" PRIu32 ".\n", fmac_error_code);
    goto _app_process_exit;
  }

  if (DacError != 0U)
  {
    dac_error_code = HAL_DAC_GetLastErrorCodes(pDAC);
    PRINTF("[ERROR] DAC error, HAL_DAC_GetLastErrorCodes = %" PRIu32 ".\n", dac_error_code);
    goto _app_process_exit;
  }

  if (DacTransferComplete == 0U)
  {
    PRINTF("[ERROR] DAC transfer did not complete.\n");
    goto _app_process_exit;
  }

  /* Stop the filter once processing is complete */
  if (HAL_FMAC_StopFilter(pFMAC) != HAL_OK)
  {
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
}


/** ########## Step 6 ##########
  * Deinitializes FMAC, DAC and timer peripherals.
  */
app_status_t app_deinit(void)
{
  mx_example_fmac_deinit();
  mx_example_dac_deinit();
  mx_example_tim_deinit();

  PRINTF("[INFO] Step 6: Peripherals de-initialized: FMAC, DAC and timer.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** FMAC write X1 complete callback
  * @user This implementation of the FMAC callback can be customized.
  * This function is executed if the write X1 complete interrupt is generated.
  */
static void WriteX1CompleteCallback(hal_fmac_handle_t *handle_fmac)
{
  /* async processing related to step 3 */
  WriteX1Complete = 1U;
}

/** FMAC error callback
  * @user This implementation of the FMAC callback can be customized.
  * This function is executed if error interrupt is generated.
  */
static void ErrorCallback(hal_fmac_handle_t *handle_fmac)
{
  /* async processing related to step 3 */
  FmacError = 1U;
}

/** DAC conversion complete callback
  * @user This implementation of the DAC callback can be customized.
  * This function is executed when the DAC DMA transfer completes.
  */
void DacConvCpltCallback(hal_dac_handle_t *hdac, hal_dac_channel_t channel)
{
  /* Stop the DAC trigger timer once the DMA transfer is complete. */
  HAL_TIM_Stop(pTIM);
  /* Disable the FMAC underflow interrupt before the circular DMA can restart and read from the
   * now-empty Y FIFO. Without this, the circular DMA would trigger an FMAC underflow error
   * even though the DAC transfer completed successfully. */
  HAL_FMAC_UnlockYReadAccessForExternPeriph(pFMAC);
  DacTransferComplete = 1U;
}

/** DAC error callback
  * @user This implementation of the DAC callback can be customized.
  * This function is executed if a DAC or DMA error occurs.
  */
void DacErrorCallback(hal_dac_handle_t *hdac)
{
  /* Stop the DAC trigger timer to prevent further conversions after an error. */
  HAL_TIM_Stop(pTIM);
  DacError = 1U;
}
