<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *rgbled_multi_color_set*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the Cube firmware package.")

This example shows how to play infinitely non-primary vivid colors using the RGBLED part PWM API.
It illustrates it by driving the on-board RGB LED through three PWM timer channels.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

 __Step 1__: Retrieves the generated RGBLED object and initializes it with the `rgbled_init()` function.

__Step 2__: Cycle indefinitely through the following colors using the `rgbled_set_color()` function, with a 500 ms delay between updates.
The brightness levels for the red, green, and blue channels are expressed as percentages, as shown below:

- White:   `{30%, 30%, 30%}`
- Magenta: `{35%, 00%, 30%}`
- Yellow:  `{35%, 30%, 00%}`
- Cyan:    `{00%, 30%, 30%}`
- Off:     `{00%, 00%, 00%}`

__End of example__: The RGB LED repeats this 2.5 s color sequence indefinitely if no error occurs.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the Cube firmware package.")

The generated board configuration must provide the following RGBLED resources:

- One PWM-capable timer channel for the red component
- One PWM-capable timer channel for the green component
- One PWM-capable timer channel for the blue component

The following peripherals are configured by the generated code:

- *GPIO*:
  - The RGB LED pins are configured in alternate-function mode to expose the timer PWM outputs.
  - No pull-up or pull-down resistors are enabled on the RGB LED channels.
  - The GPIO speed is set to low frequency for the RGB LED channels pin.
- *TIM*:
  - The timers selected for the RGB LED channels are initialized in up-counting PWM mode.
  - Each RGB channel uses a dedicated compare unit.
  - The output polarity is configured as active high.
  - The timers are initialized as free-running 32-bit counters and the RGBLED driver updates the duty cycle on each channel at runtime.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

No external wiring is required for the RGB LED demonstration on the `B-G564E-DPOW1` board.
The example drives the on-board RGB LED reference `LD1`.

### __3.2. Specific board setups__

<details>
  <summary>On STM32G5 series.</summary>
  <details>
    <summary>On board B-G564E-DPOW1.</summary>

  |  MCU pin  |  Signal name  |  User Label  |
  |:---------:|:-------------:|:------------:|
  |    PC6    |   TIM3_CH1    | RGBLED Red   |
  |   PD11    |   TIM4_CH2    | RGBLED Green |
  |    PB7    |   TIM2_CH1    | RGBLED Blue  |
  |    PA5    |     GPIO      | Status LED   |

- With `SYSCLK = 280 MHz`, each RGB LED channel outputs a 250 kHz PWM signal.

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube firmware package.")


- If the RGB LED does not change color, check that the generated bindings still map the RGB channels to `PC6`, `PD11`, and `PB7`.
- If the application exits its main loop because of an error, the status LED blinks briefly every 2 seconds.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the Cube firmware package.")

This [Application note AN2841](https://www.st.com/content/ccc/resource/technical/document/application_note/64/4d/e3/1d/b3/34/4f/0b/CD00213809.pdf/files/CD00213809.pdf/jcr:content/translations/en.CD00213809.pdf)
gives a detailed example of LED dimming implemented on STM32 microcontroller in combination with the STP24DP05 LED driver.

You can also refer to these other examples:

- hal_gpio_toggle to toggle any GPIO with HAL API (the GPIO can be connected to an LED)
- hal_tim_pwm_output shows how to configure the TIM peripheral in PWM (Pulse Width Modulation) mode.
- led_blinking_gpio shows how to drive an LED using the LED GPIO part driver.

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
