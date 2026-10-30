<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *usbx_device_custom_hid_freertos*__

**Example version:** 2.0.0

How to configure a USB Human Interface Device "HID" custom in RTOS mode based on USBX stack.

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the Cube Firmware package.")


## __1. Detailed scenario__

__Initialization phase__: At the beginning of the main program, `mx_system_init()` is called to initialize all the peripherals, initialize the Flash interface, the system clock, the systick and the instruction cache (2-ways set associative cache).

The application executes the following __example steps__:

- __Step 1__: USB PCD initialization

- __Step 2__: Initialization of USBX stack.

- __Step 3__: Initialization of USBX Custom HID Device Class.

__End of example__: After Step 3, the example is completed and the HID class is ready to interact with the host.

In case of successful operations:

- LED remains turned on
- Global variable `ExecStatus` set to EXEC_STATUS_INIT_OK

You can verify the USBX stack operates as expected:

- Start the "USB HID Demonstrator" PC applet, available for download from www.st.com,
- Plug the device running the Custom HID example into a USB port on the PC.
- The device should be detected and shown in the USB HID target box
- Press Graphic View button
- Select "use SET_FEATURE" or "use SET_REPORT" in order to use SET_REPORT request to send HID report
  for LED control
- Make sure that the following report IDs are configured: LED1 ID (0x1), LED2 ID(0x2), LED3 ID(0x3) and
  BUTTON1_ID(0x5).
- Select LEDs to switch on/off on the board: a SET_REPORT request will be sent.
- Press the user button on the device and observe the switch on/off of the button 1 ID.

In case of failure:

- LED toggle infinitely
- Global variable `ExecStatus` set to EXEC_STATUS_ERROR


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example uses the following peripheral:

__USB PCD OTG FS__: is configured as indicated below:

  - Full-speed operation
  - Embedded PHY Interface
  - SOF Disabled


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

      - Make sure the STM32 board is powered
      - Plug the STM32 device into a host PC via the USB connector.

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32G5 series.</summary>
  <details>
    <summary>On board NUCLEO-G564RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PG3    |     GPIO      | MX_STATUS_LED |
  |   PC13    |     GPIO      |          -    |

  </details>
</details>



## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube Firmware package.")

Here are the points of attention for this specific example:

  1. If there is an issue, remember to check that:
     - The STM32 is powered.
     - The STM32 USB connector is connected to a USB port in the host.
  2. Depending on STM32 series, and USB instance used (FS/HS) specific USB configurations must be respected (Endpoint Max Packet Size, Clock)

For details, refer to the reference manual of your MCU.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the Cube Firmware package.")

This [wiki](https://wiki.st.com/stm32mcu/wiki/Introduction_to_USBX) introduction to USBX.

More information about the STM32 Cube Drivers can be found in the drivers' user manual of the STM32 series you are using.

For instance, for the STM32V8 series: [User Manual](https://www.st.com).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
