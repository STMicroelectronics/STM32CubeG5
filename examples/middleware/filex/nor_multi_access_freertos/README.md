<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *filex_nor_multi_access_freertos*__

**Example version:** 2.0.3

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

This application provides an example of FileX stack usage.

It demonstrates the FileX's concurrent file access capabilities. The application is designed to execute file operations on the NOR flash device. The code provides all required software code for handling NOR flash I/O operations.

The application starts by initializing the FileX system and creating threads to handle concurrent file operations. At this stage, all FileX resources are created, the NOR flash driver is initialized, and the application creates 3 threads with the same priorities:

- main_task used to initialize the NOR flash driver and opening it as a FileX Media.
- fx_thread_one used to create, write, and read operations for file fx_file_one.
- fx_thread_two used to create, write, and read operations for file fx_file_two.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as NOR flash memory), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

- __Step 1__: Creates an event and 3 tasks with the same priority.
- __Step 2__: Starts the FreeRTOS scheduler.
- __Step 3__: Use 'task entry' to initialize the NOR flash driver and open it as a FileX Media.
- __Step 4__: Use 'task one' to perform create, write, and read operations on the file fx_file_one.
- __Step 5__: Use 'task two' to execute creation, writing, and reading operations for the file fx_file_two.
- __Step 6__: Complete the scenario by ensuring that the media is properly closed.


__End of example__: After Step 6, the example is completed. You can check its status via the status LED.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

- *NOR*: is configured with the following features and settings:
- Write and erase operations enabled
- Optimized for concurrent access
- Configured for high-speed operations
- Selected GPIO pins support the NOR alternate function. They are configured in push-pull mode with no pull-up or pull-down.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

```text
  +-------------------------+                     +-------------------------+
  |          +--------------+                     |                         |
  |          | STM32 SPIi   |                     |                         |
  |          |              |                     |                         |
  |          |         SCLK *---------------------* SCLK                    |
  |          |              |                     |                         |
  |          |         MOSI *---------------------* MOSI                    |
  |          |              |                     |                         |
  |          |         MISO *---------------------* MISO                    |
  |          |              |                     |                         |
  |          |           CS *---------------------* CS#                     |
  |          |              |                     |                         |
  |          |              |                     |                         |
  |          +--------------+                     |                         |
  |                         |                     |                         |
  | STM32 MCU               |                     |       W25Q128J          |
  +-------------------------+                     +-------------------------+
```

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.



<details>
  <summary>On STM32G5 series.</summary>
  <details>
    <summary>On board NUCLEO-G564RE.</summary>

  |  MCU pin  |  Signal name  |  User Label  |
  |:---------:|:-------------:|:------------:|
  |    PA2    |   USART2_TX   |     PA2      |
  |    PB2    |   SPI1_MOSI   |     PB2      |
  |   PB15    |     GPIO      |      -       |
  |   PC11    |   SPI1_MISO   |     PC11     |
  |   PC13    |     GPIO      |      -       |
  |    PG3    |   SPI1_SCK    |     PG3      |

  On the NUCLEO-G564RE board, the external W25Q128J memory is connected via the SPI interface. The W25Q128J NOR Flash memory supports up to 133 MHz except for Read Data operation, which supports up to 50 MHz. For this example, SPI1 is clocked from PCLK2 (140 MHz) with a prescaler of 32, which sets the SPI1 clock to 4.375 MHz.

  </details>
</details>




## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS - IS.
