<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example : *example_uart_dma_secure_nonsecure_non_secure*__

**Example version:** 0.0.1

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to receive a password over UART using circular DMA and validate it through a non-secure callable (NSC) service.


## __1. Detailed scenario__

__Initialization phase__: At non-secure main program start, the `mx_system_init()` function is called. It initializes the non-secure peripherals, MPU regions, SCB, and the SysTick before the application starts.

The non-secure application executes the following __example steps__:

    * The init of the UART and DMA instances in circular mode are triggered by the applicative code.
    * The user callbacks for I2C master TX/RX transfer completed and transfer error are registered.

__Step 1__: The UART in DMA circular mode is configured and the Rx callback is registered.

__Step 2__: Start the reception of the user password through UART in DMA circular mode.

__Step 3__: Wait the reception of the next password through UART in DMA circular mode.  A carriage return (`CR`) terminates the password reception.

__Step 4__: Check the correctness of the received user password through non-secure callable `IsPasswordCorrect()` function.
Returns to __Step3__ if the password is incorrect until receiving the correct one.

__End of example__: After __Step4__, the example is completed. You can verify that the example runs properly via the status LED and the `ExecStatus` variable.

> [!WARNING] The example password is `I love STM32`. It is transmitted as plaintext, echoed over UART, and hard-coded in the secure image. Anyone with access to the UART signals, debugger, or terminal can observe it. This mechanism demonstrates
> TrustZone and GTZC isolation only and must not be used as a production authentication design.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

__UART__:

The UART is configured with the following settings:

- The baud rate is set to 115200.
- The word length is set to 8 bits.
- Stop bits are set to 1 bit.
- Parity is set to NONE.
- Hardware flow control is disabled.

__DMA__:

- A channel is linked to the UART RX request.
- The transfer direction is peripheral-to-memory.
- The destination address is incremented for each received byte.
- Reception uses a circular linked-list transfer.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

Select the STM32 UART instance connected to the embedded ST-LINK on your board. The ST-LINK provides a virtual COM port over USB, which is mounted on the host PC and ready for use with a terminal emulator.

<!--
@startuml
@startditaa{doc/ASCII_Board_PC.png}

STM32 board connected to the host PC.

    /--------------------------------------------------\           /----------------\
    |  /--------------\      /-------------------------+           |                |
    |  |STM32 MCU     |      |ST-LINK                  |           |                |
    |  |  /-----------+      +-----------\             |           |                |
    |  |  |USART      |      |USART      |             |           |                |
    |  |  |           |      |           |             |           |                |
    |  |  | USARTi_TX *------* USARTi_RX |   /---------+           +---------\      |
    |  |  |           |      |           |   |   USB   +-----------+   USB   |      |
    |  |  | USARTi_RX *------* USARTi_TX |   \---------+           +---------/      |
    |  |  |           |      |           |             |           |                |
    |  |  \-----------+      +-----------/             |           |                |
    |  |              |      |                         |           |                |
    |  \--------------/      \-------------------------+           |                |
    |                                                  |           |                |
    |  /-------------\                                 |           +---------\      |
    |  | STM32 Board |                                 |           | Host PC |      |
    \--+-------------+---------------------------------/           \---------+------/
@endditaa
@enduml
-->

![ASCII_Board_PC](doc/ASCII_Board_PC.png)

### __3.2. Specific board setups__

<details>
  <summary>On STM32G5 series.</summary>
  <details>
    <summary>On board NUCLEO-G564RE.</summary>

  | MCU pin | Signal name  | User Label    |
  | :-----: | :----------: | :-----------: |
  |   PG3   |   GPIO       | MX_STATUS_LED |
  |   PA3   |  LPUART1_RX  |  DBG_VCP_RX   |
  |   PA2   |  LPUART1_TX  |  DBG_VCP_TX   |


  | Region      | Start address | Size      |
  |:-----------:|:-------------:|:---------:|
  | ROM         | `0x08040000`  | `0x40000` |
  | RAM         | `0x20008000`  | `0x14000` |

  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

See [top readme Troubleshooting](../README.md#4-troubleshooting)


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

See [top readme See Also](../README.md#5-see-also)


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
