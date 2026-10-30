<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_fmac_fir_dma_to_it*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/read/read_toc.html "An offline version is also available in the STM32Cube firmware package.")

How to use the FMAC peripheral to filter a noisy signal using DMA for input data buffer and interrupt for output data buffer.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Configures the FMAC peripheral and register the user callbacks.

__Step 2__: Preloads X2 with filter coefficients and configures the internal buffers for DMA input and interrupt-driven output.

__Step 3__: Starts the FMAC FIR filter processing with DMA input enabled.

__Step 4__: Checks the filtering results.

__Step 5__: Deinitializes the FMAC.

__End of example__: Reports the outcome of the data transfer via the variable **`ExecStatus`** and the **status LED** remains turned on in case of success.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Step 2: FMAC inputs and output configured.
[INFO] Step 3: FMAC started.
[INFO] Step 4: Calculated buffer equal reference buffer.
[INFO] Step 5: de-init
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

__FMAC__: is configured with the following parameters:

- The FMAC coefficient vector size is 5.
- The FMAC addresses and sizes used for X1, X2 and Y buffer are configured to not overlap:
  - 0 to 4: coefficient buffer;
  - 5 to 131: input buffer;
  - 132 to 254: output buffer;

__DMA__: is configured with the following parameters:

- The DMA read channel is used to transfer the input values from the input buffer to the FMAC. It is configured in Memory to Peripheral mode with an incremented source address and a fixed destination address:
- The data is loaded from the memory area to the FMAC input register incrementally.
- Since the FMAC buffers are uint16_t arrays, the DMA configuration is set with a halfword data width.
- The corresponding NVIC line is configured and enabled.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

JTAG/SWD probe may be used to check variables and buffers.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.
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

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube Firmware package.")

Here are the points of attention for this specific example:

- Take care of the DMA data width used, as the FMAC buffers' data are halfword size


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the Cube Firmware package.")

- [Application Note AN5305](https://www.st.com/resource/en/application_note/an5305-digital-filter-implementation-with-the-fmac-using-stm32cubeg4-mcu-package-stmicroelectronics.pdf): Digital filter implementation with the FMAC using STM32CubeG4 MCU Package

More information about the STM32 Cube Drivers can be found in the drivers' user manual of the STM32 series you are using.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
