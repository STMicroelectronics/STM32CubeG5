<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Template: *example_template_isolation_secure*__

**Example version:** 0.6.4

This set of template files initiates __secure bare-metal C projects with trusted execution environment setup__ with the `Integrated Development Environments (IDEs)` supported in the STM32Cube ecosystem.


## __1. Content Description__

This template secure application project initiates a bare-metal C project.
It integrates all the required startup files and HAL drivers to initialize the system.
The secure project template provides one binary with trusted execution environment (tee) setup.

The secure template files are used to:

- `Start an IDE project` with your preferred IDE.

> **_NOTE:_** In this document, the `_s` suffix indicates a secure element.

These secure template files encompass:

- Platform and IDE agnostic template files in the `application/` folder which contain secure application and non secure callable services.
- Platform and IDE agnostic secure non-secure callable (NSC) services header file in `../secure_nsclib/` folder.
- Target specific files (one folder per STM32 board, for example: `NUCLEO-V873XJ/`).

These target specific files belong to several categories.

  - Project files, depending of format (`cmake/`, `ewarm/` and `open-cmsis/`).
  - Hardware-specific C code:
    - Application specific files in `user_modifiable/Device/` (linker scripts, device startup files...).
    - STM32 utility initialization and configuration files in `user_modifiable/Utility/` (syscalls).
    - STM32 trusted execution environment setup and peripherals initialization files in `generated/` (HAL code for clock configuration...).


## __2. Software Environment and how to use the template__

The secure template project files facilitate the creation of an application using the tools of your choice.

### __2.1. With one of the STM32Cube ecosystem IDEs__

You just need to open the IDE project files present in the folder of the `STM32 board` you are interested in.
The IDE initiates the project with the reference files, and you can complete it as per your needs.

### __2.2. With any other IDE of your choice__

You just need to create a project of your own and import the reference C code (`application/` and `generated/` folders).

At startup, the reset handler first executes `SystemInit()`, where `TZ_SAU_Setup()` applies secure/non-secure partitioning (SAU memory attribution, Cortex security features, and NVIC target-state partitioning).

Then, at secure main program start, `mx_system_init()` is called; where `mx_tee_setup()` is executed to apply trusted execution setup before the remaining startup steps including MPU regions configuration if applicable and peripherals configuration.

Finally, the secure application jumps to nsecure using `mx_start_application()`.

## __3. Configuration__

### __3.1. Default Configuration__

This secure template application runs from the internal non-volatile memory.

The system configuration aims at offering the optimal MCU performances:

- The system clock is set to its maximum speed.

> **_NOTE:_** This configuration is optimized for best performance but not for low-power consumption.


## __4. Troubleshooting__

See [top readme Troubleshooting](../README.md#4-troubleshooting)


## __5. See Also__

See [top readme See Also](../README.md#5-see-also)


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
