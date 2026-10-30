<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_dts_get_temperature_trigger_sw*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the HAL DTS driver in polling mode to start a temperature measurement on DTS sensor 1, wait for the conversion completion, read back the measured temperature, and then stop the sensor. The example reports execution steps and the measured temperature through a trace interface and an execution status variable.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the DTS peripheral, MPU regions, and the system clock.

The application executes the following __example steps__:

__Step 1__:  Configures and initializes the DTS instance.

__Step 2__: Start the DTS sensor 1.

__Step 3__: Wait for measurement completion (polling with timeout).

__Step 4__: Read the measured temperature value.

__Step 5__: Stop the DTS sensor.

__Step 6__: DTS peripheral deinitialization.

__End of example__: After step 6, the example is completed. You can verify that the example runs properly via the status LED and the `ExecStatus` variable.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO]  Step 1: Device initialization COMPLETED.
[INFO]  Step 2: DTS sensor start operation COMPLETED.
[ERROR] Step 2: DTS sensor start operation ERROR
[INFO]  Step 3: DTS temperature measurement READY.
[ERROR] Step 3: DTS temperature measurement NOT AVAILABLE within the expected timeout.
[INFO]  Step 4: DTS temperature value RETRIEVED: x degree Celsius.
[ERROR] Step 4: DTS temperature value NOT RETRIEVED.
[INFO]  Step 5: DTS sensor stop operation COMPLETED.
[ERROR] Step 5: DTS sensor stop operation NOT COMPLETED.
...
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

__DTS configuration__:

The DTS peripheral is configured to measure the internal die temperature using sensor 1:

- DTS clock enabled from the corresponding bus.
- DTS sensor 1 enabled.
- Conversion mode: single conversion (one-shot) triggered by software.
- Conversion result expressed in Celsius using the HAL conversion helper.
- Sampling time of 15 ADC clock cycles (HAL_DTS_SAMPLING_TIME_15_CYCLES) to acquire the temperature.
- The STM32 product line provides factory calibration values for the DTS, these values are used (when available) to improve the accuracy of the reported temperature.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No specific hardware setup needed for this example.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<!-- YOUR BOARDS ADDED HERE BY README GENERATION -->
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

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
