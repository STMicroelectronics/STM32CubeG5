<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *usbpd_drp_pd_freertos*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the Cube Firmware package.")

How to configure a USB Type-C Power Delivery DRP with FreeRTOS based on the USBPD stack.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

- __Step 1__: Initialization of USBPD stack: DPM and DPM User.
              This step initializes the USBPD stack (CAD and PE layers), and create tasks for the both CAD and PE layers.

- __Step 2__: Starts the FreeRTOS scheduler.
              The USBPD tasks are executed by the FreeRTOS scheduler.

__End of example__: This is an endless example that does not stop unless an error occurs.

In case of successful operations:

- Status LED remains turned on
- Global variable `ExecStatus` set to EXEC_STATUS_INIT_OK

- Connect the USB Type-C cable to the USB Type-C port of the board.
- USBPD trace is displayed in STM32CubeMonitor-UCPD when connecting or disconnecting a USB Type-C cable to the USB Type-C port of the board. This cable must be connected to a USB Type-C device, for example a source or a sink.

In case of failure:

- Status LED toggle infinitely
- Global variable `ExecStatus` set to EXEC_STATUS_ERROR

When using Keil(AC6), the usage of flag -fshort-enums is required for compatibility with USBPD library.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the Cube Firmware package.")

This example uses the following peripheral(s):

  - UCPD
  - GPIO
  - TIM
  - ADC
  - DMA
  - UART
  - I2C
  - EXTI

### __2.1. Block Diagram__

This example is based on the USBPD stack. The USBPD stack is defined with the following architecture:

![USBPD stack diagram](doc/USBPD_stack_diagram.svg)


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

Connect the USB Type-C cable to the USB Type-C port of the board. This cable must be connected to a USB Type-C source, or a legacy USB Type-A source with a USB Type-C to USB Type-A cable.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32G5 series.</summary>
  <details>
    <summary>On board B-G564E-DPOW1.</summary>

  |     MCU pin      |  Signal name  |  User Label   |
  |:----------------:|:-------------:|:-------------:|
  |       PA5        |     GPIO      | MX_STATUS_LED |
  |       PA8        |   UCPD1_CC1   |       -       |
  |       PA9        |   UCPD1_CC2   |       -       |
  |       PC15       |   UART5_TX    |       -       |
  |       PE2        |   I2C4_SCL    |       -       |
  |       PC7        |   I2C4_SDA    |       -       |
  |       PD12       |   ADC3_INP9   |       -       |
  |       PC13       |     GPIO      |       -       |
  | PH1-OSC_OUT(PH1) |     GPIO      |       -       |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube Firmware package.")

Find below the points of attention for this specific example.

__USBPD Trace__: STM32CubeMonitor-UCPD can used to debug the USBPD stack.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the Cube Firmware package.")

This [application note](https://www.st.com/resource/en/application_note/an5225-introduction-to-usb-typec-power-delivery-for-stm32-mcus-and-mpus-stmicroelectronics.pdf) and this [wiki article](https://wiki.st.com/stm32mcu/index.php?title=Introduction_to_USB_Power_Delivery_with_STM32&sfr=stm32mcu) explains further this USB Type-C Power Delivery feature.

More information can be found in the [STM32 solutions for USB Type-C and Power Delivery](https://www.st.com/content/st_com/en/ecosystems/stm32-usb-c.html) page.

The user manual of the drivers of the relevant STM32 series contains more detailed information.

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
