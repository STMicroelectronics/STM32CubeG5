<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_tim_one_pulse*__

**Example version:** 2.0.6

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure TIM peripheral to generate a single pulse when a rising edge of an external signal is received on the dedicated TIMER input pin.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Initializes the input clock, counter clock, output clock, output channels, and GPIO pins for the timer.

__Step 2__: Starts the timer to generate a pulse signal when an external trigger occurs.

__End of example__: If no error occurs, the one pulse signal is generated for each trigger on input TIMER channel.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

### __2.1. Timer configuration:__

The *TIM* is configured as follows:

- The timer channel is configured as pulse generator in up counting mode.
- The timer prescaler is configured to set the timer counter clock to 1 MHz.
- The pulse is configured to 0.5 milliseconds.
- The delay is configured to 0.5 milliseconds.

The system clock configuration is specific to each STM32 MCU (see section [Hardware environment and setup](#3-hardware-environment-and-setup)).

__One pulse output configuration:__

The generated pulse is defined by its delay between the pulse and the trigger, and the pulse length. Each of these values depend on the timer counter clock (tim_cnt_clk).

The delay between the trigger and the pulse generation depends on the CCR register as follows:

    Delay = CCRx / tim_cnt_clk frequency

The duration of the pulse can be determined using the following formula:

    Pulse = (ARR + 1 - CCRx) / tim_cnt_clk frequency

  ![one_pulse_mode_timing](doc/one_pulse_mode_timing.svg)

<details>

  <summary>Numerical calculations</summary>

  The timer's counter clock is set to 1MHz (see prescaler computation in section [Hardware environment and setup](#3-hardware-environment-and-setup)).

  To set a delay of 0.5 milliseconds:

    CCRx = delay * tim_cnt_clk frequency
         = 0.0005 * 1000000
         = 500

  To set a pulse width of 0.5 milliseconds:

    ARR = CCRx + (pulse * tim_cnt_clk frequency) - 1
        = 500 + (0.0005 * 1000000) - 1
        = 999

</details>

__GPIO__:

We configure the GPIO pin as a timer channel pin, thanks to the appropriate alternate function.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

The timer configuration depends on the timer's input clock.
This clock is derived from the system clock tree.
So, the system clock configuration is a critical setup step.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.
This example can run without external setup: in this case, the timer (TIM) can control the signal on the TIMx_CHy pin connected to the 3.3V pin.

<details>
  <summary>On STM32G5 series.</summary>
  <details>
    <summary>Common configuration.</summary>

  The external signal is connected to the TIM1_CH2, and a rising edge on this input is used to trigger the timer.

  The one-pulse signal is output on the TIM1_CH1.

  - The timer's internal input clock (tim_ker_ck) also named bus clock (bus_ck) depends on the system clock (sys_clk) according to this formula:
      bus_clk = sys_clk / HPRE
      With HPRE the Bus Matrix clock prescaler

      So, tim_ker_ck = sys_clk / HPRE in Hz

  To obtain the timer's counter clock frequency (tim_cnt_ck), the timer prescaler register (TIM_PSC) is computed as follows:

      TIM_PSC = ( (sys_clk / HPRE) / tim_cnt_ck ) - 1

  Standard STM32G5xx timer clock distribution diagram:
    <!--
@startuml
@startditaa{doc/stm32g5_peripherals_clocks.png}

               +---------+
  sys_clk      |  HPRE  |      tim_ker_ck
------------+->+ / 1,2,4 +-----------------+->
               +---------+       To TIMx

@endditaa
@enduml
-->
  ![clocks](doc/stm32g5_peripherals_clocks.png)

In this configuration:

- The sys_clk is set to 280 MHz.
- The HPRE prescaler is set to 2 so the tim_ker_ck = 280 MHz.

As we want the timer counter clock equal to 1 MHz, the timer prescaler must be:

      timer_prescaler = (280 MHz / 1 MHz) - 1 = 279

  </details>

  <details>
    <summary>On board NUCLEO-G564RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PG3    |     GPIO      | MX_STATUS_LED |
  |    PC6    |   TIM8_CH1    |       -       |
  |    PB6    |   TIM8_CH2    |       -       |

  </details>
</details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

__System clock__: The timer clock depends on the system clock configuration. Changing the CPU clock or the peripheral bus' clock affects the frequency.

__Trigger as input__: The pulse signal needs a trigger to be generated. If the signal is not generated, be sure that there is a trigger signal provided as input on the input timer channel.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

This [General-purpose timer cookbook for STM32 microcontrollers (ref. AN4776)](https://www.st.com/content/ccc/resource/technical/document/application_note/group0/91/01/84/3f/7c/67/41/3f/DM00236305/files/DM00236305.pdf/jcr:content/translations/en.DM00236305.pdf) provides a simple and clear description of the basic features and operating modes of the STM32 general-purpose timer peripherals.

This [STM32 cross-series timer overview (ref. AN4013)](https://www.st.com/content/ccc/resource/technical/document/application_note/54/0f/67/eb/47/34/45/40/DM00042534.pdf/files/DM00042534.pdf/jcr:content/translations/en.DM00042534.pdf) presents an overview of the timer peripherals for the STM32 product series.

More information about the STM32Cube Drivers can be found in the drivers' user manual of the STM32 series you are using.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
