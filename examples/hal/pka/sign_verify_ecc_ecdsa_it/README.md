<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_pka_sign_verify_ecc_ecdsa_it*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to compute and verify a signed message regarding the Elliptic curve digital signature algorithm (ECDSA) in the interrupt mode.


## __1. Detailed scenario__

This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

 - __Step 1__: RNG and PKA initialization

    Initialization of HW RNG that is used by the PKA (Public Key Accelerator).

 - __Step 2__: Computes and verifies signature in the interrupt mode.
    This step is scheduled as follows:
    - __Step 2.1__: Configures PKA for Signature Generation.
    - __Step 2.2__: Launches Signature Generation in IT mode.
    - __Step 2.3__: Compares the ECDSA signature operation result to expected result.
    - __Step 3.1__: Configures PKA for Signature Verification.
    - __Step 3.2__: Launches Signature Verification.
    - __Step 3.3__: Checks if the ECDSA signature is valid.

 - __Step 3__: PKA and RNG deinitialization before leaving the scenario.

__End of example__: After step 3, the example is completed. You can verify that the example runs properly via the status LED (turned on) and the ExecStatus variable (EXEC_STATUS_OK).


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:
```text
[INFO] Step 1: RNG and PKA initialization COMPLETED.
[INFO] Step 2.1: PKA configuration to process message signature using elliptic curves COMPLETED.
[INFO] Step 2.2: Signature computation in IT mode COMPLETED.
[INFO] Step 2.3: Compare the ECDSA signature operation result to expected result COMPLETED.
[INFO] Step 2.3: Compare the ECDSA signature operation result to expected result COMPLETED.
[INFO] Step 2.4: PKA configuration to process message verification using elliptic curves COMPLETED.
[INFO] Step 2.5: Signature computation in IT mode COMPLETED.
[INFO] Step 2.6: Signature verification COMPLETED.
[INFO] Step 3: PKA and RNG deinitialization COMPLETED.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No generic hardware dependencies for this example.

### __3.2. Specific board setups__

This example is designed to run on the boards that support PKA.

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

> **_NOTE:_** Vectors examples for RSA are taken from "psa_arch_tests\api-tests\dev_apis\crypto", available at https://github.com/ARM-software/psa-arch-tests.git
NIST vectors examples for ECDSA are taken from "ECDSA Test vectors / FIPS 186-4", available at https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/digital-signatures

> **_NOTE:_** `Known_Signature` is modified for matching `k` used by ARM (not the same as `k` of NIST)


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
