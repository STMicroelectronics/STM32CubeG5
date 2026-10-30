<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *stcryptolib_auth_verify_aes_cmac*__

**Example version:** 2.0.6

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STM32 Cryptographic library to authenticate and to verify a message using the AES CMAC algorithm.

This example demonstrates, two ways of using the cryptographic services:

  1. The single call method: algorithm configuration and execution are done via a single API call.
  2. The multiple calls method: algorithm configuration and execution are done in several API calls, allowing in particular a piecemeal injection of data to process.

For each method, there are two examples provided the first is a generation of the MAC of a known message, followed by the verification of it.And the second is a verification of the message using the given authentication tag.


## __1. Detailed scenario__

This example is scheduled as follows:

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

 - __Step 1__: Demonstrates how to use the STM32 Cryptographic library to authenticate and to verify a message using the AES CMAC algorithm.

        Step 1.1: Computes and verifies the message using single API call method.
        Step 1.2: Computes and verifies the message using multiple API calls method.

 __End of example__

In case of successful operations:

- The status LED remains turned on
- Global variable `ExecStatus` set to `EXEC_STATUS_OK`
If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1.1: Compute and verify the message using single API call method COMPLETED.
[INFO] Step 1.2: Compute and verify the message using multiple API calls method COMPLETED.
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

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube Firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube Firmware package.")

> **_NOTE:_** NIST vectors examples for AES CMAC are taken from "Block Cipher Modes of Operation - CMAC Mode for Authentication", available at [SP 800-38B](https://csrc.nist.gov/projects/cryptographic-standards-and-guidelines/example-values).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
