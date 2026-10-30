# origin-pack: generated_STMicroelectronics::rgbled_part_drivers@0.0.1
# file-format: 1.0.0
project(generated_STMicroelectronics_rgbled_part_drivers_0_0_1)
cmake_minimum_required(VERSION 3.30)
add_library(generated_STMicroelectronics_rgbled_part_drivers_0_0_1 INTERFACE)

# List of all CMSIS properties that influence conditions for this package


# Device specific defined symbols








# Enable all components in this package
if(CMSIS_ENTIRE_generated_STMicroelectronics_rgbled_part_drivers_0_0_1)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:STM32CubeMX2 Config#Csub:RGBLED#Cversion:1.0.0#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.rgbled_part_drivers.0.0.1:RGBLED Advanced Condition
# description: STMicroelectronics RGBLED current-mode (DAC + COMP) Driver
set(generated_STMicroelectronics.rgbled_part_drivers.0.0.1_RGBLED_Advanced_Condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:TIM(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:DAC(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:COMP(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.rgbled_part_drivers.0.0.1_RGBLED_Advanced_Condition enabled")


# condition: generated_STMicroelectronics.rgbled_part_drivers.0.0.1:RGBLED CFG Condition
# description: STMicroelectronics configuration for RGBLED part driver
set(generated_STMicroelectronics.rgbled_part_drivers.0.0.1_RGBLED_CFG_Condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:RGBLED#.*Csub:PWM NO CONTROL(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.rgbled_part_drivers.0.0.1_RGBLED_CFG_Condition enabled")


# condition: generated_STMicroelectronics.rgbled_part_drivers.0.0.1:RGBLED Core Condition
# description: STMicroelectronics RGBLED Core Driver
set(generated_STMicroelectronics.rgbled_part_drivers.0.0.1_RGBLED_Core_Condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:TIM(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.rgbled_part_drivers.0.0.1_RGBLED_Core_Condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:STM32CubeMX2 Config#Csub:RGBLED#Cversion:1.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_Board_Part_STM32CubeMX2_Config_RGBLED_1_0_0")
  target_compile_definitions(generated_STMicroelectronics_rgbled_part_drivers_0_0_1 INTERFACE "$<${generated_STMicroelectronics.rgbled_part_drivers.0.0.1_RGBLED_CFG_Condition}:-DCMSIS_USE_generated_Board_Part_STM32CubeMX2_Config_RGBLED_1_0_0=1>")
  target_include_directories(generated_STMicroelectronics_rgbled_part_drivers_0_0_1 INTERFACE "$<${generated_STMicroelectronics.rgbled_part_drivers.0.0.1_RGBLED_CFG_Condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_rgbled_part_drivers_0_0_1 INTERFACE "$<${generated_STMicroelectronics.rgbled_part_drivers.0.0.1_RGBLED_CFG_Condition}:${CMAKE_CURRENT_LIST_DIR}/mx_rgbled_no_control.c>")
endif()

