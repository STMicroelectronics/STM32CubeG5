<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *da_stda*__

**Example version:** 2.0.0

How to use __Debug Authentication (DA)__ on STM32 devices.

Debug Authentication controls access to device debug services. This example supports password and certificate authentication, depending on the hardware capabilities of the target. It generates and installs a DA configuration, then allows the user to perform a discovery whenever the device is reachable, or a regression with the selected authentication method. When certificate authentication is selected, the user can also authenticate a debug session.

The example uses the RoT Provisioning software pack and STM32CubeProgrammer. It does not contain an application project.


## __1. Overview__

Each board directory contains the files required to run the standalone DA flow:

- `da.py`: Example entry point.
- `da.ini`: Board-specific provisioning, credential, and final lifecycle-state configuration.
- `config/example.json`: Internal device description and default option bytes. Do not modify this file.
- `xml/`: DA configuration files. The password is defined in the `Password` field of `da_config_password.xml`.
- `keys/`: DA key pair used for certificate authentication (only on supported boards).
- `certificate/`: DA certificate (only on supported boards).
- `obk/`: Generated DA OBK output directory.

The provisioning workflow generates the DA OBK and guides the user through flashing their own binaries, option bytes, OBK and the generated DA OBK. It then programs the configured final product state.

Available commands are `provisioning`, `discovery`, `dbg-auth`, and `regression`.
In password mode, `dbg-auth` is equivalent to `regression`.


## __2. Requirements__

- Python 3.10 or later
- STM32CubeProgrammer, including STM32TrustedPackageCreator where required by the target
- An ST-LINK connection to the target board
- Required Python modules:

  ```bash
  pip install -r requirements.txt
  ```

Check that the `STM32_PRG_PATH` environment variable points to the STM32CubeProgrammer installation you want to use, and update it if necessary.


## __3. Quickstart__

<details open>
  <summary>On STM32G5 series.</summary>

On this series, the DA supports password and certificate authentication mode.

Follow [How to start with DA on STM32G5](https://wiki.st.com/stm32mcu/wiki/Security:How_to_start_with_DA_on_STM32G5)
</details>


## __4. Known limitations__

<details open>
  <summary>On STM32G5 series.</summary>

TZ disabled not supported.
</details>


## __5. See Also__

<details open>
  <summary>On STM32G5 series.</summary>

More information about DA can be found in [Debug Authentication for STM32G5](https://wiki.st.com/stm32mcu/wiki/Security:Debug_Authentication_for_STM32G5)
</details>


## __6. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
