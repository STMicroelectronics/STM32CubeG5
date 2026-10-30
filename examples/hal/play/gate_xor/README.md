<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_play_gate_xor*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure the PLAY peripheral to act as a XOR gate. This example shows how to use the HAL drivers to configure the PLAY peripheral to perform a logical XOR operation on two input signals: PLAYi_INx external input and Software Trigger 0 input.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Configures, initializes, and starts the PLAY instance.

__Step 2__: Toggles the PLAY Software Trigger 0 logical level each second.
            The result of the XOR operation is outputted on the PLAYi_OUTy signal.
            Returns to step 2 indefinitely if no error occurs.

**_NOTE:_**: During this step, the user can change the PLAYi_INx input signal to see how the output changes accordingly.
             As the PLAY peripheral performs a logical XOR operation, the output will be high when one of the inputs is high and the other is low, and low when both inputs are either high or low.
             As the PLAY input is configured to be filtered by setting a minimal pulse width, the signal will be taken into account only if it is stable for a certain number of PLAY clock cycles, defined by the filter configuration.

__End of example__: The example run indefinitely or until an error occurs.

You can verify that the example runs properly via the status LED and the `ExecStatus` variable.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Step 2: Software trigger toggled.
[INFO] Step 2: Software trigger toggled.
...
```

![Timing diagram](doc/STMicroelectronics_example_hal_play_gate_xor_timing.svg)


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

The PLAY peripheral is configured to perform a logical XOR operation on two inputs: the PLAYi_INx external input and the Software Trigger 0 input. The external input is filtered by setting a minimum pulse width, so it must be stable for a certain number of PLAY clock cycles to be taken into account.
One lookup table (LUT) is used to implement the XOR operation.
The PLAYi_OUTy output signal is connected to this LUT and is used to output the result of the XOR operation on the PLAYi_OUTy pin.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

<!--
@startuml
@startditaa{doc/STMicroelectronics_example_hal_play_gate_xor.png}
                /------------------------------------------\
                |                                          |
                |      /--------------------------\        |
                |      |          PLAYi           |        |
                |      |                          |        |
                |      |     /---------------\    |        |
                |      |     |   XOR LUT     |    |        |
                |      |     |               |    |        |
  PLAYi_INx Pin *----------+-> Input 1       |    |        |
 External input |      |     |        Output *----------+->* PLAYi_OUTy Pin
                |      |   +-> Input 2       |    |        | External output
                |      |   | |               |    |        |
                |      |   | \---------------/    |        |
                |      |   |                      |        |
                |      |   |                      |        |
                |      |   | /---------------\    |        |
                |      |   | | SW Trigger    |    |        |
                |      |   | |  (toggling    |    |        |
                |      |   +-* each second)  |    |        |
                |      |     |               |    |        |
                |      |     \---------------/    |        |
                |      |                          |        |
                |      \--------------------------/        |
                |                                          |
                |               STM32 MCU                  |
                \------------------------------------------/
@endditaa
@enduml
-->

![hardware setup](doc/STMicroelectronics_example_hal_play_gate_xor.png)

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32G5 series.</summary>
  <details>
    <summary>On board NUCLEO-G564RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PG3    |     GPIO      | MX_STATUS_LED |
  |   PC14    | RCC_OSC32_IN  |       -       |
  |   PC15    | RCC_OSC32_OUT |       -       |
  |    PA2    |   USART2_TX   |       -       |
  |   PC13    |   PLAY1_IN4   |       -       |
  |   PD11    |  PLAY1_OUT0   |       -       |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Find below the points of attention for this specific example.

__Pins__: Make sure that the user signals are connected to the correct pin of the STM32 MCU.

__Signal stability__: The PLAYi_INx input signal is filtered by the PLAY peripheral by using a minimum pulse width configuration, so it must be stable for a certain number of PLAY clock cycles to be taken into account.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

The documentation of the drivers of the relevant STM32 series contains more detailed information.

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
