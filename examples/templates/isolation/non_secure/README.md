<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Template: *example_template_isolation_non_secure*__

**Example version:** 0.6.4

This set of template files initiates __non-secure bare-metal C projects with with trusted execution environment setup__ with the `Integrated Development Environments (IDEs)` supported in the STM32Cube ecosystem.


## __1. Content Description__

This template non-secure application project initiates a bare-metal C project.
It integrates all the required startup files and HAL drivers to initialize the system.
This non-secure template application creates a single binary that can use the entire non-secure memory layout and non-secure system resources.

The non-secure template files are used to:

- `Start an IDE project` with your preferred IDE.

> **_NOTE:_** In this document, the `_ns` suffix indicates a non-secure element.

These template files encompass:

- Platform and IDE agnostic template files in the `application/` folder which contain non-secure application and non-secure callable services.
- Platform and IDE agnostic secure non-secure callable (NSC) services header file in `../secure_nsclib/` folder.
- Target specific files (one folder per STM32 board, for example: `NUCLEO-V873XJ/`).
- Target specific files secure non-secure callable (NSC) object (one folder per STM32 board, for example: `../secure_nsclib/NUCLEO-V873XJ/`).

These target specific files belong to several categories.

  - Project files, depending of format (`cmake/`, `ewarm/` and `open-cmsis/`).
  - Hardware-specific C code:
    - Application specific files in `user_modifiable/Device/` (linker scripts, device startup files...).
    - STM32 utility initialization and configuration files in `user_modifiable/Utility/` (syscalls).
    - STM32 peripherals initialization files in `generated/`.


## __2. Software Environment and how to use the template__

The non-secure template project files facilitate the creation of an application using the tools of your choice.

### __2.1. With one of the STM32Cube ecosystem IDEs__

You just need to open the non-secure IDE project files present in the folder of the `STM32 board` you are interested in.
Where provided, use the global Open-CMSIS solution at the template root to build and debug the complete secure and non-secure application.

### __2.2. With any other IDE of your choice__

You just need to create a non-secure project of your own and import the reference C code (`application/` and target `generated/` folders).

At non-secure main program start, the `mx_system_init()` function is called. It initializes the peripherals, nonvolatile memory (such as flash memory, NVM, or external memories), MPU regions (if applicable), and the SysTick.


## __3. Configuration__

### __3.1. Default Configuration__

This non-secure template application runs from the internal non-volatile memory.

The system configuration aims at offering the optimal MCU performances:

- All optimization paths are enabled (instruction cache, data cache...).

> **_NOTE:_** This configuration is optimized for best performance but not for low-power consumption.

## __4. Troubleshooting__

See [top readme Troubleshooting](../README.md#4-troubleshooting)


## __5. See Also__

See [top readme See Also](../README.md#5-see-also)


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
