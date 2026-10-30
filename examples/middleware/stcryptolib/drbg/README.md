<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *stcryptolib_drbg*__

**Example version:** 2.0.6

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STCryptoLib to generate random numbers based on known input entropy, nonce and personalization. So that produced random can be compared to known random.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

 - __Step 1__: Demonstrates how to use the STCryptoLib to generate random numbers based on known input entropy, nonce and personalization.
    - __Step 1.1__: Initializes the DRBG context with entropy, nonce and personalization string parameters.
    - __Step 1.2__: Reseeds the DRBG with reseed parameters.
    - __Step 1.3__: Generates random data.
    - __Step 1.4__: Verifies generated data.
    - __Step 1.5__: Cleans the DRBG context.

__End of example__:

In case of successful operations:

- The status LED remains turned on
- Global variable `ExecStatus` set to `EXEC_STATUS_OK`
If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1.1: DRBG context initialization COMPLETED.
[INFO] Step 1.2: Reseed the DRBG with reseed parameters COMPLETED.
[INFO] Step 1.3: Generate random data COMPLETED.
[INFO] Step 1.4: Verify generated data COMPLETED.
[INFO] Step 1.5: Clean-up the DRBG context COMPLETED.
```


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

> **_NOTE:_** NIST vectors examples for DRGB are taken from "Test Vectors", available at: https://csrc.nist.gov/projects/cryptographic-algorithm-validation-program/random-number-generators


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
