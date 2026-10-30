/**
  ******************************************************************************
  * file           : example.c
  * brief          : How to use PKA ECDSA signature & verification
  *                  in the interrupt mode.
  *
  ******************************************************************************
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include <string.h>
#include "prime256v1.h" /* reference buffers containing the description of nist P-256 (ECDSA-256) */
#include "sig_gen.h"    /* reference buffers from NIST */
#include "sig_ver.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* timeout value in milliseconds */
#define PKA_TIMEOUT 5000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

hal_pka_handle_t *pPKA;  /* PKA handle */
hal_rng_handle_t *pRNG;  /* RNG handle */

hal_pka_ecdsa_signature_protect_config_t InSign;
hal_pka_ecdsa_verif_config_t InVerif;
hal_pka_ecdsa_signature_protect_result_t Out;
hal_pka_ecdsa_signature_result_ext_config_t OutExt;
uint32_t ResultSize;
/* Data buffers */
uint8_t rSignBuffer[32 + 2] = {0};
uint8_t sSignBuffer[32 + 2] = {0};
uint8_t BufferPtX[32 + 2] = {0};
uint8_t BufferPtY[32 + 2] = {0};
/* Set to 1 if the operation is completed */
uint32_t OperationCpltCb = 0;
/* Set to 1 if an error is detected */
uint32_t ErrorCb = 0;
uint32_t EndOfProcess = 0;
uint32_t HalEndOfProcess = 0;
/* Private functions prototype -----------------------------------------------*/
static void OperationtCpltCallback(hal_pka_handle_t *pPKA);
static void ErrorCallback(hal_pka_handle_t *pPKA);


