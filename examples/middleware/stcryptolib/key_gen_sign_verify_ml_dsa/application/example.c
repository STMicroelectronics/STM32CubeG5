/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Cryptographic
  *                  library to generate an ML-DSA key pair, sign a message and
  *                  verify the signature (post-quantum ML-DSA-44, NIST FIPS 204).
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
#include "cmox_crypto.h"        /* Crypto services and defines */
#include <string.h>             /* Various functions for character manipulating. */

/* Global variables ----------------------------------------------------------*/
cmox_pqc_handle_t Pqc_Ctx;
uint8_t Private_Key[CMOX_PQC_ML_DSA_44_PRIVKEY_LEN];
uint8_t Public_Key[CMOX_PQC_ML_DSA_44_PUBKEY_LEN];
uint8_t Header[CMOX_PQC_DSA_HEADER_MAX_LENGTH];
uint8_t Signature[CMOX_PQC_ML_DSA_44_SIG_LEN];
size_t Private_Key_Length;
size_t Public_Key_Length;
size_t Header_Length;
size_t Signature_Length;
uint8_t membuf[12000];


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
  * WARNING: This is data to be used for example purposes,
  * DO NOT USE IT ON PRODUCTION CODE!
  *
  */

/**
  * For this example, the random seeds for the key and signature generation
  * are fixed. For real applications, they need to come from a TRNG or a
  * cryptographically-secure DRNG, and MUST be used just once, then freshly
  * generated.
  * suite: ML-DSA-44
  */
const uint8_t KeyGen_Seed[CMOX_PQC_ML_DSA_44_KEYGEN_SEED_LEN] =
{
  0x93u, 0xEFu, 0x2Eu, 0x6Eu, 0xF1u, 0xFBu, 0x08u, 0x99u, 0x9Du, 0x14u, 0x2Au, 0xBEu, 0x02u, 0x95u, 0x48u, 0x23u,
  0x70u, 0xD3u, 0xF4u, 0x3Bu, 0xDBu, 0x25u, 0x4Au, 0x78u, 0xE2u, 0xB0u, 0xD5u, 0x16u, 0x8Eu, 0xCAu, 0x06u, 0x5Fu
};

const uint8_t Sig_Seed[CMOX_PQC_ML_DSA_44_SIGN_SEED_LEN] =
{
  0x00u, 0x48u, 0x13u, 0xC9u, 0xE7u, 0xD2u, 0xC6u, 0x54u, 0xC2u, 0x53u, 0x0Au, 0x71u, 0xE5u, 0xF8u, 0xC1u, 0x09u,
  0x42u, 0xFBu, 0x6Du, 0x88u, 0x41u, 0x53u, 0x5Au, 0xB1u, 0xDAu, 0x43u, 0xE8u, 0xCBu, 0x0Bu, 0xB8u, 0x9Eu, 0x78u
};

