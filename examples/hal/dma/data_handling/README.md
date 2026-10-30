<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *hal_dma_data_handling*__

**Example version:** 2.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use a DMA to transfer a buffer from memory to memory, using different variants of data handling. It is achieved through the HAL API.


## __1. Detailed scenario__

In this example, the CPU and a DMA share a buffer to manage the data: `DstBuffer`.
On an STM32 device with data cache enabled, it is mandatory to ensure the buffer is never cached, as this scenario does not include data cache maintenance operations.
To do so, we place the buffer in the `non_cacheable_area` memory section and apply the appropriate MPU settings during system initialization in `mx_system_init()`.

On STM32V8 targets, this example reserves a dedicated 1 KB non-cacheable RAM window for CPU/DMA shared data.
This short-term implementation follows the current recommendation for direct DMA transfers when the total size of shared data remains below 1 KB.
If the amount of non-cacheable data grows beyond 1 KB, the board linker scripts and MPU configuration must be extended accordingly.

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), the system clock, and the SysTick.

The application executes the following __example steps__:

Demonstrates sequentially different variants of DMA data handling configuration:

__Step 1__: Initializes the DMA according to the selected configuration, including the data handling configuration.

__Step 2__: Starts the DMA transfer in interrupt mode.

__Step 3__: Waits for the DMA transfer completion and verifies the destination buffer.

__Step 4__: Deinitializes the DMA.

Data handling variants:

  - Right aligned zero padding
  - Right aligned sign extended data
  - Right aligned left truncated data
  - Left aligned right truncated data
  - Source byte exchange
  - Destination byte exchange
  - Destination half-word exchange

__End of example__: Reports the outcome of the data transfer via the variable **`ExecStatus`**,\
and the **status LED** remains turned on in case of success.


## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

- The *DMA* is configured in memory to memory mode.
- The *DMA* is configured with source and destination increment:\
  After each word transfer, the DMA automatically increments source and destination addresses to copy the next word
  to the next location in the destination buffer.
- 32-bits alignment: source and destination addresses must respect DMA data width. The worst case is WORD for the below tests.
- On STM32V8 targets, `DstBuffer` is placed in `non_cacheable_area` with 8-byte alignment, and the linker reserves a dedicated 1 KB non-cacheable RAM window for this section.
- The *DMA* is triggered by software.
- Data handling configuration:
    - Right aligned zero padding:
      - Source data length: bytes
      - Destination data length: half-word
      - Right aligned, zero padding

      Each byte is converted to a half-word. The byte is right aligned with zero padding (on the left).

    - Right aligned sign extended data
      - Source data length: bytes
      - Destination data length: half-word
      - Right aligned, sign extended

      Each byte is converted to a half-word. The byte is right aligned with the byte sign extended on the left.

    - Right aligned left truncated data
      - Source data length: half-word
      - Destination data length: byte
      - Right aligned, left truncated

      Each half-word is converted to bytes, right aligned and thus left truncated.

    - Left aligned right truncated data
      - Source data length: half-word
      - Destination data length: byte
      - Left aligned, right truncated

      Each half-word is converted to bytes, left aligned and thus right truncated.

    - Source byte exchange
      - Source data length: word
      - Destination data length: word
      - Source byte exchange

      The two consecutive bytes within the unaligned half-word of each source word are exchanged.  B3B2B1B0 --> B3B1B2B0

    - Destination byte exchange
      - Source data length: word
      - Destination data length: word
      - Destination byte exchange

      The two consecutive bytes are exchanged in each destination half-word.

    - Destination half-word exchange
      - Source data length: word
      - Destination data length: word
      - Half-word byte exchange

      The two consecutive half-words are exchanged in each destination word.


## __3. Hardware environment and setup__

### __3.1. Generic Setup__

JTAG/SWD probe may be used to check variables and buffers.

### __3.2. Specific board setups__

<details>
  <summary>On STM32G5 series.</summary>
  <details>
  <summary>DMA specification:</summary>

    - The DMA source (SrcBuffer) is declared as const and thus placed in the Flash memory.
    - The DMA destination (DstBuffer) is placed in the RAM.
    - The transfer is handled by GPDMA1 channel 0 in memory-to-memory mode.
    - STM32G5 has no data cache; the `non_cacheable_area` section is therefore mapped as plain RAM.
  </details>
  <details>
  <summary>On board NUCLEO-G564RE.</summary>

  | Board pin   | MCU pin | Signal name          | ARDUINO <br/> connector pin |
  | :---:       | :---:   | :---:                | :---:        |
  | -           | PG3     | LED                  | -            |
  </details>
</details>


## __4. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Here are the points of attention for this specific example:

 - Take care of data misalignment:\
   Depending on the DMA data width used, the source and the destination addresses must respect the data alignment.
   For details, refer to the reference manual of your MCU.

 - Take care of cache coherency issues:\
   When the data cache memory is enabled, it is generally not in the path of the DMA transfer,
   thus cache coherency issues might appear. It might be necessary to tackle the cache coherency.
   In this example, every buffer shared by the CPU and the DMA must stay in `non_cacheable_area`.
   If new DMA-shared data is added and exceeds the reserved 1 KB window, the board linker scripts and MPU configuration must be updated together.
   See H7 FAQ:\
   [DMA-is-not-working-on-STM32H7-devices](https://community.st.com/s/article/FAQ-DMA-is-not-working-on-STM32H7-devices)

 - Take care of __DMA ports__:
  Depending on STM32 series, and DMA instance used (GPDMA/HPDMA/LPDMA) specific DMA ports constraints must be respected.
  For details, refer to the reference manual of your MCU. You can also see the application note in the `__5. See Also` section.


## __5. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

- [Application Note AN2548](https://www.st.com/resource/en/application_note/an2548-introduction-to-dma-controller-for-stm32-mcus-stmicroelectronics.pdf): Using the STM32 Series DMA controller
- [Application Note AN4031](https://www.st.com/resource/en/application_note/an4031-using-the-stm32f2-stm32f4-and-stm32f7-series-dma-controller-stmicroelectronics.pdf): Using the STM32F2, STM32F4 and STM32F7 Series DMA controller
- [Application Note AN5593](https://www.st.com/resource/en/application_note/an5593-how-to-use-the-gpdma-for-stm32-mcus-stmicroelectronics.pdf): How to use the GPDMA for STM32 Series microcontrollers

The documentation of the drivers of the relevant STM32 series contains more detailed information.

For instance for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