/** ########## Step 1 ##########
  * The applicative code initializes the RNG and PKA peripherals.
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Init the RNG */
  pRNG = mx_example_rng_init();

  /* Init the PKA */
  pPKA = mx_example_pka_init();

  if ((pRNG == NULL) && (pPKA == NULL))
  {
    goto _app_init_exit;
  }

  /* Register the PKA Error Callback. */
  if (HAL_PKA_RegisterErrorCallback(pPKA, ErrorCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  /* Register the PKA command complete Callback. */
  if (HAL_PKA_RegisterOperationCpltCallback(pPKA, OperationtCpltCallback) != HAL_OK)
  {
    goto _app_init_exit;
  }

  PRINTF("[INFO] Step 1: RNG and PKA initialization COMPLETED.\n");

  return_status = EXEC_STATUS_INIT_OK;

_app_init_exit:
  return return_status;
} /* end app_init */


/** ########## Step 2 ##########
  * Computes a signed message regarding the Elliptic
  * curve digital signature algorithm (ECDSA) in the interrupt mode.
  * Then determine if this signature is valid or not.
  * This step is scheduled as follows:
  * Step 2.1: Configures PKA for Signature Generation
  * Step 2.2: Launches Signature Generation in IT mode.
  * Step 2.3: Compares to expected result.
  * Step 2.4: Configures PKA for verifying the validity of a signature.
  * Step 2.5: Launches PKA operation in the interrupt mode.
  * Step 2.6: Compares to expected result.
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Reset the output buffer */
  memset(rSignBuffer, 0, sizeof(rSignBuffer));
  rSignBuffer[0]       = 0xAA; /* magic number to detect overflow */
  rSignBuffer[33]      = 0xAA; /* magic number to detect overflow */
  memset(sSignBuffer, 0, sizeof(sSignBuffer));
  sSignBuffer[0]       = 0xAA; /* magic number to detect overflow */
  sSignBuffer[33]      = 0xAA; /* magic number to detect overflow */
  memset(BufferPtX, 0, sizeof(BufferPtX));
  BufferPtX[0]  = 0xAA; /* magic number to detect overflow */
  BufferPtX[33] = 0xAA; /* magic number to detect overflow */
  memset(BufferPtY, 0, sizeof(BufferPtY));
  BufferPtY[0]  = 0xAA; /* magic number to detect overflow */
  BufferPtY[33] = 0xAA; /* magic number to detect overflow */

  /* Set output parameters */
  Out.p_r_sign  = &rSignBuffer[1];
  Out.p_s_sign  = &sSignBuffer[1];
  OutExt.p_pt_x = &BufferPtX[1];
  OutExt.p_pt_y = &BufferPtY[1];

  /* Set Sign input parameters */
  InSign.prime_order_size_byte = Prime256v1OrderLen;
  InSign.modulus_size_byte     = Prime256v1PrimeLen;
  InSign.coeff_sign            = Prime256v1_A_Sign;
  InSign.p_coeff               = Prime256v1AbsA;
  InSign.p_coeff_b             = Prime256v1_B;
  InSign.p_modulus             = Prime256v1Prime;
  InSign.p_base_pt_x           = Prime256v1GeneratorX;
  InSign.p_base_pt_y           = Prime256v1GeneratorY;
  InSign.p_prime_order         = Prime256v1Order;
  InSign.p_integer             = SigGen_K;
  InSign.p_hash                = SigGenHashMsg;
  InSign.p_private_key         = SigGen_D;

  /* Set the message signature configuration */
  if (HAL_PKA_ECDSA_SetConfigSignatureProtect(pPKA, &InSign) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.1: PKA configuration to process message signature using elliptic curves COMPLETED.\n");

  /* Launch signature computation */
  if ((HAL_PKA_Compute_IT(pPKA) != HAL_OK))
  {
    goto _app_process_exit;
  }

  /* Wait for one of these PKA interrupts: operation complete or detected error. */
  while ((OperationCpltCb == 0) && (ErrorCb == 0))
  {
    /* Put the CPU in Wait For Interrupt state.*/
    __WFI();
  }

  /* Check if there is an error occurred */
  if (ErrorCb == 1)
  {
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 2.2: Signature computation in IT mode COMPLETED.\n");

  /* Retrieve ECDSA signature */
  ResultSize = HAL_PKA_ECDSA_GetResultSignatureProtect(pPKA, &Out, &OutExt);

  if (ResultSize != SigGen_R_Len)
  {
    goto _app_process_exit;
  }

  /* Compare the ECDSA signature operation result to expected result */
  if (memcmp(Out.p_r_sign, SigGen_R, ResultSize) != 0)
  {
    goto _app_process_exit;
  }

  if (memcmp(Out.p_s_sign, SigGen_S, ResultSize) != 0)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.3: Compare the ECDSA signature operation result to expected result COMPLETED.\n");

  /* Set Verification input parameters. */
  InVerif.prime_order_size_byte = Prime256v1OrderLen;
  InVerif.modulus_size_byte     = Prime256v1PrimeLen;
  InVerif.coeff_sign            = Prime256v1_A_Sign;
  InVerif.p_coeff               = Prime256v1AbsA;
  InVerif.p_modulus             = Prime256v1Prime;
  InVerif.p_base_pt_x           = Prime256v1GeneratorX;
  InVerif.p_base_pt_y           = Prime256v1GeneratorY;
  InVerif.p_prime_order         = Prime256v1Order;
  InVerif.p_pub_key_curve_pt_x  = SigVer_Qx;
  InVerif.p_pub_key_curve_pt_y  = SigVer_Qy;
  InVerif.p_r_sign              = SigVer_R;
  InVerif.p_s_sign              = SigVer_S;
  InVerif.p_hash                = SigVerHashMsg;

  /* Set the configuration for verifying the validity of a signature */
  if (HAL_PKA_ECDSA_SetConfigVerifSignature(pPKA, &InVerif) != HAL_OK)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.4: PKA configuration to process message verification using elliptic curves COMPLETED.\n");

  /* Reset the operation complete callback flag */
  OperationCpltCb = 0;

  /* Launch signature computation */
  if (HAL_PKA_Compute_IT(pPKA) != HAL_OK)
  {
    goto _app_process_exit;
  }

  /* Wait for one of these PKA interrupts: operation complete. */
  while ((OperationCpltCb == 0) && (ErrorCb == 0))
  {
    /* Put the CPU in Wait For Interrupt state.*/
    __WFI();
  }
  if (ErrorCb == 1)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.5: Signature computation in IT mode COMPLETED.\n");

  /* Check if the ECDSA signature is valid */
  if (HAL_PKA_ECDSA_IsValidVerifSignature(pPKA) != PKA_ECDSA_SIGNATURE_VALID)
  {
    goto _app_process_exit;
  }

  PRINTF("[INFO] Step 2.6: Signature verification COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


/** ########## Step 3 ##########
  * Deinitializes the PKA and the RNG before leaving the scenario.
  */
app_status_t app_deinit(void)
{
  mx_example_pka_deinit();
  mx_example_rng_deinit();

  PRINTF("[INFO] Step 3: PKA and RNG deinitialization COMPLETED.\n");

  return EXEC_STATUS_OK;
} /* end app_deinit */


/** Process completed callback.
  * @user this implementation of Process completed callback can be customized.
  */
static void OperationtCpltCallback(hal_pka_handle_t *pPKA)
{
  OperationCpltCb = 1U;
}
/** PKA error callback
  * @user This implementation of the PKA error callback can be customized.
  */
static void ErrorCallback(hal_pka_handle_t *pPKA)
{
  ErrorCb = 1U;
}
