<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_uart_wakeup_from_stop*__

**Example version:** 2.0.6

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

The communication consists of an infinite number of receive-transmit transactions of changing message,
the scenario includes entering STOP mode to save power when there is no ongoing transmission or reception.


## __1. Detailed scenario__

__Initialization phase__: At the beginning of the `main()` function, the `mx_system_init()` function is called to initialize the peripherals, the flash interface, the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: configures and initializes the UART instance and the NVIC.
            Registers the user callbacks for UART interrupts: TX/RX transfer completed and transfer error.

__Step 2__: the responder expects to receive a message as a null-terminated string from the controller board, in interrupt mode. A counter of attempts is reset when initiating the communication loop.

__Step 3__: the MCU will enter STOP mode and one or several UART interrupts will be able to wake it up.

__Step 4__: the responder sends back the received message in interrupt mode. UART communication wakes the MCU up, and the clock is re-configured.

__Step 5__: waits for one of these UART interrupts: write transfer complete or transfer error. If no transmission or reception is occurring, the MCU enters STOP mode.
            Returns to step 2 indefinitely if no error occurs.

The communication status is reported via the status LED and the variable ExecStatus.

__End of example__: if no error occurs, the data is transferred infinitely between the controller and the responder. If the maximum number of attempts is reached, the data transfer is stopped by reporting an error status.

The following **message sequence chart** is used to describe the UART communication behavior between the controller board and the responder board.

The Low Power status of the MCU can be checked via the status wakeup pin. This pin is activated to indicate the wakeup status of the MCU.
MCU turns on this LED in RUN mode and turns it off in STOP mode.

![uart-wakeup_from_stop_NominalCase](doc/uart-wakeup_from_stop_NominalCase.svg)

<details>
<summary> Expand this tab to visualize the sequence chart diagram in case of a data transmission error. </summary>

![uart-wakeup_from_stop_AttemptsMechanism](doc/uart-wakeup_from_stop_AttemptsMechanism.svg)

</details>


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

The example demonstrates the following peripheral:

__UART__:

We select an UART with accessible Tx and Rx signals on the board so that we can wire it to the controller board.

The UART is configured with the following settings:

- The baud rate is set to 115200.
- The word length is set to 8 bits.
- Stop bits are set to 1 bit.
- Parity is set to NONE.

<!--
@startuml
@startditaa{doc/ASCII_data_frame.png} -E -S

    The UART data frame of the current configuration:

      /--------------------------------------\
      |  /------+-----------------+-------\  |
      |  |  SB  |   8 bits data   |  STB  |  |
      |  \------+-----------------+-------/  |
      \--------------------------------------/

      /---------------\
      | SB:  Start Bit|
      | STB: Stop Bit |
      \=--------------/
@endditaa
@enduml
-->


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

<!--
@startuml
@startditaa{doc/ASCII_uart_two_boards.png} -E -S
    /-------------------------\                     /-------------------------\
    |          /--------------+                     +--------------\          |
    |          | STM32 USARTi |                     | STM32 USARTi |          |
    |          |              |                     |              |          |
    |          |    USARTi_TX *---------------------* USARTi_RX    |          |
    |          |              |                     |              |          |
    |          |              |                     |              |          |
    |          |              |                     |              |          |
    |          |    USARTi_RX *---------------------* USARTi_TX    |          |
    |          |              |                     |              |          |
    |          \--------------+                     +--------------/          |
    |                         |                     |                         |
    |                     GND *---------------------* GND                     |
    |                         |                     |                         |
    |  /------------------\   |                     |  /-----------------\    |
    |  | STM32 Responder  |   |                     |  | STM32 Controller|    |
    |  | Board            |   |                     |  | Board           |    |
    |  \------------------/   |                     |  \-----------------/    |
    \-------------------------/                     \-------------------------/
@endditaa
@enduml
-->

![ASCII_uart_two_boards](doc/ASCII_uart_two_boards.png)

### __3.2. Specific board setups__

This section describes the exact hardware configurations of your project.

<!-- YOUR BOARDS ADDED HERE BY README GENERATION -->

<details>
  <summary>On STM32G5 series.</summary>
  <details>
    <summary>On board NUCLEO-G564RE.</summary>

  |  MCU pin  |  Signal name  |  User Label   |
  |:---------:|:-------------:|:-------------:|
  |    PG3    |     GPIO      | MX_STATUS_LED |
  |   PD11    |   UART4_RX    |       -       |
  |   PD12    |   UART4_TX    |       -       |
  |    PC6    |     GPIO      |       -       |

  **_NOTE:_**
    - UART4 is the UART instance used for the communication between the Nucleo boards.
    - UART4 is clocked by HSIDIV
    - UART transmission interrupts (TX) cannot wake up the MCU from STOP mode. Only UART reception (RX) can.

  </details>
</details>


### __3.3. Testing the Example__

This example can be tested in two different ways:

__3.3.1. Using Another STM32 Board as Controller
You can use one of the following examples to act as the controller that sends messages to wake up the responder:

hal_uart_two_boards_com_polling_controller: The controller side in a polling mode UART communication.
hal_uart_two_boards_com_it_controller: The controller side in an interrupt mode UART communication.
hal_uart_two_boards_com_dma_controller: The controller side in a DMA mode UART communication.
These examples will send UART messages to the responder, which will wake up from STOP mode upon receiving these messages.

__3.3.2. Using a Terminal on a PC**
Alternatively, you can test this example by connecting the STM32 board to a PC via the STLink UART interface.
To do this, you need to change the UART pins from USART2 (PD6 and PD5) to USART1 (PA9 and PA10), which are connected to the STLink.
This allows you to send UART messages from a terminal on the PC to wake up the responder.


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Find below the points of attention for this specific example.

__Communication Buffers__: Make sure that the size, in bytes, of the responder's reception buffer is equal to the size of the controller's transmission buffer.
__System Clock__: When exiting from Stop mode, the system clock must be configured (see the RCC peripheral section in the reference manual of your MCU).


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

You can also refer to these examples to go further with the UART peripheral:

- hal_uart_two_boards_com_polling_controller: The controller side in a polling mode UART communication.
- hal_uart_two_boards_com_polling_responder: The responder side in a polling mode UART communication.
- hal_uart_echo_polling: retargeting of the C library input and output functions to operate on the UART peripheral.
- hal_uart_two_boards_com_it_controller: The controller side in an interrupt mode UART communication.

More information about the STM32Cube Drivers can be found in the drivers' user manual of the STM32 series you are using.

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
