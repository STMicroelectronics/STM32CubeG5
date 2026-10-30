<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example : *example_uart_dma_secure_nonsecure_secure*__

**Example version:** 0.0.1

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to provide the following secure services using the NSC (Non Secure Callable) techniques:
- Check if the password provided by the user in the non-secure context is correct.


## __1. Detailed scenario__

__Startup phase__: At the secure reset handler, `TZ_SAU_Setup()` is called to configure the SAU (Secure Attribution Unit), the Cortex security features, and the security assignment of the NVIC vectors.

__Initialization phase__: At secure main program start, the `mx_system_init()` function is called. It initializes the HAL and system clocks, then calls `mx_tee_setup()` to configure the trusted execution environment then initializes peripherals used by secure application and start the non-secure application by calling `mx_start_application()`.

The secure application provides only NSC services thaty's why it has no example steps.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

No peripheral configuration is present in secure application.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No generic hardware setup needed for this example.

### __3.2. Specific board setups__

<details>
  <summary>On STM32G5 series.</summary>
  <details>
    <summary>On board NUCLEO-G564RE.</summary>

  | Region      | Start address | Size      |
  |:-----------:|:-------------:|:---------:|
  | ROM         | `0x0C000000`  | `0x3E000` |
  | NSC veneers | `0x0C03E000`  | `0x2000`  |
  | RAM         | `0x30000000`  | `0x8000`  |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

See [top readme Troubleshooting](../README.md#4-troubleshooting)


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

See [top readme See Also](../README.md#5-see-also)


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