const uint8_t Message[256] =
{
  0x16u, 0xD6u, 0xA2u, 0xB1u, 0xF4u, 0x71u, 0xA5u, 0x1Fu, 0x24u, 0x86u, 0xC0u, 0x64u, 0x2Cu, 0xB7u, 0x1Bu, 0xF7u,
  0x0Bu, 0x8Cu, 0xC1u, 0xA8u, 0xC4u, 0xDEu, 0x01u, 0xCDu, 0x08u, 0x77u, 0xD1u, 0x92u, 0x83u, 0x50u, 0xBAu, 0xC0u,
  0x6Au, 0xA1u, 0xEAu, 0x6Du, 0xE7u, 0x0Fu, 0xC2u, 0x2Eu, 0x8Eu, 0x18u, 0xB5u, 0x74u, 0x2Du, 0xDDu, 0xEFu, 0x43u,
  0xC3u, 0x42u, 0x49u, 0x11u, 0x72u, 0x91u, 0xB5u, 0x55u, 0xA2u, 0xBBu, 0x72u, 0xDFu, 0x31u, 0x4Cu, 0x71u, 0x02u,
  0xC7u, 0xD4u, 0x4Eu, 0x56u, 0x17u, 0x08u, 0x41u, 0x5Cu, 0x66u, 0xA2u, 0xE8u, 0xEBu, 0x1Bu, 0xDEu, 0x34u, 0xD1u,
  0xC5u, 0x30u, 0xD0u, 0x77u, 0x32u, 0xD3u, 0xADu, 0xF9u, 0x84u, 0x91u, 0xBBu, 0xFDu, 0x5Cu, 0xE5u, 0xFAu, 0x1Cu,
  0xA1u, 0xE9u, 0xC1u, 0x14u, 0xD6u, 0x5Du, 0xF4u, 0x0Du, 0xFFu, 0x9Eu, 0x07u, 0xB4u, 0xACu, 0xE4u, 0xF8u, 0x63u,
  0x49u, 0x75u, 0xA6u, 0x51u, 0xD0u, 0x5Du, 0x1Au, 0x3Du, 0x8Cu, 0x28u, 0x0Au, 0x56u, 0x44u, 0x2Eu, 0xCEu, 0x53u,
  0xE2u, 0xA8u, 0xF2u, 0x26u, 0x39u, 0x10u, 0x80u, 0x37u, 0x1Au, 0x50u, 0xFCu, 0xFDu, 0xB3u, 0x47u, 0x76u, 0xB6u,
  0x55u, 0x5Du, 0x38u, 0x42u, 0xD7u, 0x75u, 0x34u, 0xAAu, 0xF0u, 0x65u, 0x62u, 0xF8u, 0xD6u, 0xA4u, 0xB8u, 0xB6u,
  0xD0u, 0xB4u, 0xD8u, 0x90u, 0x42u, 0xC3u, 0x35u, 0x90u, 0x6Eu, 0xC3u, 0xB2u, 0xB7u, 0x3Au, 0x8Au, 0x7Bu, 0xF5u,
  0xFBu, 0xBDu, 0x57u, 0xF2u, 0x0Fu, 0xE9u, 0xAEu, 0x26u, 0x33u, 0x79u, 0x78u, 0x8Bu, 0xCDu, 0xAFu, 0xF9u, 0x53u,
  0x2Au, 0x36u, 0x46u, 0xDBu, 0x37u, 0x8Eu, 0x5Du, 0x6Eu, 0x8Au, 0xA6u, 0xCCu, 0xE7u, 0x3Fu, 0xC2u, 0x8Au, 0xB9u,
  0x31u, 0x49u, 0xD1u, 0xEFu, 0x84u, 0x5Au, 0x53u, 0x65u, 0x63u, 0x4Au, 0x8Du, 0x9Bu, 0x96u, 0xFCu, 0xD1u, 0x60u,
  0x59u, 0x8Bu, 0x3Bu, 0x52u, 0x1Bu, 0xF2u, 0xFAu, 0xCDu, 0x91u, 0xFCu, 0x6Cu, 0xD2u, 0x88u, 0x93u, 0x8Bu, 0x90u,
  0x24u, 0xF0u, 0xCFu, 0x7Eu, 0xDEu, 0x0Bu, 0x83u, 0xECu, 0x46u, 0xCCu, 0xD8u, 0xEFu, 0x75u, 0x9Au, 0x13u, 0x38u
};

const uint8_t Sig_Context[3] =
{
  0x01u, 0x02u, 0x03u
};

/* Private functions prototype -----------------------------------------------*/

app_status_t app_init(void)
{
  return EXEC_STATUS_INIT_OK;
} /* end app_init */


