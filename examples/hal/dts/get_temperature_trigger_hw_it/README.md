<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_dts_get_temperature_trigger_hw_it*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the HAL DTS driver with a hardware trigger in interrupt mode. An LPTIM (low-power timer) periodically generates a hardware trigger internally connected to the DTS sensor trigger input. Each completed measurement raises a DTS end-of-measurement interrupt, and the application retrieves the measured temperature.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the MPU regions, the system clock, and the trace interface. The DTS and the LPTIM trigger source are initialized by the application.

The application executes the following __example steps__:

__Step 1__: Configures and initializes the DTS instance (end-of-measurement interrupt) and the LPTIM instance used as the trigger source, then registers the completion callback.

__Step 2__: Select the LPTIM compare event as the DTS hardware trigger source.

__Step 3__: Start the DTS sensor in interrupt mode. The sensor is armed and waits for the hardware trigger (no software start of conversion).

__Step 4__: Start the LPTIM channel output compare and counter, generating the hardware trigger that drives the DTS conversions.

__Step 5__: Acquire one hardware-triggered measurement. Completion is signaled by the DTS end-of-measurement interrupt callback, and the application retrieves and prints the measured temperature. While waiting for the trigger, the CPU sleeps (`__WFI()`), illustrating the benefit of interrupt mode.

__Step 6__: Stop the LPTIM trigger source (counter then channel) and the DTS sensor to release resources.

__End of example__: After step 6, the example is completed. The execution status indicates whether the example ran successfully.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Step 2: DTS hardware trigger source selected.
[INFO] Step 3: DTS sensor armed in interrupt mode.
[INFO] Step 4: LPTIM hardware trigger source started.
[INFO] Step 5: Temperature measurement acquired: 25 degC.
[INFO] Step 6: DTS sensor and LPTIM trigger stopped.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

__DTS configuration__:

The DTS peripheral is configured to measure the internal die temperature:

- DTS clock enabled from the corresponding bus.
- DTS reference clock selected as PCLK. The DTS also supports LSE as a reference-clock source, but LSE is not used in this example.
- DTS sensor enabled.
- Conversion mode: single conversion triggered by a __hardware trigger__ (an LPTIM channel compare event).
- End-of-measurement __interrupt__ enabled; the DTS global interrupt is enabled in the NVIC.
- Conversion result expressed in Celsius using the HAL conversion helper.
- Sampling time configured to acquire the temperature.
- The application prints the latest measured temperature; the check against the normal operating range is performed by the test suite.
- The STM32 product line provides factory calibration values for the DTS, these values are used (when available) to improve the accuracy of the reported temperature.

__LPTIM configuration (hardware trigger source)__:

- LPTIM clock enabled.
- The LPTIM clock is independent from the DTS reference clock and is selected according to the target configuration.
- The LPTIM prescaler must be selected so that the LPTIM counter clock is below 1 MHz, as required for DTS temperature calibration.
- Free-running (continuous) mode with an output compare channel.
- The channel compare event is routed internally to the DTS trigger input; no GPIO pin is required.
- The prescaler and period define the temperature sampling rate.
- The hardware-trigger frequency is calculated as `LPTIM clock frequency / (prescaler * period)`, which defines the DTS temperature sampling rate.

<details>
<summary>How the hardware-trigger frequency is obtained</summary>

  The LPTIM output compare event periodically triggers a DTS measurement. Its frequency defines the DTS temperature sampling rate.

  The LPTIM counter clock (lptim_cnt_ck) is derived from the LPTIM kernel clock through the prescaler:

    lptim_cnt_ck = lptim_ker_ck / prescaler

  The hardware-trigger frequency (trigger_freq) is then obtained from the period register:

    trigger_freq = lptim_cnt_ck / period = lptim_ker_ck / (prescaler * period)

  *Note:* The LPTIM counter clock (lptim_cnt_ck) must remain below 1 MHz, as required for DTS temperature calibration.
</details>

> __Note__: The LPTIM peripheral and the DTS hardware-trigger / NVIC settings are reflected in the generated code and CMake build. Regenerate the project from the provided `.ioc2` in STM32CubeMX to refresh all IDE project files (EWARM, open-CMSIS, CMake) consistently with this configuration.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No specific hardware setup needed for this example.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32G5 series.</summary>
  <details>
    <summary>Common configuration.</summary>

  On the STM32 G5 family, the LPTIM trigger reaches the DTS through an internal interconnect, so no external wiring is required.

  LPTIM counter clock configuration:

  - The LPTIM kernel clock (lptim_ker_ck) is selected from PCLK3, derived from the system clock according to the RCC bus configuration.

      lptim_ker_ck = PCLK3

  To obtain the LPTIM counter clock (lptim_cnt_ck), the LPTIM prescaler is applied:

      lptim_cnt_ck = lptim_ker_ck / prescaler

  In this configuration:

  - RCC clock is set to 280 MHz.
  - LPTIM uses a division factor of 128.
  - The period register is set to 781.

  With these values, the LPTIM counter clock is:

      lptim_cnt_ck = 280 MHz / 128 = 2.1875 MHz

  This does not satisfy the below-1-MHz calibration requirement; the APB divider must be increased (or a slower LPTIM kernel source selected) before generating the final project.

  The resulting hardware-trigger frequency is:

      trigger_freq = 280,000,000 / (128 * 781) = approximately 2.799 kHz

  This corresponds to one DTS measurement every `0.357 ms`.

  </details>
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

Related DTS examples:

- [hal_dts_get_temperature_trigger_sw](../get_temperature_trigger_sw): get the DTS temperature using a software trigger.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
