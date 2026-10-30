# origin-pack: generated_STMicroelectronics::led_part_drivers@0.0.1
# file-format: 1.0.0
project(generated_STMicroelectronics_led_part_drivers_0_0_1)
cmake_minimum_required(VERSION 3.30)
add_library(generated_STMicroelectronics_led_part_drivers_0_0_1 INTERFACE)

if(CMSIS_ENTIRE_generated_STMicroelectronics_led_part_drivers_0_0_1)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:STM32CubeMX2 Config#Csub:LED#Cversion:1.1.0#generated:true")
endif()

set(generated_STMicroelectronics.led_part_drivers.0.0.1_LED_CFG_Condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:LED#.*Csub:GPIO(#.*|$)>,>>")

if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:STM32CubeMX2 Config#Csub:LED#Cversion:1.1.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)
  target_compile_definitions(generated_STMicroelectronics_led_part_drivers_0_0_1 INTERFACE "$<${generated_STMicroelectronics.led_part_drivers.0.0.1_LED_CFG_Condition}:-DCMSIS_USE_generated_Board_Part_STM32CubeMX2_Config_LED_1_1_0=1>")
  target_include_directories(generated_STMicroelectronics_led_part_drivers_0_0_1 INTERFACE "$<${generated_STMicroelectronics.led_part_drivers.0.0.1_LED_CFG_Condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
endif()