/** ########## Step 1 ##########
  * Demonstrates how to use the STM32 Cryptographic library to generate an
  * ML-DSA key pair, sign a message and verify the signature, using the
  * post-quantum ML-DSA-44 algorithm (NIST FIPS 204).
  *
  * Construct a PQC context, specifying the low-level engines and the working
  * buffer used for later processing.
  *
  * Note: CMOX_PQC_LLENGINES_DEFAULT refers to the default low-level engines
  * selection. To use a specific implementation, the user can directly choose
  * another set of engines defined in the library.
  *
  * This step is scheduled as follows:
  * Step 1.1: Constructs the PQC context and generates the ML-DSA-44 key pair
  * Step 1.2: Prepares the signing header from the signing context string
  * Step 1.3: Generates the signature of the known message
  * Step 1.4: Verifies the signature (with fault-injection protection check)
  * Step 1.5: Cleans up the PQC context
  *
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  cmox_pqc_retval_t retval;
  /* Fault check verification variable */
  uint32_t fault_check = CMOX_PQC_AUTH_FAIL;

  /** ########## Step 1.1 ##########
    * Constructs the PQC context and generates the ML-DSA-44 key pair
    */
  /**
    * Construct a PQC context for DSA, specifying the algorithm implementation (ML-DSA)
    */
  cmox_pqc_dsa_construct(&Pqc_Ctx, CMOX_PQC_LLENGINES_DEFAULT, membuf, sizeof(membuf));

  /* Generate the key pair passing all the needed parameters */
  retval = cmox_pqc_dsa_keyGen(&Pqc_Ctx, /* Initialized context */
                               CMOX_PQC_ML_DSA_44_KEYGEN_ALGO, /* Use ML-DSA key generation algorithm */
                               KeyGen_Seed, sizeof(KeyGen_Seed), /* Random seed (and its length) to generate keys */
                               Private_Key, /* Buffer that will contain the private key */
                               &Private_Key_Length, /* Variable that will contain private key length */
                               Public_Key, /* Buffer that will contain the public key */
                               &Public_Key_Length); /* Variable that will contain public key length */

  /* Verify API returned value */
  if (retval != CMOX_PQC_SUCCESS)
  {
    PRINTF("[ERROR] Step 1.1: ML-DSA key pair generation ERROR.\n");
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 1.1: ML-DSA key pair generation COMPLETED.\n");

  /** ########## Step 1.2 ##########
    * Prepares the signing header from the signing context string
    */
  retval = cmox_pqc_dsa_prepareHeader(&Pqc_Ctx, /* Initialized PQC context */
                                      Sig_Context, sizeof(Sig_Context), /* Signing context, that is a
                                                                           personalization binary string */
                                      CMOX_PQC_DSA_STANDARD, /* ML-DSA standard, not the pre-hashed version */
                                      Header, /* Buffer that will contain the signing header */
                                      &Header_Length); /* Variable that will contain the header length */

  /* Verify API returned value */
  if (retval != CMOX_PQC_SUCCESS)
  {
    PRINTF("[ERROR] Step 1.2: Prepare the signing header ERROR.\n");
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 1.2: Prepare the signing header COMPLETED.\n");

  /** ########## Step 1.3 ##########
    * Generates the signature of the known message
    */
  /* Generate a signature passing all the needed parameters */
  retval = cmox_pqc_dsa_sign(&Pqc_Ctx, /* Initialized context */
                             CMOX_PQC_ML_DSA_44_SIGN_LOWRAM_ALGO, /* Use ML-DSA signature generation algorithm */
                             Sig_Seed, sizeof(Sig_Seed), /* Random seed (and its length) to sign the message */
                             Private_Key, sizeof(Private_Key), /* Buffer containing the private key, and its length */
                             Header, Header_Length, /* Buffer containing the header, and its length */
                             Message, sizeof(Message), /* Buffer containing the message, and its length */
                             Signature, /* Buffer that will contain the signature */
                             &Signature_Length); /* Variable that will contain the signature length */

  /* Verify API returned value */
  if (retval != CMOX_PQC_SUCCESS)
  {
    PRINTF("[ERROR] Step 1.3: Generate the signature ERROR.\n");
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 1.3: Generate the signature COMPLETED.\n");

  /** ########## Step 1.4 ##########
    * Verifies the signature (with fault-injection protection check)
    */
  /* Verify the signature passing all the needed parameters */
  retval = cmox_pqc_dsa_verify(&Pqc_Ctx, /* Initialized context */
                               CMOX_PQC_ML_DSA_44_VERIFY_LOWRAM_ALGO, /* Use ML-DSA-44 key signature
                                                                         verification algorithm */
                               Public_Key, sizeof(Public_Key), /* Buffer containing the public key, and its length */
                               Header, Header_Length, /* Buffer containing the header, and its length */
                               Message, sizeof(Message), /* Buffer containing the message, and its length */
                               Signature, sizeof(Signature), /* Buffer containing the signature, and its length */
                               &fault_check); /* Variable that will contain an additional return value
                                                 to check, in order to be protected against simple fault attacks */

  /* Verify API returned value */
  if (retval != CMOX_PQC_AUTH_SUCCESS)
  {
    PRINTF("[ERROR] Step 1.4: Verify the signature ERROR.\n");
    goto _app_process_exit;
  }

  /* Verify Fault check variable value */
  if (fault_check != CMOX_PQC_AUTH_SUCCESS)
  {
    PRINTF("[ERROR] Step 1.4: Signature fault-injection check ERROR.\n");
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 1.4: Verify the signature COMPLETED.\n");

  /** ########## Step 1.5 ##########
    * Cleans up the PQC context
    */
  /* Cleanup context */
  cmox_pqc_dsa_cleanup(&Pqc_Ctx);
  PRINTF("[INFO] Step 1.5: Clean-up the PQC context COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;
} /* end app_deinit */
