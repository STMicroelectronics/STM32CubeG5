# file-format: 1.0.0
if(CMAKE_BUILD_TYPE STREQUAL "release_GCC_NUCLEO-G564RE")
  set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX ".elf")
  target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC ${CPU_FLAGS})
  target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC "SHELL:-Os -fdata-sections -ffunction-sections -std=gnu11 -Wall -fstack-usage --specs=nano.specs --specs=nosys.specs -g -Werror=implicit-function-declaration -Wno-incompatible-pointer-types ${CC_SECURE} -include ${CMAKE_SOURCE_DIR}/../oemirot/user_modifiable/_release_GCC_NUCLEO-G564RE/Pre_Include_Global.h -include ${CMAKE_SOURCE_DIR}/../oemirot/boot_pre_include_global.h")
  target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ../oemirot/user_modifiable/_release_GCC_NUCLEO-G564RE ../../../../../../arch/cmsis/CMSIS/Core/Include ../oemirot ../oemirot/generated/hal ../oemirot/generated/middleware/stfcf ../oemirot/generated/utilities/basic_stdio ../../../../../../middleware/mbedtls/configs ../../../../../../middleware/mbedtls/include ../../../../../../middleware/mbedtls/include/mbedtls ../../../../../../middleware/mbedtls/include/psa ../../../../../../middleware/mbedtls/library ../../../../../../middleware/mcuboot/boot/bootutil/include ../../../../../../middleware/mcuboot/boot/bootutil/include/bootutil ../../../../../../middleware/mcuboot/boot/bootutil/include/bootutil/crypto ../../../../../../middleware/mcuboot/boot/bootutil/src ../../../../../../middleware/mcuboot/platform_common ../../../../../../middleware/stfcf/mbedtls_alt/include/common ../../../../../../middleware/stfcf/mbedtls_alt/include/hal ../../../../../../stm32g5xx_dfp/Include ../../../../../../stm32g5xx_drivers/hal ../../../../../../stm32g5xx_drivers/ll ../../../../../../utilities/basic_stdio ../../../../../../utilities/basic_stdio/interface_io)
  target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC USE_TRACE=0 STM32G564xx _RTE_)
  target_link_options(${CMAKE_PROJECT_NAME} PUBLIC -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -Wl,--start-group -lc -lm -Wl,--end-group -static --specs=nano.specs --specs=nosys.specs ${CC_SECURE})
  target_link_options(${CMAKE_PROJECT_NAME} PUBLIC ${CPU_FLAGS})
endif()

# Clean .map and .elf file
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/${CMAKE_PROJECT_NAME}.map")
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/${CMAKE_PROJECT_NAME}.elf")
