/**
  ******************************************************************************
  * file           : example.c
  * brief          : FMAC FIR filtering data buffer from interrupt to DMA.
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
#include <string.h>   /* importing memcmp and memset functions */
#include "example.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Size of the input data arrays written during calculation */
#define INPUT_SAMPLES_SIZE     512U

/* Size of the output data arrays read during calculation
   Total expected size: input array size - number of feed-forward taps */
#define OUTPUT_SAMPLES_SIZE     INPUT_SAMPLES_SIZE - COEFF_VECTOR_B_SIZE + 1

#define FILTER_GAIN  1

/* @user: The maximum data bus width used by DMA in STM32 devices is 64 bits.
          Therefore, 8-byte alignment is the minimum recommended alignment for DMA buffers across STM32 devices. */
#define DMA_ACCESS_ALIGNMENT        (8U)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
hal_fmac_handle_t *pFMAC;  /* pointer referencing the FMAC handle from the generated code */

uint32_t fmac_error_code;  /* Memorizes the FMAC error code */

/* Array of filter coefficients B (feed-forward taps) in Q1.15 format */
static int16_t Coefficients[COEFF_VECTOR_B_SIZE] =
{
  2212, 8848, 13272, 8848, 2212
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

/* Array of input values in q1.15 format */
static q15_t InputSamplesQ15[INPUT_SAMPLES_SIZE];

/** Buffer to store converted raw data to be transferred through DMA.
  * - Non-cacheable memory for data cache consistency.
  * - Aligned for DMA constraints.
  * - Mandatory with data cache enabled, harmless otherwise: portable across STM32 series.
  */
__attribute__((section("non_cacheable_area"), aligned(DMA_ACCESS_ALIGNMENT)))
/* Array of calculated filtered data in Q1.15 format */
int16_t OutputSamples[OUTPUT_SAMPLES_SIZE];

/* Array of reference filtered data for FIR "5 feed-forward taps, gain = 2" in Q1.15 format */
const int16_t ReferenceFilteredData[OUTPUT_SAMPLES_SIZE] =
{
  0x2370, 0x3498, 0x447e, 0x539a, 0x60ed, 0x6cdb, 0x76c6, 0x7ea8, 0x8483, 0x87d3,
  0x892a, 0x87c5, 0x8467, 0x7e7e, 0x768f, 0x6c98, 0x609e, 0x5343, 0x441f, 0x3432,
  0x2306, 0x1185, 0xff92, 0xeda0, 0xdc24, 0xcb01, 0xbb21, 0xac0d, 0x9ec4, 0x92e1,
  0x8902, 0x812d, 0x7b60, 0x781d, 0x76d5, 0x7848, 0x7bb5, 0x81ac, 0x89a7, 0x93aa,
  0x9fae, 0xad14, 0xbc40, 0xcc33, 0xdd63, 0xeee7, 0x00dc, 0x12cc, 0x2444, 0x3563,
  0x453c, 0x5448, 0x6188, 0x6d61, 0x7734, 0x7efb, 0x84bb, 0x87ef, 0x8929, 0x87a7,
  0x842c, 0x7e28, 0x7620, 0x6c11, 0x6002, 0x5293, 0x435f, 0x3366, 0x2230, 0x10aa,
  0xfeb5, 0xecc5, 0xdb50, 0xca36, 0xba63, 0xab5f, 0x9e2a, 0x925c, 0x8895, 0x80da,
  0x7b28, 0x7802, 0x76d6, 0x7867, 0x7bef, 0x8201, 0x8a17, 0x9432, 0xa04c, 0xadc4,
  0xbcff, 0xccff, 0xde39, 0xefc3, 0x01b9, 0x13a7, 0x251a, 0x362f, 0x45fb, 0x54f6,
  0x6222, 0x6de5, 0x77a0, 0x7f4e, 0x84f2, 0x880a, 0x8927, 0x8788, 0x83f1, 0x7dd1,
  0x75af, 0x6b88, 0x5f64, 0x51e3, 0x42a0, 0x329a, 0x215b, 0x0fcf, 0xfdd9, 0xebec,
  0xda7c, 0xc96c, 0xb9a6, 0xaab2, 0x9d90, 0x91d9, 0x882a, 0x8089, 0x7af2, 0x77e8,
  0x76d9, 0x7886, 0x7c2b, 0x8259, 0x8a88, 0x94bb, 0xa0eb, 0xae75, 0xbdc0, 0xcdcc,
  0xdf0f, 0xf09e, 0x0295, 0x1481, 0x25ed, 0x36f8, 0x46b7, 0x55a3, 0x62bc, 0x6e68,
  0x780b, 0x7f9e, 0x8527, 0x8823, 0x8923, 0x8767, 0x83b4, 0x7d79, 0x753d, 0x6aff,
  0x5ec5, 0x5132, 0x41df, 0x31cc, 0x2085, 0x0ef4, 0xfcfc, 0xeb11, 0xd9a8, 0xc8a1,
  0xb8e9, 0xaa06, 0x9cf8, 0x9156, 0x87bf, 0x8039, 0x7abd, 0x77d1, 0x76df, 0x78a9,
  0x7c6a, 0x82b2, 0x8afa, 0x9545, 0xa189, 0xaf27, 0xbe82, 0xce9a, 0xdfe5, 0xf179,
  0x0371, 0x155b, 0x26c1, 0x37c2, 0x4774, 0x564f, 0x6354, 0x6eeb, 0x7875, 0x7fef,
  0x855c, 0x883a, 0x891d, 0x8745, 0x8375, 0x7d20, 0x74cb, 0x6a75, 0x5e26, 0x5080,
  0x411d, 0x30fe, 0x1faf, 0x0e19, 0xfc20, 0xea37, 0xd8d4, 0xc7d8, 0xb82d, 0xa95b,
  0x9c60, 0x90d4, 0x8756, 0x7fea, 0x7a8a, 0x77b9, 0x76e4, 0x78cb, 0x7ca8, 0x830c,
  0x8b6e, 0x95d0, 0xa229, 0xafd9, 0xbf43, 0xcf67, 0xe0bc, 0xf254, 0x044e, 0x1635,
  0x2794, 0x388c, 0x4830, 0x56fa, 0x63ec, 0x6f6c, 0x78dd, 0x803c, 0x858e, 0x8851,
  0x8918, 0x8723, 0x8337, 0x7cc6, 0x7457, 0x69e9, 0x5d85, 0x4fcc, 0x405a, 0x3030,
  0x1ed8, 0x0d3d, 0xfb43, 0xe95e, 0xd802, 0xc70f, 0xb771, 0xa8b0, 0x9bc8, 0x9053,
  0x86ee, 0x7f9c, 0x7a58, 0x77a4, 0x76ec, 0x78ef, 0x7ce8, 0x8367, 0x8be3, 0x965c,
  0xa2ca, 0xb08d, 0xc006, 0xd036, 0xe193, 0xf32f, 0x052a, 0x170e, 0x2866, 0x3953,
  0x48ea, 0x57a4, 0x6482, 0x6fec, 0x7945, 0x808a, 0x85c0, 0x8866, 0x890f, 0x86fd,
  0x82f5, 0x7c6a, 0x73e2, 0x695d, 0x5ce3, 0x4f18, 0x3f97, 0x2f61, 0x1e00, 0x0c62,
  0xfa67, 0xe884, 0xd72e, 0xc646, 0xb6b6, 0xa806, 0x9b33, 0x8fd4, 0x8687, 0x7f4f,
  0x7a27, 0x7790, 0x76f4, 0x7914, 0x7d2a, 0x83c3, 0x8c58, 0x96e9, 0xa36c, 0xb140,
  0xc0c9, 0xd105, 0xe26a, 0xf40b, 0x0606, 0x17e7, 0x293a, 0x3a1c, 0x49a5, 0x584c,
  0x6516, 0x706a, 0x79ab, 0x80d5, 0x85f0, 0x8879, 0x8906, 0x86d8, 0x82b4, 0x7c0e,
  0x736c, 0x68d0, 0x5c42, 0x4e64, 0x3ed4, 0x2e92, 0x1d29, 0x0b86, 0xf98b, 0xe7ab,
  0xd65d, 0xc57f, 0xb5fd, 0xa75d, 0x9a9e, 0x8f56, 0x8622, 0x7f05, 0x79f8, 0x777d,
  0x76fe, 0x793b, 0x7d6c, 0x8421, 0x8ccf, 0x9777, 0xa410, 0xb1f6, 0xc18d, 0xd1d4,
  0xe341, 0xf4e6, 0x06e2, 0x18c0, 0x2a0c, 0x3ae4, 0x4a5f, 0x58f6, 0x65ab, 0x70e7,
  0x7a0f, 0x811f, 0x861e, 0x888b, 0x88fb, 0x86b0, 0x8270, 0x7baf, 0x72f4, 0x6840,
  0x5b9e, 0x4daf, 0x3e10, 0x2dc2, 0x1c51, 0x0aa9, 0xf8ae, 0xe6d2, 0xd58a, 0xc4b8,
  0xb544, 0xa6b6, 0x9a0a, 0x8ed8, 0x85bd, 0x7ebb, 0x79ca, 0x776b, 0x770a, 0x7963,
  0x7db1, 0x847f, 0x8d47, 0x9806, 0xa4b3, 0xb2ab, 0xc251, 0xd2a4, 0xe419, 0xf5c3,
  0x07bf, 0x1999, 0x2add, 0x3bab, 0x4b17, 0x599c, 0x663e, 0x7164, 0x7a73, 0x8168,
  0x864b, 0x889b, 0x88ef, 0x8687, 0x822b, 0x7b4f, 0x727b, 0x67b0, 0x5af9, 0x4cf8,
  0x3d4b, 0x2cf2, 0x1b7a, 0x09cf, 0xf7d3, 0xe5fa, 0xd4ba, 0xc3f2, 0xb48c, 0xa60f,
  0x9978, 0x8e5d, 0x855b, 0x7e73, 0x799d, 0x775b, 0x7716, 0x798c, 0x7df6, 0x84e0,
  0x8dc1, 0x9897, 0xa558, 0xb362, 0xc316, 0xd374, 0xe4f1, 0xf69f, 0x089b, 0x1a71,
  0x2bae, 0x3c70, 0x4bcf, 0x5a43, 0x66d1, 0x71e0, 0x7ad6, 0x81b1, 0x8678, 0x88ab,
  0x88e2, 0x865d, 0x81e5, 0x7aee, 0x7201, 0x6720, 0x5a54, 0x4c41, 0x3c85, 0x2c22,
  0x1aa2, 0x08f3, 0xf6f6, 0xe522, 0xd3e9, 0xc32b, 0xb3d4, 0xa568
};

volatile uint8_t WriteX1Complete; /* Set to 1 if the write operation is correctly completed */
volatile uint8_t ReadYComplete;  /* Set to 1 if the read operation is correctly completed */
volatile uint8_t FmacError;  /* Set to 1 if the an error occurred */


/* Private functions prototype -----------------------------------------------*/
static void WriteX1CompleteCallback(hal_fmac_handle_t *handle_fmac);
static void ReadYCompleteCallback(hal_fmac_handle_t *handle_fmac);
static void ErrorCallback(hal_fmac_handle_t *handle_fmac);


/** ########## Step 1 ##########
  * The init of the FMAC instance is triggered by the applicative code.
  * User's callbacks for IT writing and DMA reading complete are registered.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  pFMAC = mx_example_fmac_init();
  if (pFMAC == NULL)
  {
    goto _app_init_exit;
  }

  /* Select Callback function called after WriteX1 complete */
  if (HAL_FMAC_RegisterWriteX1CpltCallback(pFMAC, WriteX1CompleteCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Select Callback function called after ReadY complete */
  if (HAL_FMAC_RegisterReadYCpltCallback(pFMAC, ReadYCompleteCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Select Callback function called after Error interrupt */
  if (HAL_FMAC_RegisterErrorCallback(pFMAC, ErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  return_status = EXEC_STATUS_INIT_OK;
  PRINTF("[INFO] Step 1: Device initialization COMPLETED.\n");

_app_init_exit:
  return return_status;
} /* end app_init */


app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /** ########## Step 2 ##########
    * Preload X2 buffer then configure write and read transfers.
    */

  /* Preload X2 buffer */
  hal_fmac_preload_x2_t preloadX2_data;
  preloadX2_data.p_coeff_b = Coefficients;
  preloadX2_data.coeff_b_size_half_word = COEFF_VECTOR_B_SIZE;
  preloadX2_data.coeff_a_size_half_word = 0;    /* Coeff A array is not used for FIR filtering */
  if (HAL_FMAC_PreloadX2(pFMAC, &preloadX2_data) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Convert float buffer in q1.15 format */
  arm_float_to_q15(InputSamplesFloat, InputSamplesQ15, INPUT_SAMPLES_SIZE);

  /* Write X1 input buffer in interrupt mode */
  if (HAL_FMAC_WriteX1_IT(pFMAC, InputSamplesQ15, INPUT_SAMPLES_SIZE) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Read Y output buffer in DMA mode */
  if (HAL_FMAC_ReadY_DMA(pFMAC, OutputSamples, OUTPUT_SAMPLES_SIZE)  != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2: FMAC inputs and output configured.\n");


  /** ########## Step 3 ##########
    * Start FMAC FIR Filter
    */
  if (HAL_FMAC_StartFilterFIR(pFMAC, COEFF_VECTOR_B_SIZE, FILTER_GAIN, HAL_FMAC_OPT_IT_FILTER_NONE) != HAL_OK)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 3: FMAC started.\n");

  /* Wait for completed event or error detection */
  while ((WriteX1Complete == 0) && (FmacError == 0))
  {
    /** Put the CPU in Wait For Interrupt state. A FMAC or SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the DMA. It can be replaced by your own code.
      */
    __WFI();
  }
  if (FmacError != 0)
  {
    fmac_error_code = HAL_FMAC_GetLastErrorCodes(pFMAC);
    PRINTF("[ERROR] FMAC error, HAL_FMAC_GetLastErrorCodes = %" PRIu32 ".\n", fmac_error_code);
    goto _app_process_exit;
  }

  /* Wait for completed event or error detection */
  while ((ReadYComplete ==  0)  && (FmacError == 0))
  {
    /** Put the CPU in Wait For Interrupt state. A DMA interrupt or a SystTick interrupt can wake up the CPU.
      * @user: This process is used to illustrate the interest of the DMA. It can be replaced by your own code.
      */
    __WFI();
  }
  if (FmacError != 0)
  {
    fmac_error_code = HAL_FMAC_GetLastErrorCodes(pFMAC);
    PRINTF("[ERROR] FMAC error, HAL_FMAC_GetLastErrorCodes = %" PRIu32 ".\n", fmac_error_code);
    goto _app_process_exit;
  }

  if (HAL_FMAC_StopFilter(pFMAC) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /** ########## Step 4 ##########
    * Check filtering results.
    */
  if (memcmp(OutputSamples, ReferenceFilteredData, sizeof(OutputSamples)) != 0)
  {
    PRINTF("[ERROR] FMAC calculated buffer different than the expected one.\n");
    goto _app_process_exit;
  }

  return_status = EXEC_STATUS_OK;
  PRINTF("[INFO] Step 4: Calculated buffer equal reference buffer.\n");

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 5 ##########
  * Deinitializes the FMAC instance before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  PRINTF("[INFO] Step 5: de-init \n");
  mx_example_fmac_deinit();

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** FMAC write X1 complete callback
  * @user This implementation of the FMAC callback can be customized.
  * This function is executed if the write X1 complete interrupt is generated.
  */
static void WriteX1CompleteCallback(hal_fmac_handle_t *handle_fmac)
{
  /* async processing related to step 3 */
  WriteX1Complete = 1;
}

/** FMAC read Y complete callback
  * @user This implementation of the FMAC callback can be customized.
  * This function is executed if the read Y complete interrupt is generated.
  */
static void ReadYCompleteCallback(hal_fmac_handle_t *handle_fmac)
{
  /* async processing related to step 3 */
  ReadYComplete = 1;
}

/** FMAC error callback
  * @user This implementation of the FMAC callback can be customized.
  * This function is executed if error interrupt is generated.
  */
static void ErrorCallback(hal_fmac_handle_t *handle_fmac)
{
  /* async processing related to step 3 */
  FmacError = 1;
}
