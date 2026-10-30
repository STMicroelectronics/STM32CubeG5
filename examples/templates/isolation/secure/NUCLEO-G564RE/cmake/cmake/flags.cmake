# file-format: 1.0.0
if(CMAKE_BUILD_TYPE STREQUAL "release_GCC_NUCLEO-G564RE")
  set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX ".elf")
  target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC ${CPU_FLAGS})
  target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC "SHELL:-Os -fdata-sections -ffunction-sections -std=gnu11 -Wall -fstack-usage --specs=nano.specs --specs=nosys.specs -g -Werror=implicit-function-declaration ${CC_SECURE}")
  target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ../user_modifiable/_release_GCC_NUCLEO-G564RE ../../../../../../arch/cmsis/CMSIS/Core/Include ../generated/hal ../../../../../../stm32g5xx_dfp/Include ../../../../../../stm32g5xx_drivers/hal ../../../../../../stm32g5xx_drivers/ll ../../../../../../stm32g5xx_drivers/timebases ../../application ../../../secure_nsclib)
  target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC STM32G564xx _RTE_)
  target_link_options(${CMAKE_PROJECT_NAME} PUBLIC -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -Wl,--start-group -lc -lm -Wl,--end-group -static --specs=nano.specs --specs=nosys.specs ${CC_SECURE})
  target_link_options(${CMAKE_PROJECT_NAME} PUBLIC ${CPU_FLAGS})
elseif(CMAKE_BUILD_TYPE STREQUAL "debug_GCC_NUCLEO-G564RE")
  set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES SUFFIX ".elf")
  target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC ${CPU_FLAGS})
  target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC "SHELL:-g3 -O0 -fdata-sections -ffunction-sections -std=gnu11 -Wall -fstack-usage --specs=nano.specs --specs=nosys.specs -Werror=implicit-function-declaration ${CC_SECURE}")
  target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ../user_modifiable/_debug_GCC_NUCLEO-G564RE ../../../../../../arch/cmsis/CMSIS/Core/Include ../generated/hal ../../../../../../stm32g5xx_dfp/Include ../../../../../../stm32g5xx_drivers/hal ../../../../../../stm32g5xx_drivers/ll ../../../../../../stm32g5xx_drivers/timebases ../../application ../../../secure_nsclib)
  target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC STM32G564xx _RTE_)
  target_link_options(${CMAKE_PROJECT_NAME} PUBLIC -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -Wl,--start-group -lc -lm -Wl,--end-group -static --specs=nano.specs --specs=nosys.specs ${CC_SECURE})
  target_link_options(${CMAKE_PROJECT_NAME} PUBLIC ${CPU_FLAGS})
endif()

set(NSCLIB_DIR "${CMAKE_SOURCE_DIR}/../../../secure_nsclib/NUCLEO-G564RE/${BUILD_TYPE}")

# Redirect ELF output to secure_nsclib so the non-secure build can pick it up
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${NSCLIB_DIR}")

# Generate the CMSE gateway veneer library in secure_nsclib
target_link_options(${CMAKE_PROJECT_NAME} PUBLIC
    -Wl,--cmse-implib
    "-Wl,--out-implib,${NSCLIB_DIR}/secure_nsclib.o")

# Clean build artifacts
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map")
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${NSCLIB_DIR}/${CMAKE_PROJECT_NAME}.elf")
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES "${NSCLIB_DIR}/secure_nsclib.o")
