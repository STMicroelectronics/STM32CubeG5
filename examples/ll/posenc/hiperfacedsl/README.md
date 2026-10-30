<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *ll_posenc_hiperfacedsl*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure and use the POSENC peripheral with the LL API to communicate with a HiperfaceDSL encoder. The example configures the POSENC with HiperfaceDSL protocol, starts the Hiperface DSL link, checks the link status, and reads the encoder turn and angular position.
This example requires an HiperfaceDSL compatible encoder connected to the POSENC interface of the STM32 board.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: The application initializes the POSENC and timer instances.

__Step 2__: The application selects the HiperfaceDSL encoder protocol, enables the HiperfaceDSL fast-position interrupt, and starts the HiperfaceDSL core.

__Step 3__: The application starts the timer that generates the POSENC strobe signal. The timer is configured to generate a trigger output on each update event, which is connected to the POSENC strobe input.

__Step 4__: The application waits for the HiperfaceDSL communication to be established. It checks the link status flag from the HiperfaceDSL core.

__Step 5__: At each execution loop, the application waits for the POSENC interrupt callback to report new fast-position data, then reads the online status words and the HiperfaceDSL `FPOSMT1` and `FPOSMT2` registers through DATA0 and DATA1. Then it updates the `online_status_d`, `online_status_1`, `online_status_2`, `encoder_turn`, and `encoder_position_degree` variables.
            Returns to step 5 indefinitely if no error occurs.

__End of example__: This is an endless example. If an error occurs, the application exits the execution loop and enters the error handler.

You can verify that the example runs properly via the status LED and the `ExecStatus` variable. When initialization succeeds, the status LED remains on. If an error occurs, the status LED flashes with a short on time and a long off time.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

__POSENC__:

The POSENC peripheral is configured to communicate with a HiperfaceDSL encoder:

  - POSENC instance: `POSENC1`
  - protocol: HiperfaceDSL
  - initialization layer: LL
  - strobe resynchronization: enabled
  - trigger source: `TIM1_TRGO`
  - POSENC1 kernel clock: 75 MHz from `PLL2P`
  - interrupt: enabled for fast-position availability

The POSENC clock source is `PLL2P`, configured to provide the 75 MHz POSENC1 kernel clock required by HiperfaceDSL. The applicative code selects HiperfaceDSL mode, enables fast-position notifications, and starts the HiperfaceDSL core in free-running mode.

__TIM__:

The timer is configured as the POSENC synchronization trigger source:

  - timer instance: `TIM1`
  - initialization layer: LL
  - trigger output source: update event

TIM1 is initialized from the generated project configuration and started before the application begins the HiperfaceDSL read loop.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.
Connect an HiperfaceDSL encoder to the dedicated POSENC interface of the STM32 board. The power supply must be provided externally through the POSENC interface.
Refer to the board schematic and the encoder electrical requirements to ensure that the external encoder power supply and signal levels are compatible.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32G5 series.</summary>
  <details>
    <summary>On board B-G564E-MC1S.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PH0    |  RCC_OSC_IN   |       -       |
  |    PH1    |  RCC_OSC_OUT  |       -       |
  |   PF14    |  POSENC1_EN   |      DIR      |
  |   PF12    |  POSENC1_IN   |      RX       |
  |   PF11    |  POSENC1_OUT  |      TX       |
  |   PA11    |     GPIO      | MX_STATUS_LED |

  &gt; **_NOTE:_** Connect the encoder interface according to the board schematic and the encoder electrical requirements. Ensure that the external encoder power supply and signal levels are compatible with the STM32G5 POSENC pins.

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Find below the points of attention for this specific example.

__Encoder connection__: Check that the HiperfaceDSL encoder signals are connected to the expected POSENC pins and that the encoder is powered before starting the example.

__No new data received__: If the `new_data_received` variable remains cleared, check the POSENC interrupt configuration, the TIM1 trigger output, and the HiperfaceDSL signal wiring.

__Invalid position value__: If `position_degree` or `turn` does not update as expected, check the `MASTER_QM` link status, the DATA0/DATA1 read completion flags, and that the HiperfaceDSL communication clock is suitable for the setup.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For the STM32G5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32g5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
