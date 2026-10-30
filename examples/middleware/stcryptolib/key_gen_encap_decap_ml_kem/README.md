<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *stcryptolib_key_gen_encap_decap_ml_kem*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STCryptoLib for ML-KEM key generation, encapsulation and decapsulation of a shared secret. This post-quantum (PQC) key-encapsulation mechanism, standardized in NIST FIPS 203, provides quantum-resistant establishment of a shared secret.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

 - __Step 1__: Demonstrates how to use the STCryptoLib to generate an ML-KEM key pair, encapsulate a shared secret, decapsulate it and verify that both shared secrets match, using the post-quantum ML-KEM-512 algorithm.
    - __Step 1.1__: Constructs the PQC context for KEM and generates the ML-KEM-512 key pair from a known seed.
    - __Step 1.2__: Encapsulates the shared secret using the public key.
    - __Step 1.3__: Decapsulates the shared secret using the private key.
    - __Step 1.4__: Verifies that the decapsulated shared secret matches the original.
    - __Step 1.5__: Cleans the PQC context.

__End of example__:

In case of successful operations:

- The status LED remains turned on
- Global variable `ExecStatus` set to `EXEC_STATUS_OK`
If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1.1: ML-KEM key pair generation COMPLETED.
[INFO] Step 1.2: Shared secret encapsulation COMPLETED.
[INFO] Step 1.3: Shared secret decapsulation COMPLETED.
[INFO] Step 1.4: Verify the decapsulated shared secret COMPLETED.
[INFO] Step 1.5: Clean-up the PQC context COMPLETED.
```

> **_NOTE:_** For this example, the random seeds for the key generation and key encapsulation are fixed. For real applications, they need to come from a TRNG or a cryptographically-secure DRNG, and MUST be used just once, then freshly generated.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No generic hardware dependencies for this example.

### __3.2. Specific board setups__

<details>
  <summary>On STM32G5 series.</summary>
  <details>
    <summary>On board NUCLEO-G564RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PG3    |     GPIO      | MX_STATUS_LED |
  |    PA2    |   USART2_TX   |       -       |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

> **_NOTE:_** ML-KEM is specified in NIST FIPS 203, available at: <https://csrc.nist.gov/pubs/fips/203/final>


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
