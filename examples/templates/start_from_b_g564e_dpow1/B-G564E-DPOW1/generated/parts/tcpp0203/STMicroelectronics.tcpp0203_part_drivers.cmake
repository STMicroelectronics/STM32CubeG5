# origin-pack: generated_STMicroelectronics::tcpp0203_part_drivers@0.0.1
# file-format: 1.0.0
project(generated_STMicroelectronics_tcpp0203_part_drivers_0_0_1)
cmake_minimum_required(VERSION 3.30)
add_library(generated_STMicroelectronics_tcpp0203_part_drivers_0_0_1 INTERFACE)

# List of all CMSIS properties that influence conditions for this package


# Device specific defined symbols








# Enable all components in this package
if(CMSIS_ENTIRE_generated_STMicroelectronics_tcpp0203_part_drivers_0_0_1)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:STM32CubeMX2 Config#Csub:TCPP0203#Cversion:1.0.0#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.tcpp0203_part_drivers.0.0.1:TCPP0203 CFG Condition
# description: STMicroelectronics configuration for TCPP0203 part driver
set(generated_STMicroelectronics.tcpp0203_part_drivers.0.0.1_TCPP0203_CFG_Condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:USBPD#.*Csub:TCPP0203 Core(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:USBPD#.*Csub:TCPP0203 IO#.*Cvariant:I2C EXTI ADC Polling(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.tcpp0203_part_drivers.0.0.1_TCPP0203_CFG_Condition enabled")


# condition: generated_STMicroelectronics.tcpp0203_part_drivers.0.0.1:TCPP0203 Core Condition
# description: STMicroelectronics TCPP0203 Core Driver
set(generated_STMicroelectronics.tcpp0203_part_drivers.0.0.1_TCPP0203_Core_Condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:USBPD#.*Csub:TCPP0203 IO(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.tcpp0203_part_drivers.0.0.1_TCPP0203_Core_Condition enabled")


# condition: generated_STMicroelectronics.tcpp0203_part_drivers.0.0.1:TCPP0203 Custom IO Condition
# description: 
set(generated_STMicroelectronics.tcpp0203_part_drivers.0.0.1_TCPP0203_Custom_IO_Condition "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.tcpp0203_part_drivers.0.0.1_TCPP0203_Custom_IO_Condition enabled")


# condition: generated_STMicroelectronics.tcpp0203_part_drivers.0.0.1:TCPP0203 IO I2C EXTI AND ADC Polling Condition
# description: STMicroelectronics TCPP0203 IO I2C AND ADC Polling Driver
set(generated_STMicroelectronics.tcpp0203_part_drivers.0.0.1_TCPP0203_IO_I2C_EXTI_AND_ADC_Polling_Condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:I2C(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:ADC(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:EXTI(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.tcpp0203_part_drivers.0.0.1_TCPP0203_IO_I2C_EXTI_AND_ADC_Polling_Condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:STM32CubeMX2 Config#Csub:TCPP0203#Cversion:1.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_Board_Part_STM32CubeMX2_Config_TCPP0203_1_0_0")
  target_compile_definitions(generated_STMicroelectronics_tcpp0203_part_drivers_0_0_1 INTERFACE "$<${generated_STMicroelectronics.tcpp0203_part_drivers.0.0.1_TCPP0203_CFG_Condition}:-DCMSIS_USE_generated_Board_Part_STM32CubeMX2_Config_TCPP0203_1_0_0=1>")
  target_include_directories(generated_STMicroelectronics_tcpp0203_part_drivers_0_0_1 INTERFACE "$<${generated_STMicroelectronics.tcpp0203_part_drivers.0.0.1_TCPP0203_CFG_Condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_tcpp0203_part_drivers_0_0_1 INTERFACE "$<${generated_STMicroelectronics.tcpp0203_part_drivers.0.0.1_TCPP0203_CFG_Condition}:${CMAKE_CURRENT_LIST_DIR}/mx_tcpp0203.c>")
endif()

