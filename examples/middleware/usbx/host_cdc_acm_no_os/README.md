<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *usbx_host_cdc_acm_no_os*__

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to configure a USB host CDC_ACM to enumerate and communicate with a CDC_ACM device in bare-metal mode using the USBX stack.

**Example version:** 2.0.0


## __1. Detailed scenario__

__Initialization phase__: At the beginning of the main program, `mx_system_init()` is called to initialize the peripherals, the Flash interface, the system clock, the SysTick, and the instruction cache (2-way set associative cache).

The application executes the following __example steps__:

__Step 1__: USB HCD initialization.

__Step 2__: Initialization of USBX stack.

__Step 3__: Initialization of USBX CDC_ACM Host Class.

__End of example__: After Step 3, the example is completed and the CDC_ACM host is ready to interact with the device.

In case of successful operations:

- LED remains turned on
- Global variable `ExecStatus` set to EXEC_STATUS_INIT_OK

You can verify the USBX stack operates as expected:

- Connect the ST-Link cable to the PC USB port to open the board debug terminal.
- Plug the USB CDC_ACM device into the board USB host connector through a Type-C to A-female cable.

  A first virtual COM port appears for the board debug terminal:
  - HyperTerminal configuration
    - Data Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - Flow control: None

  A second virtual COM port appears when the CDC_ACM device is enumerated:
  - HyperTerminal configuration
    - Data Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 9600 baud
    - Flow control: None

  Use the board user button to send data from the USB CDC_ACM host. On boards fitted with a joystick, this corresponds to the center button. Sent data is received and displayed on the terminal connected to the CDC_ACM device.

In case of failure:

- The status LED flashes infinitely
- Global variable `ExecStatus` set to EXEC_STATUS_ERROR
- Errors are detected such as (Enumeration Fail) and the corresponding message is displayed on the HyperTerminal.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example uses the following peripheral(s):

__USB HCD DRD__:

The USB is configured with the following settings:

- Full-speed
- Embedded PHY Interface

__UART__:

The board UART is configured with the following settings:

- The baud rate is set to 115200.
- The word length is set to 8 bits.
- Stop bits are set to 1 bit.
- Parity is set to NONE.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

- Make sure the STM32 board is powered.
- Provide the external 5 V injection required for VBUS.
- Connect the ST-Link interface to the PC and configure the board debug terminal to 115200 baud.
- Check the CDC device COM port number and configure the terminal connected to that device to 9600 baud.

### __3.2. Specific board setups__

<details>
  <summary>On STM32G5 series.</summary>
  <details>
    <summary>On board B-G564E-DPOW1.</summary>

  |  MCU pin  |  Signal name  |      User Label       |
  |:---------:|:-------------:|:---------------------:|
  |    PA5    |     GPIO      |     MX_STATUS_LED     |
  |    PA0    |     GPIO      | MX_EXAMPLE_EXTI_TRIGGER |
  |    PC15   |   UART5_TX    |           -           |

  </details>
</details>



## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube Firmware package.")

Here are the points of attention for this specific example:
  1. If there is an issue, remember to check that:
    - External 5 V injection for VBUS is present
    - The board debug COM port is correct and configured to 115200 baud
    - The CDC device COM port is correct and configured to 9600 baud
  2. Depending on STM32 series, and USB instance used (FS/HS) specific USB configurations must be respected (Endpoint Max Packet Size, Clock)
     For details, refer to the reference manual of your MCU.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the Cube Firmware package.")

This [wiki](https://wiki.st.com/stm32mcu/wiki/Introduction_to_USBX)introduction to USBX.

More information about the STM32 Cube Drivers can be found in the drivers' user manual of the STM32 series you are using.

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
