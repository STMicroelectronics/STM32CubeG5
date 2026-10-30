<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *levelx_nor_multi_access_freertos*__

**Example version:** 2.0.3

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

This application provides an example of LevelX stack usage with NOR flash and FreeRTOS.

It demonstrates LevelX's concurrent NOR flash access capabilities. The application is designed to execute multiple read, write, and erase operations on a NOR flash device, showcasing concurrent access from multiple FreeRTOS tasks. Each task logs its progress after every successful operation, and all required software code for handling NOR flash I/O operations is provided.

The application starts by initializing the LevelX system and creating tasks to handle concurrent NOR flash operations. At this stage, all LevelX resources are created, the NOR flash driver is initialized, and the application creates 3 tasks (main task at higher priority than the other tasks):

- __main_task__: Initializes and manages the NOR flash driver lifecycle.
- __app_task_one__: Performs multiple write, read, and data integrity checks on random NOR flash sectors, logging each successful iteration.
- __app_task_two__: Performs multiple write, read, and data integrity checks on random NOR flash sectors, logging each successful iteration.


## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as NOR flash memory), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

- __Step 1__: Prepare concurrent execution context. Creates an event and 3 tasks with the same priority.
- __Step 2__: Initialize LevelX NOR flash.
- __Step 3__: Start the FreeRTOS scheduler.
- __Step 4__: Format and open LevelX NOR flash.
- __Step 5__: Each task performs 5 iterations of read, write, and data integrity check on random NOR flash sectors, logging each successful operation.
- __Step 6__: Close the NOR flash driver.


If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: Prepare concurrent execution context
[INFO] Step 2: Initialize LevelX NOR flash
[INFO] Step 3: Starting FreeRTOS Scheduler
[INFO] Step 4: Format and open LevelX NOR flash
[INFO] Step 5, Task 1, Iteration 1: Read, write, and data integrity check completed successfully for sector 220
[INFO] Step 5, Task 2, Iteration 1: Read, write, and data integrity check completed successfully for sector 368
[INFO] Step 5, Task 1, Iteration 2: Read, write, and data integrity check completed successfully for sector 884
[INFO] Step 5, Task 2, Iteration 2: Read, write, and data integrity check completed successfully for sector 573
[INFO] Step 5, Task 1, Iteration 3: Read, write, and data integrity check completed successfully for sector 13
[INFO] Step 5, Task 2, Iteration 3: Read, write, and data integrity check completed successfully for sector 363
[INFO] Step 5, Task 1, Iteration 4: Read, write, and data integrity check completed successfully for sector 992
[INFO] Step 5, Task 2, Iteration 4: Read, write, and data integrity check completed successfully for sector 693
[INFO] Step 5, Task 1, Iteration 5: Read, write, and data integrity check completed successfully for sector 782
[INFO] Step 5, Task 2, Iteration 5: Read, write, and data integrity check completed successfully for sector 464
[INFO] Step 5: All concurrent read, write, and data integrity checks completed successfully for both tasks
[INFO] Step 6: Close the NOR flash driver
```


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

- *NOR*: Configured with the following features and settings:
  - Write, read, and erase operations enabled
  - Optimized for concurrent access
  - Configured for high-speed operations
  - Selected GPIO pins support the NOR alternate function. They are configured in push-pull mode with no pull-up or pull-down.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

<!--
@startuml
@startditaa{doc/w25q128j_generic_hardware_setup.png}
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

<!--
@startuml
@startditaa{doc/w25q128j_generic_hardware_setup.png}
  +-------------------------+                     +-------------------------+
  |          +--------------+                     |                         |
  |          |STM32 XSPIi   |                     |                         |
  |          |              |                     |                         |
  |          |          CLK *---------------------* SCLK                    |
  |          |              |                     |                         |
  |          |       DQS/WS *---------------------* DQS                     |
  |          |              |                     |                         |
  |          |      IO[7:0] *---------------------* SIO[7:0]                |
  |          |              |                     |                         |
  |          |          nCS *---------------------* CS#                     |
  |          |              |                     |                         |
  |          |              |                     |                         |
  |          +--------------+                     |                         |
  |                         |                     |                         |
  |                         |                     |                         |
  | STM32 MCU               |                     |       MX66UW1G45G flash |
  +-------------------------+                     +-------------------------+
@endditaa
@enduml
-->

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.



<details>
<summary>On STM32G5 series.</summary>
<details>
  <summary>On board NUCLEO-G564RE.</summary>

  | Arduino pin | MCU pin | Signal name     | User Label    |
  | :---:       | :---:   | :---:           | :---:         |
  | D13         | PG3     | SPI1_CLK        | -             |
  | D12         | PC11    | SPI1_MISO       | -             |
  | D11         | PB2     | SPI1_MOSI       | -             |
  | D9          | PB15    | SPI1_CS         | -             |

  The W25Q128J NOR Flash memory is provided by an Arduino-click Flash-5/Flash-6 shield connected to the Arduino headers. The SPI1 kernel clock is PCLK2 (140 MHz) and the baud-rate prescaler is 32, giving a 4.375 MHz SCLK, well within the W25Q128J Read Data limit of 50 MHz.
</details>
</details>




## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the Cube Firmware package.")


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the Cube Firmware package.")


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
