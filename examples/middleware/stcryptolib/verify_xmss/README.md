<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *stcryptolib_verify_xmss*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STCryptoLib to verify a message authenticity and integrity with a known signature, using the XMSS algorithm (eXtended Merkle Signature Scheme). This stateful hash-based signature (HBS) scheme, standardized in NIST SP 800-208 and RFC 8391, provides quantum-resistant digital signature verification.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

 - __Step 1__: Demonstrates how to use the STCryptoLib to verify a message authenticity and integrity with a known signature, using the XMSS algorithm with SHA-256.
    - __Step 1.1__: Constructs the HBS context.
    - __Step 1.2__: Verifies the message signature and checks the fault-injection protection value.

__End of example__:

In case of successful operations:

- The status LED remains turned on
- Global variable `ExecStatus` set to `EXEC_STATUS_OK`
If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1.1: Construct the HBS context COMPLETED.
[INFO] Step 1.2: Verify the signature COMPLETED.
```

> **_NOTE:_** For this example, the message, public key and signature are fixed test vectors generated with the HBS tool provided under `Utilities/PC_Software/HBS_Utility`. For real applications, they need to be produced by the signing party from freshly generated key material.


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

> **_NOTE:_** XMSS is specified in NIST SP 800-208 and in RFC 8391, available at: [NIST SP 800-208](https://csrc.nist.gov/pubs/sp/800/208/final) and [RFC 8391](https://datatracker.ietf.org/doc/html/rfc8391)


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
