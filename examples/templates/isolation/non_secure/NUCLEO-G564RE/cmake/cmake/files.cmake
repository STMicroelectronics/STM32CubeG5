# file-format: 1.0.0
if(CMAKE_BUILD_TYPE STREQUAL "release_GCC_NUCLEO-G564RE")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ../../application/main.c ../../application/main.h ../generated/hal/mx_icache.c)
elseif(CMAKE_BUILD_TYPE STREQUAL "debug_GCC_NUCLEO-G564RE")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ../../application/main.c ../../application/main.h ../generated/hal/mx_icache.c)
endif()

# NOTE: mx_icache.c is added explicitly above because the checked-in
# generated/hal/STM32_HAL_Driver_codegen.cmake for this board predates ICACHE
# codegen support and never references it, even though mx_system.c calls
# mx_icache_init()/mx_icache_gethandle(). Regenerating this example for
# NUCLEO-G564RE via STM32CubeMX2/CEB would refresh that script and make this
# workaround unnecessary.
