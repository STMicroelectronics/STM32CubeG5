<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_fmac_fir_adc_dma*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/read/read_toc.html "An offline version is also available in the STM32Cube firmware package.")

How to use the FMAC peripheral to apply a five-tap low-pass FIR filter to ADC samples acquired at 1 kHz. ADC conversion data is transferred directly to the FMAC X1 input register using DMA, while a second DMA channel stores the filtered FMAC Y output samples into memory.
The FIR coefficients are calculated for a sampling frequency of 1 kHz. The filter was designed to achieve a -3dB cutoff frequency close to 130 Hz. In demo mode, a known voltage waveform can be applied to the ADC input pin, then the OutputSamples buffer can be compared with the expected low-pass FIR response computed from the configured coefficients, cutoff frequency, and sampling frequency.
The provided FIR coefficients are intended for demonstration purposes only. They are not necessarily optimal for every application and should be recalculated according to the required filtering characteristics, including sampling frequency, cutoff frequency, transition band, and attenuation requirements.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

- __Step 1__: Configures the FMAC peripheral, ADC, DMA and timer, then registers the FMAC and ADC user callbacks.

- __Step 2__: Starts and calibrates the ADC, then starts the timer used to trigger ADC conversions.

- __Step 3__: Preloads the FMAC X2 buffer with the FIR filter coefficients.

- __Step 4__: Gets the FMAC X1 input address and locks X1 write access so ADC DMA can write directly into the FMAC input buffer.

- __Step 5__: Starts the FMAC Y DMA transfer toward the output memory buffer.

- __Step 6__: Starts the FMAC FIR filter.

- __Step 7__: Starts ADC DMA toward FMAC X1 and waits until FMAC FIR filtering completes. The filtered output samples are stored in `OutputSamples`.

- __Step 8__: Deinitializes FMAC, ADC and timer peripherals.

__End of example__: Reports the outcome of the transfer through the variable **`ExecStatus`** and the **status LED** remains turned on in case of success.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: FMAC, ADC, DMA and timer initialization COMPLETED.
[INFO] Step 2: ADC start and calibration COMPLETED. TIM start COMPLETED.
[INFO] Step 3: FMAC X2 buffer preloaded with FIR filter coefficients.
[INFO] Step 4: FMAC X1 input address retrieved and X1 write access locked for ADC DMA.
[INFO] Step 5: FMAC Y DMA transfer configured toward the output buffer.
[INFO] Step 6: FMAC FIR filter started.
[INFO] Step 7: ADC DMA started toward FMAC X1 while TIM trigger paces conversions. FMAC FIR filtering completed and output samples are available in memory or OutputSamples.
[INFO] Step 8: Peripherals de-initialized: FMAC, ADC and timer.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the Cube Firmware package.")

__FMAC__: is configured with the following parameters:

- The FMAC coefficient vector size is 5.
- The FMAC addresses and sizes used for X2, X1 and Y buffer are configured to not overlap:
  - 0 to 4: coefficient buffer;
  - 5 to 131: input buffer;
  - 132 to 254: output buffer;

__ADC__: is configured with the following parameters:

- Single channel conversion.
- Trigger Timer:Timer is configured to provide a time base of 1kHz.
- DMA transfer.

__DMA__:

- ADC DMA is configured to transfer conversion data from ADC data register to the fixed FMAC X1 input register address returned by `HAL_FMAC_GetX1Address()`.
- FMAC read DMA is configured in peripheral-to-memory direction because `HAL_FMAC_ReadY_DMA()` reads filtered samples from FMAC Y output and stores them in `OutputSamples`.

__TIM__: Configured to trigger the ADC conversion with the following parameters:

  - Timer update event is used as an output trigger (TRGO).
  - Parameters have been chosen to get a 1kHz signal in the output of the TIM to trigger the ADC.

<details>
<summary>The aim is to provide a time base of 1kHz</summary>
  The time counter clock is set to 64KHz.

  The ADC conversion stream is paced by timer at 1kHz.

  *Note:* This frequency corresponds to the timer frequency.

  To get this 1kHz, with a 64KHz timer counter clock:

    ARR = (64 kHz / 1 kHz) - 1 = 63

  *Note:* The "+1" used in the formulas is needed because the registers' values start from 0.
</details>


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

JTAG/SWD probe may be used to check variables.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.
This example can run without external setup: in this case, ADC measures a floating voltage level and
conversion data are not deterministic.
To control voltage converted by ADC, apply a voltage level in range [0; VDDA] on pin ADCx_INy mentioned in table below.

<details>
  <summary>On STM32G5 series.</summary>
  <details>
    <summary>Common configuration.</summary>

  Timer's counter clock configuration with prescalers set to 1:

  - The timer's internal input clock (tim_ker_ck) is set to the system clock (SYSCLK).

      tim_ker_ck = SYSCLK (system clock)

  To obtain the timer's counter clock frequency (tim_cnt_ck), the timer prescaler register (TIM_PSC) is computed as follows:

      TIM_PSC = (SYSCLK / tim_cnt_ck ) - 1

  In this configuration:

- The HCLK is set to 280MHz.
- The timer counter clock is set to 64kHz.

To obtain a timer counter clock at 64kHz with prescalers set to 1 and the HCLK set to 280MHz, the timer prescaler must be:

    timer_prescaler = (280 MHz / 64 kHz) - 1 = 4374

  </details>

  <details>
    <summary>On board NUCLEO-G564RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PG3    |     GPIO      | MX_STATUS_LED |
  |    PA2    |   USART2_TX   |       -       |
  |    PA0    |   ADC1_INP1   |       -       |

  The example uses ADC1 on channel ADC1_INP1.

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube Firmware package.")

Here are the points of attention for this specific example:

- Take care of the DMA data width used, as the FMAC buffers' data are halfword size
- Ensure the ADC DMA destination address is the FMAC X1 input register address returned by `HAL_FMAC_GetX1Address()`.
- The ADC DMA destination address must remain fixed because each DMA request writes to the same FMAC input register.
- The timer trigger frequency defines the ADC sampling rate and must be compatible with the FMAC processing throughput.


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
