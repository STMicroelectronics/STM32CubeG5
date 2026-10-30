# origin-pack: generated_STMicroelectronics::usbpd@0.0.1
# file-format: 1.0.0
project(generated_STMicroelectronics_usbpd_0_0_1)
cmake_minimum_required(VERSION 3.30)
add_library(generated_STMicroelectronics_usbpd_0_0_1 INTERFACE)

# List of all CMSIS properties that influence conditions for this package
if(NOT DEFINED CMSIS_Dcore)
  set(CMSIS_Dcore "CMSIS_Dcore not set")
  message(DEBUG "CMSIS_Dcore is not set to any value")
endif()

if(NOT DEFINED CMSIS_Dvendor)
  set(CMSIS_Dvendor "CMSIS_Dvendor not set")
  message(DEBUG "CMSIS_Dvendor is not set to any value")
endif()

if(NOT DEFINED CMSIS_Tcompiler)
  set(CMSIS_Tcompiler "CMSIS_Tcompiler not set")
  message(DEBUG "CMSIS_Tcompiler is not set to any value")
endif()


# Device specific defined symbols




# Include Pre_Include_Global.h globally if needed
if(CMSIS_Tcompiler STREQUAL "IAR")
    target_compile_options(generated_STMicroelectronics_usbpd_0_0_1 INTERFACE "SHELL:--preinclude $<QUOTE>${CMAKE_CURRENT_LIST_DIR}/Pre_Include_Global.h$<QUOTE>")
else()
    target_compile_options(generated_STMicroelectronics_usbpd_0_0_1 INTERFACE "SHELL:-include $<QUOTE>${CMAKE_CURRENT_LIST_DIR}/Pre_Include_Global.h$<QUOTE>")
endif()



# Enable all components in this package
if(CMSIS_ENTIRE_generated_STMicroelectronics_usbpd_0_0_1)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:STM32CubeMX2 Codegen#Csub:pdo_defs#Cversion:2.0.0#generated:true")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:STM32CubeMX2 Codegen#Csub:pwr_tcpp0203_if#Cversion:2.0.0#generated:true")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:STM32CubeMX2 Config#Csub:USBPD#Cversion:0.1.0#generated:true")
endif()

# All conditions used by this package

