# file-format: 1.0.0
cmake_minimum_required(VERSION 3.30)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

include(cmake/target.cmake)
include(cmake/${TOOLCHAIN_FILE})

project(oemirot_dualslot_hwcrypto)
add_executable(oemirot_dualslot_hwcrypto)
include(cmake/files.cmake)
include(cmake/flags.cmake)
include(cmake/components.cmake)

# Stubs to be able to add compiler flag overrides, e.g. -Os
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
  # Add additional source files here
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional include directories here
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional defined symbols here
)

target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional compiler flags here
)

target_link_options(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional linker flags here
)

target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional linker search directories here
)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
  # Add additional linker libraries here
)

# OEMiRoT prebuild hook: generates flash_layout.h, linker scripts, and other derived artifacts
set(PROJECT_SCRIPT "${CMAKE_SOURCE_DIR}/../oemirot/project_oemirot.py")
get_filename_component(PROJECT_SCRIPT "${PROJECT_SCRIPT}" ABSOLUTE)
get_filename_component(PROJECT_SCRIPT_DIR "${PROJECT_SCRIPT}" DIRECTORY)

if(NOT EXISTS "${PROJECT_SCRIPT}")
  message(FATAL_ERROR "Python prebuild script not found: ${PROJECT_SCRIPT}")
endif()

add_custom_target(prebuild
  COMMAND python ${PROJECT_SCRIPT} prebuild --compiler ${CMSIS_Tcompiler}
  WORKING_DIRECTORY ${PROJECT_SCRIPT_DIR}
  COMMENT "OEMiRoT prebuild"
)

add_dependencies(${CMAKE_PROJECT_NAME} prebuild)

# OEMiRoT post-build: generate HEX next to the target then copy into shared bin/ directory
find_program(CMAKE_OBJCOPY arm-none-eabi-objcopy REQUIRED)
find_program(CMAKE_SIZE arm-none-eabi-size REQUIRED)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
  COMMAND ${CMAKE_OBJCOPY} -O ihex
          $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
          $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/$<TARGET_FILE_BASE_NAME:${CMAKE_PROJECT_NAME}>.hex
  COMMAND ${CMAKE_COMMAND} -E make_directory
          ${CMAKE_SOURCE_DIR}/../bin
  COMMAND ${CMAKE_COMMAND} -E copy
          $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/$<TARGET_FILE_BASE_NAME:${CMAKE_PROJECT_NAME}>.hex
          ${CMAKE_SOURCE_DIR}/../bin/oemirot.hex
  COMMENT "Printing size, generating HEX file, and copying it to ../bin/oemirot.hex"
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/../oemirot/user_modifiable/Device/STM32G564RET3/system_stm32g5xx.c
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    "-T${CMAKE_SOURCE_DIR}/../oemirot/user_modifiable/Device/STM32G564RET3/stm32g564xe_flash.ld"
)
