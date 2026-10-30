<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *example_stirot*__

**Example version:** 2.0.0

How to use the __STiRoT__ boot stage.

STiRoT stands for *ST immutable Root of Trust*. It provides two services:

- __Secure Boot__: Verifies the integrity and authenticity of the application firmware before execution.
- __Secure Firmware Update__: Verifies and installs new encrypted firmware images.

It requires two flash areas: one for firmware execution, one for downloading new firmware.
After successful verification, the new firmware image is installed in the execution area.

STiRoT security relies on cryptography for integrity, authenticity, and confidentiality.
However, the use of cryptography alone is insufficient. A set of hardware and software security measures
are included in the STiRoT to protect critical operations and sensitive data (such as a secret key).


## __1. Overview__

This example contains one project:

- **User application**: The main firmware image to protect and update.

The new firmware image is downloaded through the application menu using a YModem transfer in a terminal emulator.

Supporting Python scripts include:

- `provisioning.py`: Guides the user through configuration, build, and programming steps.
- `project_appli.py`: Automates prebuild/postbuild steps.


## __2. Requirements__

- Python 3.10 or later
- STM32CubeProgrammer (with STM32TrustedPackageCreator)
- Tera Term (or another terminal emulator supporting YModem)
- Required Python modules:
  ```bash
  pip install -r requirements.txt
  ```


## __3. Quickstart__

<details open>
  <summary>On STM32G5 series.</summary>

Follow [How to start with STiRoT on STM32G5](https://wiki.st.com/stm32mcu/wiki/Security:How_to_start_with_STiRoT_on_STM32G5)
</details>


## __4. Known limitations__

None


## __5. See Also__

<details open>
  <summary>On STM32G5 series.</summary>

More information about STiRoT can be found in [STiRoT for STM32G5](https://wiki.st.com/stm32mcu/wiki/Security:STiRoT_for_STM32G5)
</details>


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