# condition: generated_STMicroelectronics.usbpd.0.0.1:ARMCC GCC IAR
# description: 
if((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR"))
  set(generated_STMicroelectronics.usbpd.0.0.1_ARMCC_GCC_IAR "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_ARMCC_GCC_IAR enabled")
else()
  set(generated_STMicroelectronics.usbpd.0.0.1_ARMCC_GCC_IAR 0)
endif()


# condition: generated_STMicroelectronics.usbpd.0.0.1:M0PLUS
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M0+")))
  set(generated_STMicroelectronics.usbpd.0.0.1_M0PLUS "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_M0PLUS enabled")
else()
  set(generated_STMicroelectronics.usbpd.0.0.1_M0PLUS 0)
endif()


# condition: generated_STMicroelectronics.usbpd.0.0.1:M33
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M33")))
  set(generated_STMicroelectronics.usbpd.0.0.1_M33 "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_M33 enabled")
else()
  set(generated_STMicroelectronics.usbpd.0.0.1_M33 0)
endif()


# condition: generated_STMicroelectronics.usbpd.0.0.1:M55
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M55")))
  set(generated_STMicroelectronics.usbpd.0.0.1_M55 "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_M55 enabled")
else()
  set(generated_STMicroelectronics.usbpd.0.0.1_M55 0)
endif()


# condition: generated_STMicroelectronics.usbpd.0.0.1:M7
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M7")))
  set(generated_STMicroelectronics.usbpd.0.0.1_M7 "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_M7 enabled")
else()
  set(generated_STMicroelectronics.usbpd.0.0.1_M7 0)
endif()


# condition: generated_STMicroelectronics.usbpd.0.0.1:M85
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M85")))
  set(generated_STMicroelectronics.usbpd.0.0.1_M85 "1")
  message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_M85 enabled")
else()
  set(generated_STMicroelectronics.usbpd.0.0.1_M85 0)
endif()


# condition: generated_STMicroelectronics.usbpd.0.0.1:Use TCPP01 pwr if
# description: 
set(generated_STMicroelectronics.usbpd.0.0.1_Use_TCPP01_pwr_if "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_Use_TCPP01_pwr_if enabled")


# condition: generated_STMicroelectronics.usbpd.0.0.1:Use TCPP0203 pwr if
# description: 
set(generated_STMicroelectronics.usbpd.0.0.1_Use_TCPP0203_pwr_if "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:STM32CubeMX2 Codegen#.*Csub:pwr_tcpp0203_if(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_Use_TCPP0203_pwr_if enabled")


# condition: generated_STMicroelectronics.usbpd.0.0.1:Use custom pwr if
# description: 
set(generated_STMicroelectronics.usbpd.0.0.1_Use_custom_pwr_if "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_Use_custom_pwr_if enabled")


# condition: generated_STMicroelectronics.usbpd.0.0.1:Use usb if
# description: 
set(generated_STMicroelectronics.usbpd.0.0.1_Use_usb_if "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_Use_usb_if enabled")


# condition: generated_STMicroelectronics.usbpd.0.0.1:Use vdm user app
# description: 
set(generated_STMicroelectronics.usbpd.0.0.1_Use_vdm_user_app "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_Use_vdm_user_app enabled")


# condition: generated_STMicroelectronics.usbpd.0.0.1:usbpd core condition
# description: USBPD Core dependency
set(generated_STMicroelectronics.usbpd.0.0.1_usbpd_core_condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBPD#.*Csub:Core(#.*|$)>,>>,$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBPD#.*Csub:Lib#.*Cvariant:USBPD_PD3FULL(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:STM32CubeMX2 Codegen#.*Csub:pdo_defs(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBPD#.*Csub:Core timerserver(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBPD Interface#.*Csub:timerserver(#.*|$)>,>>>,$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBPD#.*Csub:Utils tracer(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBPD Interface#.*Csub:trace(#.*|$)>,>>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:STM32CubeMX2 Codegen#.*Csub:pwr_tcpp0203_if(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBPD Interface#.*Csub:phy(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_usbpd_core_condition enabled")


# condition: generated_STMicroelectronics.usbpd.0.0.1:usbpd lib condition
# description: 
set(generated_STMicroelectronics.usbpd.0.0.1_usbpd_lib_condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBPD#.*Csub:Lib#.*Cvariant:USBPD_PD3FULL(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:STM32CubeMX2 Codegen#.*Csub:pdo_defs(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBPD#.*Csub:Core timerserver(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBPD Interface#.*Csub:timerserver(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_usbpd_lib_condition enabled")


# condition: generated_STMicroelectronics.usbpd.0.0.1:usbpd tim peripheral condition
# description: USBPD TIM interface dependency
set(generated_STMicroelectronics.usbpd.0.0.1_usbpd_tim_peripheral_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:TIM(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_usbpd_tim_peripheral_condition enabled")


# condition: generated_STMicroelectronics.usbpd.0.0.1:usbpd uart peripheral condition
# description: USBPD UART interface dependency
set(generated_STMicroelectronics.usbpd.0.0.1_usbpd_uart_peripheral_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:UART(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_usbpd_uart_peripheral_condition enabled")


# condition: generated_STMicroelectronics.usbpd.0.0.1:usbpd ucpd peripheral condition
# description: USBPD UCPD interface dependency
set(generated_STMicroelectronics.usbpd.0.0.1_usbpd_ucpd_peripheral_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:UCPD(#.*|$)>,>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_usbpd_ucpd_peripheral_condition enabled")


# condition: generated_STMicroelectronics.usbpd.0.0.1:usbpd utils monitor condition
# description: USBPD Utils monitor dependency
set(generated_STMicroelectronics.usbpd.0.0.1_usbpd_utils_monitor_condition "1")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_usbpd_utils_monitor_condition enabled")


# condition: generated_STMicroelectronics.usbpd.0.0.1:usbpd utils tracer condition
# description: 
set(generated_STMicroelectronics.usbpd.0.0.1_usbpd_utils_tracer_condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBPD#.*Csub:Utils tracer(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBPD Interface#.*Csub:trace(#.*|$)>,>>>")
message(DEBUG "CMSIS condition generated_STMicroelectronics.usbpd.0.0.1_usbpd_utils_tracer_condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:STM32CubeMX2 Codegen#Csub:pdo_defs#Cversion:2.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_USB_STM32CubeMX2_Codegen_pdo_defs_2_0_0")
  target_compile_definitions(generated_STMicroelectronics_usbpd_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbpd.0.0.1_usbpd_lib_condition}:-DCMSIS_USE_generated_USB_STM32CubeMX2_Codegen_pdo_defs_2_0_0=1>")
  target_sources(generated_STMicroelectronics_usbpd_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbpd.0.0.1_usbpd_lib_condition}:${CMAKE_CURRENT_LIST_DIR}/usbpd_pdo_defs.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:STM32CubeMX2 Codegen#Csub:pwr_tcpp0203_if#Cversion:2.0.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_USB_STM32CubeMX2_Codegen_pwr_tcpp0203_if_2_0_0")
  target_compile_definitions(generated_STMicroelectronics_usbpd_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbpd.0.0.1_Use_TCPP0203_pwr_if}:-DCMSIS_USE_generated_USB_STM32CubeMX2_Codegen_pwr_tcpp0203_if_2_0_0=1>")
  target_include_directories(generated_STMicroelectronics_usbpd_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbpd.0.0.1_Use_TCPP0203_pwr_if}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_usbpd_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbpd.0.0.1_Use_TCPP0203_pwr_if}:${CMAKE_CURRENT_LIST_DIR}/mx_usbpd_tcpp0203_pwr_hw_if.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:STM32CubeMX2 Config#Csub:USBPD#Cversion:0.1.0#generated:true" IN_LIST CMSIS_COMPONENTS_LIST)  # TO BE DEFINED
  message(DEBUG "Using component generated_USB_STM32CubeMX2_Config_USBPD_0_1_0")
  target_compile_definitions(generated_STMicroelectronics_usbpd_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbpd.0.0.1_usbpd_core_condition}:-DCMSIS_USE_generated_USB_STM32CubeMX2_Config_USBPD_0_1_0=1>")
  target_include_directories(generated_STMicroelectronics_usbpd_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbpd.0.0.1_usbpd_core_condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(generated_STMicroelectronics_usbpd_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbpd.0.0.1_usbpd_core_condition}:${CMAKE_CURRENT_LIST_DIR}/mx_usbpd_app.c>")
  target_sources(generated_STMicroelectronics_usbpd_0_0_1 INTERFACE "$<${generated_STMicroelectronics.usbpd.0.0.1_usbpd_core_condition}:${CMAKE_CURRENT_LIST_DIR}/mx_usbpd_dpm_user_app.c>")
endif()

