<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Template: *example_template_isolation*__

**Example version:** 0.6.4

This set of template files initiates __bare-metal C projects with with trusted execution environment setup in secure application followijng by the non-secure application__ with the `Integrated Development Environments (IDEs)` supported in the STM32Cube ecosystem.


## __1. Content Description__

This template application project initiates a bare-metal C project with separate secure and non-secure images.

The project template is made of two binaries:

- A secure binary in the `secure/` folder, which starts at boot and configures the system security and trusted execution environment setup.
- A non-secure binary in the `non_secure/` folder, which is started by the secure binary after the secure initialization is complete.

The `secure_nsclib/` folder contains the header that defines the secure non-secure callable (NSC) services callable by the non-secure application.

For the secure and non-secure image layouts, project contents, and configuration, see the [secure README Content Description](secure/README.md#1-content-description) and [non-secure README Content Description](non_secure/README.md#1-content-description).


## __2. Software Environment and how to use the template__

### __2.1. With one of the STM32Cube ecosystem IDEs__

You just need to open the IDE project files present in the folder of the `global_build/STM32 board` you are interested in.
The IDE initiates the secure and the non-secure projects with the reference files, and you can complete it as per your needs.

### __2.2. With any other IDE of your choice__

Create separate secure and non-secure projects. Follow the project setup instructions in the [secure README Software environment](secure/README.md#22-with-any-other-ide-of-your-choice) and [non-secure README Software environment](non_secure/README.md#22-with-any-other-ide-of-your-choice).


## __3. Configuration__

### __3.1. Default Configuration__

See the [secure README Default Configuration](secure/README.md#31-default-configuration) and [non-secure README Default Configuration](non_secure/README.md#31-default-configuration) for the default configuration of each image.

### __3.2. Specific board setups__

User Option Bytes configuration:

#### __3.2.1. STM32V8 series__

Please note the internal NVM is fully secure by default. Configure the Secure Watermark Option Bytes `SECWM1_STRT`/`SECWM1_END` and `SECWM2_STRT`/`SECWM2_END` according to the application configuration using `STM32CubeProgrammer`.

The Secure Watermark Option Bytes setup aligned with the project linker/scatter file is:

- `SECWM1_STRT=0x0`, `SECWM1_END=0xFF`: all Bank 1 pages are secure.
- `SECWM2_STRT=0x1`, `SECWM2_END=0x0`: no Bank 2 page is secure; Bank 2 is non-secure.

#### __3.2.2. STM32G5 series__

Please note that TrustZone is not activated by default on STM32G5 series devices. Enable the `TZEN` option byte using `STM32CubeProgrammer`.

Please note the internal FLASH is fully secure by default. Configure the Secure Watermark Option Bytes `SECWM1_STRT`/`SECWM1_END` and `SECWM2_STRT`/`SECWM2_END` according to the application configuration using `STM32CubeProgrammer`.

The Secure Watermark Option Bytes setup aligned with the project linker/scatter file is:

- `SECWM1_STRT=0x0`, `SECWM1_END=0x7F`: all Bank 1 pages are secure.
- `SECWM2_STRT=0x1`, `SECWM2_END=0x0`: no Bank 2 page is secure; Bank 2 is non-secure.


## __4. Troubleshooting__

Here are some typical points of attention in case of issues.

### __4.1. General Debug Tips__

#### __4.1.1. Flashing issues__

1. Try flashing using `STM32CubeProgrammer`.
2. Check the device configuration against the description in the STM32 system memory boot mode application note.

#### __4.1.2 Startup issues__

1. Read the flash or RAM content using `STM32CubeProgrammer` to make sure the code you expect to run is loaded in the device.
2. Check the boot options of your device (boot pins...) in the reference manual.
3. Try connecting to the board and use a debugger to spot any hard fault in the code.

#### __4.1.3 Issues to connect to the board__

Possible issues are:

1. The firmware running on the target is forcing a low-power mode.
The communication with STLINK is disabled when the board is in low-power mode.

2. The target is secured or debug-disabled.

#### __4.1.4. HAL timing issues__

Care must be taken when using `HAL_Delay()`,
this function provides accurate delay (in milliseconds) based on variable incremented in SysTick ISR.

This implies that if `HAL_Delay()` is called from a peripheral ISR process,
then the SysTick interrupt must have higher priority (numerically lower) than the peripheral interrupt.
Otherwise, the caller ISR process is blocked.
To change the SysTick interrupt priority you have to use `HAL_NVIC_SetPriority()` function.

The example needs to ensure that the SysTick time base is always set to 1 millisecond to have correct HAL operation.

### __4.2 Fault Exceptions__

Arm Cortex®-M processors implement an efficient exception model that traps illegal memory accesses and
several incorrect program conditions.
The HardFault exception is always enabled and has a fixed priority (higher than other interrupts and exceptions,
but lower than non-maskable interrupt NMI). The HardFault exception is therefore executed in cases where a
fault exception is disabled or when a fault occurs during the execution of a fault exception handler.

All other fault exceptions (MemManage fault, BusFault, UsageFault...) have a programmable priority.
After reset, these exceptions are disabled and may be enabled in the system or application software
using the registers in the System Control Block (SCB).

### __4.3 Debug Toolbox__

Refer to the [STM32 debug toolbox](https://www.st.com/resource/en/application_note/an4989-stm32-microcontroller-debug-toolbox-stmicroelectronics.pdf)
for further information about how to troubleshoot the problem.

#### __4.4 Asking for support__

This software component is not yet open to public contribution.
Report any feedback to your ST contact point.


## __5. See Also__

The documentation of the drivers of the relevant STM32 series contains more detailed information.

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
