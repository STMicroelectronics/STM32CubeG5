# file-format: 1.0.0
cmake_minimum_required(VERSION 3.30)

# NONE: no compiler needed at this level; each sub-project sets up its own toolchain
project(uart_dma_secure_nonsecure NONE)

include(ExternalProject)

ExternalProject_Add(secure
  SOURCE_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/../../../secure/NUCLEO-G564RE/cmake"
  BINARY_DIR   "${CMAKE_BINARY_DIR}/secure"
  CMAKE_CACHE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DBUILD_TYPE:STRING=${BUILD_TYPE}
    -DTOOLCHAIN_FILE:STRING=${TOOLCHAIN_FILE}
  BUILD_ALWAYS  TRUE
  INSTALL_COMMAND ""
)

ExternalProject_Add(non_secure
  SOURCE_DIR   "${CMAKE_CURRENT_SOURCE_DIR}/../../../non_secure/NUCLEO-G564RE/cmake"
  BINARY_DIR   "${CMAKE_BINARY_DIR}/non_secure"
  CMAKE_CACHE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DBUILD_TYPE:STRING=${BUILD_TYPE}
    -DTOOLCHAIN_FILE:STRING=${TOOLCHAIN_FILE}
  BUILD_ALWAYS  TRUE
  INSTALL_COMMAND ""
  DEPENDS secure
)
