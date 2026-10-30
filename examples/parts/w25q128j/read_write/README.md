<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *w25q128j_read_write*__

**Example version:** 2.0.6

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the W25Q128J SPI flash memory using "read/write" APIs.
This example illustrates the process of writing and reading data from the SPI flash memory.


## __1. Detailed scenario__

This scenario demonstrates how to read/write to a W25Q128J SPI flash memory with the w25q128j part driver APIs.

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: Initializes the w25q128j part.

__Step 2__: Erases the sector of the memory in which we want to write.

__Step 3__: Writes a data buffer to the flash memory.

__Step 4__: Reads the same data buffer from the flash memory, and checks that it matches the buffer written in step 3.

__Step 5__: Deinitializes the W25Q128J part.

__End of example__: After step 5, the example is completed. You can verify that the example runs properly via the status LED and the `ExecStatus` variable.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Device initialization COMPLETED.
[INFO] Step 2: Sector erased.
[INFO] Step 3: Data buffer written to the flash memory.
[INFO] Step 4: Data buffer read from the flash memory and matching the initial value.
[INFO] Step 5: Device de-initialization.
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

__W25Q128J__: The NOR flash memory is configured with this specific parameter:

- A CS pin configured as output to be selected in the part configuration panel.

__SPI__: The SPI is configured with these specific parameters:

- Mode set to "HAL_SPI_MODE_MASTER"
- Direction set to "HAL_SPI_DIRECTION_FULL_DUPLEX"
- Data width mode set to "HAL_SPI_DATA_WIDTH_8_BIT"
- Clock polarity set to "HAL_SPI_CLOCK_POLARITY_LOW"
- Clock phase set to "HAL_SPI_CLOCK_PHASE_1_EDGE"
- Baudrate prescaler set to "HAL_SPI_BAUDRATE_PRESCALER_256"
- First bit set to "HAL_SPI_MSB_FIRST"
- nss pin management set to "HAL_SPI_NSS_PIN_MGMT_INTERNAL"


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

<!--
@startuml
@startditaa{doc/generic_hardware_setup.png}
  +-------------------------+                     +-------------------------+
  |          +--------------+                     |                         |
  |          |   STM32 SPIi |                     |                         |
  |          |              |                     |                         |
  |          |          SCLK *---------------------* SCLK                   |
  |          |              |                     |                         |
  |          |          MOSI *---------------------* MOSI                   |
  |          |              |                     |                         |
  |          |         MISO *---------------------* MISO                    |
  |          |              |                     |                         |
  |          |          CS  *---------------------* CS#                     |
  |          |              |                     |                         |
  |          |              |                     |                         |
  |          +--------------+                     |                         |
  |                         |                     |                         |
  |                         |                     |                         |
  | STM32 MCU               |                     |       W25Q128J          |
  +-------------------------+                     +-------------------------+
@endditaa
@enduml
-->

![generic_hardware_setup](doc/generic_hardware_setup.png)

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<details>
  <summary>On STM32G5 series.</summary>
  <details>
    <summary>On board NUCLEO-G564RE.</summary>

  |  MCU pin  |  Signal name  |  User Label  |
  |:---------:|:-------------:|:------------:|
  |    PA2    |   USART2_TX   |      -       |
  |    PG3    |   SPI1_SCK    |      -       |
  |   PC11    |   SPI1_MISO   |      -       |
  |    PB2    |   SPI1_MOSI   |      -       |
  |   PB15    |     GPIO      |      -       |

  The W25Q128J NOR Flash memory supports up to 133 Mhz except for Read Data operation which supports only up to 50 Mhz.. For this example, the SPI1 clock is set to 2Mhz.

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

The documentation of the drivers of the relevant STM32 series contains more detailed information.

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
