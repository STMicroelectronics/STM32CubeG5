<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *example_hal_gtzc_uart_dma_secure_nonsecure*__

**Example version:** 0.0.1

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to receive a password over UART using circular DMA in non-secure context and validate it in secure context through a non-secure callable (NSC) service.


## __1. Detailed scenario__

The example is executed in the following order:
- Secure example execution: see the [secure README Detailed scenario](secure/README.md#1-detailed-scenario)
- Non-secure example execution: see the [non-secure README Detailed scenario](non_secure/README.md#1-detailed-scenario).


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

- Secure example configuration : see the [secure README Example configuration](secure/README.md#1-example-configuration)
- Non-secure example configuration : see the [non-secure README Example configuration](non_secure/README.md#1-detailed-scenario).


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

- Secure example: see the [secure README Generic Setup](secure/README.md#31-generic-setup)
- Non-secure example: see the [non-secure README Generic Setup](non_secure/README.md#31-generic-setup)

### __3.2. Specific board setups__

<details>
  <summary>Top level specific board setups</summary>

  <details>
    <summary>On STM32G5 series.</summary>

Please note the internal FLASH is fully secure by default.

Configure the Secure Watermark Option Bytes `SECWM1_STRT`/`SECWM1_END` and `SECWM2_STRT`/`SECWM2_END` according to the application configuration using `STM32CubeProgrammer`.

The Secure Watermark Option Bytes setup aligned with the project linker/scatter file is:

- `SECWM1_STRT=0x0`, `SECWM1_END=0x1F`: all Bank 1 pages are secure.
- `SECWM2_STRT=0x1`, `SECWM2_END=0x0`: no Bank 2 page is secure; Bank 2 is non-secure.
  </details>
</details>

<details>
  <summary>Secure project specific board setups</summary>

See the [secure README Specific board setups](secure/README.md#32-specific-board-setups)
</details>

<details>
  <summary>Non-secure project specific board setups</summary>

See the [non-secure README Specific board setups](non_secure/README.md#32-specific-board-setups)
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are some typical points of attention in case of issues.

### __4.1. General Debug Tips__

#### __4.1.1. Flashing issues__

1. Try flashing using `STM32CubeProgrammer`.
2. Check the device configuration against the description in the STM32 system memory boot mode application note.

#### __4.1.2 Startup issues__

1. Read the flash or RAM content using `STM32CubeProgrammer` to make sure the code you expect to run is loaded in the device.
2. Check the boot options of your device (boot pins...) in the reference manual.
3. Try connecting to the board and use a debugger to spot any hard fault in the code.

#### __4.1.3 Issues to connect to the board__

Possible issues are:

1. The firmware running on the target is forcing a low-power mode.
The communication with STLINK is disabled when the board is in low-power mode.

2. The target is secured or debug-disabled.

#### __4.1.4. HAL timing issues__

Care must be taken when using `HAL_Delay()`,
this function provides accurate delay (in milliseconds) based on variable incremented in SysTick ISR.

This implies that if `HAL_Delay()` is called from a peripheral ISR process,
then the SysTick interrupt must have higher priority (numerically lower) than the peripheral interrupt.
Otherwise, the caller ISR process is blocked.
To change the SysTick interrupt priority you have to use `HAL_NVIC_SetPriority()` function.

The example needs to ensure that the SysTick time base is always set to 1 millisecond to have correct HAL operation.

### __4.2 Fault Exceptions__

Arm Cortex®-M processors implement an efficient exception model that traps illegal memory accesses and
several incorrect program conditions.
The HardFault exception is always enabled and has a fixed priority (higher than other interrupts and exceptions,
but lower than non-maskable interrupt NMI). The HardFault exception is therefore executed in cases where a
fault exception is disabled or when a fault occurs during the execution of a fault exception handler.

All other fault exceptions (MemManage fault, BusFault, UsageFault...) have a programmable priority.
After reset, these exceptions are disabled and may be enabled in the system or application software
using the registers in the System Control Block (SCB).

### __4.3 Debug Toolbox__

Refer to the [STM32 debug toolbox](https://www.st.com/resource/en/application_note/an4989-stm32-microcontroller-debug-toolbox-stmicroelectronics.pdf)
for further information about how to troubleshoot the problem.

#### __4.4 Asking for support__

This software component is not yet open to public contribution.
Report any feedback to your ST contact point.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

The documentation of the drivers of the relevant STM32 series contains more detailed information.

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
