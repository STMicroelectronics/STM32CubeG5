/**
  ******************************************************************************
  * file           : example.c
  * brief          : This example describes how to use the STM32 Cryptographic
  *                  library to generate an ML-KEM key pair, encapsulate and
  *                  decapsulate a shared secret (post-quantum ML-KEM-512,
  *                  NIST FIPS 203).
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
/* PQC context */
cmox_pqc_handle_t Pqc_Ctx;
uint8_t Private_Key[CMOX_PQC_ML_KEM_512_PRIVKEY_LEN];
uint8_t Public_Key[CMOX_PQC_ML_KEM_512_PUBKEY_LEN];
uint8_t Ciphertext[CMOX_PQC_ML_KEM_512_CIPHERTEXT_LEN];
uint8_t Shared_Secret[CMOX_PQC_ML_KEM_512_SHARED_LEN];
uint8_t Decrypted_Shared_Secret[CMOX_PQC_ML_KEM_512_SHARED_LEN];
size_t Private_Key_Length;
size_t Public_Key_Length;
size_t Ciphertext_Length;
size_t Shared_Secret_Length;
size_t Decrypted_Shared_Secret_Length;


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
  * For this example, the random seeds for the key generation and key encapsulation
  * are fixed. For real applications, they need to come from a TRNG or a
  * cryptographically-secure DRNG, and MUST be used just once, then freshly
  * generated.
  * suite: ML-KEM-512
  */
const uint8_t KeyGen_Seed[64] =
{
  0xDAu, 0xCEu, 0xFFu, 0x5Fu, 0x86u, 0x5Au, 0x31u, 0xB7u, 0xA4u, 0x28u, 0x3Cu, 0x05u, 0x22u, 0x0Cu, 0x7Au, 0xEFu,
  0x78u, 0xF9u, 0x63u, 0x29u, 0xEEu, 0x64u, 0xABu, 0xABu, 0x99u, 0xAFu, 0xD1u, 0xC3u, 0x7Du, 0xEFu, 0x6Bu, 0xDBu,
  0x6Du, 0x35u, 0xCAu, 0xA0u, 0x25u, 0x24u, 0xC3u, 0x48u, 0xC0u, 0xEEu, 0xEAu, 0x9Du, 0xCFu, 0x30u, 0xB0u, 0xFEu,
  0xAFu, 0xFAu, 0x59u, 0xE8u, 0x1Fu, 0x5Fu, 0x45u, 0xA5u, 0x6Fu, 0x25u, 0x83u, 0x49u, 0xA2u, 0xF2u, 0xA5u, 0x8Eu
};

const uint8_t Enc_Seed[32] =
{
  0x10u, 0x9Au, 0x24u, 0x8Fu, 0xE8u, 0x05u, 0x2Fu, 0x84u, 0x27u, 0x1Fu, 0xF5u, 0x7Bu, 0xACu, 0x15u, 0x6Bu, 0x1Bu,
  0xA6u, 0xA5u, 0x09u, 0xCDu, 0xCDu, 0xBCu, 0xC9u, 0x6Cu, 0xCDu, 0xB1u, 0xCCu, 0xB8u, 0x5Cu, 0xA4u, 0x93u, 0x15u
};

/* Private functions prototype -----------------------------------------------*/

app_status_t app_init(void)
{
  return EXEC_STATUS_INIT_OK;
} /* end app_init */


