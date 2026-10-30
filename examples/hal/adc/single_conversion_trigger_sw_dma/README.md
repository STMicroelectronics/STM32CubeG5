<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_adc_single_conversion_trigger_sw_dma*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use an ADC peripheral to perform a single ADC conversion on a channel triggered by software. Converted data is transferred by DMA.
This example is based on the ADC HAL API.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: The ADC is configured to convert a single channel, in single conversion mode, from a software trigger.

__Step 2__: The ADC autocalibration is performed to ensure accurate measurements. The calibration is a prerequisite
to any ADC operation. It removes the systematic errors that may vary from chip to chip.

__Step 3__: The ADC conversion is started in DMA mode. The conversion is triggered by software.
The ADC conversion data is transferred by DMA into the variable "AdcRawData".

__Step 4__: Computation of the ADC conversion raw data to physical value. Prints the ADC raw value and the ADC computed corresponding voltage.

__End of example__: The ADC conversion is repeated endlessly (step 3 and step 4 are running in a loop).
The example status is reported via the variable **`ExecStatus`**, and the status LED
remains turned on in case of success.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: ADC initialization COMPLETED.
[INFO] Step 2: ADC start and calibration COMPLETED.
[INFO] Step 3: ADC start conversion COMPLETED.
[INFO] Step 4: ADC conversion DONE.  Raw ADC value = 0x0   Voltage = 0 mV
[INFO] Step 3: ADC start conversion COMPLETED.
[INFO] Step 4: ADC conversion DONE.  Raw ADC value = 0xfff   Voltage = 3300 mV
...
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

__ADC__: is configured with the following parameters:

- Single channel conversion.
- Single conversion mode.
- Software trigger.
- DMA transfer.

__DMA__:

Configured to transfer data from the ADC to a memory variable upon ADC request. The data is stored with a fixed source address (the ADC data register) and a fixed destination address.
The variable containing the ADC conversion value is AdcRawData.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

A JTAG/SWD probe may be used to check the variables.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.
This example can run without external setup: in this case, the ADC measures a floating voltage level and
the conversion data are not deterministic.
To control the voltage converted by the ADC, apply a voltage level in range [0; VDDA] on the pin ADCx_INy mentioned in the table below.

<details>
  <summary>On STM32G5 series.</summary>
  <details>
    <summary>On board NUCLEO-G564RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PG3    |     GPIO      | MX_STATUS_LED |
  |    PA2    |   USART2_TX   |       -       |
  |    PA0    |   ADC1_INP1   |       -       |

  The example uses ADC1 on channel ADC1_INP1 (PA0).

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

1. Take care of data misalignment:
Depending on the DMA data width used, source and destination addresses must respect data alignment.
Please refer to the reference manual of your MCU.

2. Take care of DMA ports:
Depending on the STM32 series, and on the DMA instance used (GPDMA/HPDMA/LPDMA), specific DMA ports constraints must be respected.
For details, refer to the reference manual of your MCU.

__Warning__: When debugging the ADC, any read to the Data Register (DR) clears the End Of Conversion flag (EOC).\
For instance, debugging step by step with any register inspection view might break the HAL/example behavior.

The __ADC conversion precision__ is highly dependent on the sampling time, which should be adjusted according to the input signal characteristics.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

- [Application Note AN2834](https://www.st.com/content/ccc/resource/technical/document/application_note/group0/3f/4c/a4/82/bd/63/4e/92/CD00211314/files/CD00211314.pdf/jcr:content/translations/en.CD00211314.pdf): How to get the best ADC accuracy in STM32 microcontrollers

- [Application Note AN3116](https://www.st.com/content/ccc/resource/technical/document/application_note/c4/63/a9/f4/ae/f2/48/5d/CD00258017.pdf/files/CD00258017.pdf/jcr:content/translations/en.CD00258017.pdf): STM32's ADC modes and their applications

- [Application Note AN4195](https://www.st.com/resource/en/application_note/an4195-stm32f30x-adc-modes-and-application-stmicroelectronics.pdf): STM32F30x ADC modes and application

- [Application Note AN5346](https://www.st.com/content/ccc/resource/technical/document/application_note/group1/02/ba/86/7e/6c/d7/4e/08/DM00625282/files/DM00625282.pdf/jcr:content/translations/en.DM00625282.pdf): STM32G4 ADC use tips and recommendations

- [Application Note AN2668](https://www.st.com/content/ccc/resource/technical/document/application_note/c5/24/7d/f6/98/7f/4c/f3/CD00177113.pdf/files/CD00177113.pdf/jcr:content/translations/en.CD00177113.pdf): Improving STM32F1 series, STM32F3 series and STM32Lx series ADC resolution by oversampling

- [Application Note AN4073](https://www.st.com/content/ccc/resource/technical/document/application_note/a0/71/3e/e4/8f/b6/40/e6/DM00050879.pdf/files/DM00050879.pdf/jcr:content/translations/en.DM00050879.pdf): How to improve ADC accuracy when using STM32F2xx and STM32F4xx microcontrollers

- [Application Note AN4629](https://www.st.com/content/ccc/resource/technical/document/application_note/33/e1/e4/5c/aa/67/4c/74/DM00150423.pdf/files/DM00150423.pdf/jcr:content/translations/en.DM00150423.pdf): ADC hardware oversampling for microcontrollers of the STM32L0 and L4 series

- [Application Note AN5354](https://www.st.com/content/ccc/resource/technical/document/application_note/group1/11/72/be/05/cd/94/44/5b/DM00628458/files/DM00628458.pdf/jcr:content/translations/en.DM00628458.pdf): Getting started with the STM32H7 series MCU 16-bit ADC

- [Application Note AN5593](https://www.st.com/resource/en/application_note/an5593-how-to-use-the-gpdma-for-stm32-mcus-stmicroelectronics.pdf): How to use the GPDMA for STM32 MCUs (DMA port allocation)

- [Training](https://www.st.com/content/ccc/resource/training/technical/product_training/1e/0f/65/34/7e/b0/4e/ca/STM32L4_Analog_ADC.pdf/files/STM32L4_Analog_ADC.pdf/jcr:content/translations/en.STM32L4_Analog_ADC.pdf): STM32L4-Analog-ADC(ADC)

- [Training](https://www.st.com/content/ccc/resource/training/technical/product_training/group0/0b/e4/af/01/4a/92/44/dc/STM32F7_Analog_ADC/files/STM32F7_Analog_ADC.pdf/jcr:content/translations/en.STM32F7_Analog_ADC.pdf): STM32F7_Analog_ADC

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
