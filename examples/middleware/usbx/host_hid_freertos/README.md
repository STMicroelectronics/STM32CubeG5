<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo/>

# __Example: *usbx_host_hid_freertos*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure a USB human interface device (HID) host in RTOS mode using the USBX stack, enabling enumeration and communication with USB mice and keyboards.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

- __Step 1__: USB HCD initialization

- __Step 2__: Initialization of USBX stack.

- __Step 3__: Initialization of USBX HID Host Mouse and Keyboard Class.

__End of example__: After Step 3, the example is completed and the HID host is ready to interact with the device.

In case of successful operations:

- LED remains turned on
- Global variable `ExecStatus` set to EXEC_STATUS_INIT_OK

You can verify the USBX stack operates as expected:

- Plug the USB HID device into the STM32 board through 'Type C  to A-Female' cable to the connector:
    - Connect ST-Link cable to the PC USB port to display data on the HyperTerminal.
- In case of a mouse: - (x,y) mouse position - Wheel position - Pressed mouse buttons
- In case of a keyboard: - Pressed key

In case of failure:

- The status LED flashes infinitely
- Global variable `ExecStatus` set to EXEC_STATUS_ERROR


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the Cube Firmware package.")

This example uses the following peripheral(s):

__USB HCD OTG__:

The USB is configured with the following settings:

- Full-speed
- Embedded PHY Interface
- SOF Disabled

__UART__:

The UART is configured with the following settings:

- The baud rate is set to 115200.
- The word length is set to 8 bits.
- Stop bits are set to 1 bit.
- Parity is set to NONE.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

      - Make sure the STM32 board is powered.
      - External 5v injection for Vbus.
      - Configure the BaudRate to 115200 baud on the terminal.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32G5 series.</summary>
  <details>
    <summary>On board NUCLEO-G564RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PG3    |     GPIO      | MX_STATUS_LED |
  |    PA2    |   USART2_TX   |       -       |
  |    PA3    |   USART2_RX   |       -       |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube Firmware package.")

Here are the points of attention for this specific example:
  1. If there is an issue, remember to check that:
     - External 5v injection for Vbus
     - COM port number is correct
	 - Baudrate is set to 115200
  2. Depending on STM32 series, and USB instance used (FS/HS) specific USB configurations must be respected (Endpoint Max Packet Size, Clock)
     For details, refer to the reference manual of your MCU.
  3. If enumeration issues occur when building the project in Debug mode, increasing UX_HOST_ENUM_THREAD_STACK_SIZE may resolve the issue.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the Cube Firmware package.")

This [wiki](https://wiki.st.com/stm32mcu/wiki/Introduction_to_USBX)introduction to USBX.

More information about the STM32 Cube Drivers can be found in the drivers' user manual of the STM32 series you are using.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