/** ########## Step 1 ##########
  * Demonstrates how to use the STM32 Cryptographic library to generate an
  * ML-KEM key pair, encapsulate a shared secret, decapsulate it and verify
  * that both shared secrets match, using the post-quantum ML-KEM-512 algorithm
  * (NIST FIPS 203).
  *
  * Construct a PQC context for KEM, specifying the low-level engines. No
  * temporary working buffer is needed for internal usage.
  *
  * Note: CMOX_PQC_LLENGINES_DEFAULT refers to the default low-level engines
  * selection. To use a specific implementation, the user can directly choose
  * another set of engines defined in the library.
  *
  * This step is scheduled as follows:
  * Step 1.1: Constructs the PQC context and generates the ML-KEM-512 key pair
  * Step 1.2: Encapsulates the shared secret using the public key
  * Step 1.3: Decapsulates the shared secret using the private key
  * Step 1.4: Verifies the decapsulated shared secret matches the original
  * Step 1.5: Cleans up the PQC context
  *
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;
  cmox_pqc_retval_t retval;

  /** ########## Step 1.1 ##########
    * Constructs the PQC context and generates the ML-KEM-512 key pair
    */
  /**
    * Construct a PQC context for KEM, specifying the algorithm implementation (ML-KEM).
    * NO need to set a temporary buffer for internal usage
    */
  cmox_pqc_kem_construct(&Pqc_Ctx, CMOX_PQC_LLENGINES_DEFAULT, NULL, 0u);

  /* Generate the key pair passing all the needed parameters */
  retval = cmox_pqc_kem_keyGen(&Pqc_Ctx, /* Initialized context */
                               CMOX_PQC_ML_KEM_512_KEYGEN_ALGO, /* Use ML-KEM-512 key generation algorithm */
                               KeyGen_Seed, sizeof(KeyGen_Seed), /* Random seed (and its length) to generate keys */
                               Private_Key, /* Buffer that will contain the private key */
                               &Private_Key_Length, /* Variable that will contain private key length */
                               Public_Key, /* Buffer that will contain the public key */
                               &Public_Key_Length); /* Variable that will contain public key length */

  /* Verify API returned value */
  if (retval != CMOX_PQC_SUCCESS)
  {
    PRINTF("[ERROR] Step 1.1: ML-KEM key pair generation ERROR.\n");
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 1.1: ML-KEM key pair generation COMPLETED.\n");

  /** ########## Step 1.2 ##########
    * Encapsulates the shared secret using the public key
    */
  /* Encapsulate a key passing all the needed parameters */
  retval = cmox_pqc_kem_encapsulate(&Pqc_Ctx, /* Initialized context */
                                    CMOX_PQC_ML_KEM_512_ENC_ALGO, /* Use ML-KEM-512 key encapsulation algorithm */
                                    Enc_Seed, sizeof(Enc_Seed), /* Random seed (and its length) to encapsulate key */
                                    Public_Key, sizeof(Public_Key), /* Buffer containing the public key,
                                                                       and its length */
                                    Ciphertext, /* Buffer that will contain the encrypted key */
                                    &Ciphertext_Length, /* Variable that will contain the encrypted key length */
                                    Shared_Secret, /* Buffer that will contain the key (not encrypted) */
                                    &Shared_Secret_Length); /* Variable that will contain the key length */

  /* Verify API returned value */
  if (retval != CMOX_PQC_SUCCESS)
  {
    PRINTF("[ERROR] Step 1.2: Shared secret encapsulation ERROR.\n");
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 1.2: Shared secret encapsulation COMPLETED.\n");

  /** ########## Step 1.3 ##########
    * Decapsulates the shared secret using the private key
    */
  /* Decapsulate the key passing all the needed parameters */
  retval = cmox_pqc_kem_decapsulate(&Pqc_Ctx, /* Initialized context */
                                    CMOX_PQC_ML_KEM_512_DEC_ALGO, /* Use ML-KEM-512 key decapsulation algorithm */
                                    Private_Key, sizeof(Private_Key), /* Buffer containing the private key,
                                                                         and its length */
                                    Ciphertext, sizeof(Ciphertext), /* Buffer containing the encrypted key,
                                                                       and its length */
                                    Decrypted_Shared_Secret, /* Buffer that will contain the decapsulated key */
                                    &Decrypted_Shared_Secret_Length); /* Variable that will contain the key length */

  /* Verify API returned value */
  if (retval != CMOX_PQC_SUCCESS)
  {
    PRINTF("[ERROR] Step 1.3: Shared secret decapsulation ERROR.\n");
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 1.3: Shared secret decapsulation COMPLETED.\n");

  /** ########## Step 1.4 ##########
    * Verifies the decapsulated shared secret matches the original
    */
  /* Check that decrypted key length equals the length of the original shared secret */
  if (Shared_Secret_Length != Decrypted_Shared_Secret_Length)
  {
    PRINTF("[ERROR] Step 1.4: Shared secret length mismatch ERROR.\n");
    goto _app_process_exit;
  }

  /* Check that decrypted key value is the same of the original shared secret value */
  if (memcmp(Shared_Secret, Decrypted_Shared_Secret, sizeof(Shared_Secret)) != 0)
  {
    PRINTF("[ERROR] Step 1.4: Shared secret value mismatch ERROR.\n");
    goto _app_process_exit;
  }
  PRINTF("[INFO] Step 1.4: Verify the decapsulated shared secret COMPLETED.\n");

  /** ########## Step 1.5 ##########
    * Cleans up the PQC context
    */
  /* Cleanup context */
  cmox_pqc_kem_cleanup(&Pqc_Ctx);
  PRINTF("[INFO] Step 1.5: Clean-up the PQC context COMPLETED.\n");

  return_status = EXEC_STATUS_OK;

_app_process_exit:
  return return_status;
} /* end app_process */


app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;
} /* end app_deinit */
