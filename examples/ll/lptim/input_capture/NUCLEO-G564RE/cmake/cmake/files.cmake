# file-format: 1.0.0
if(CMAKE_BUILD_TYPE STREQUAL "release_GCC_NUCLEO-G564RE")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ../../application/example.c ../../application/main.c ../../application/stm32g5xx_ll_example.c ../../application/example.h ../../application/main.h ../../application/ll_example.h)
elseif(CMAKE_BUILD_TYPE STREQUAL "debug_GCC_NUCLEO-G564RE")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ../../application/example.c ../../application/main.c ../../application/stm32g5xx_ll_example.c ../../application/example.h ../../application/main.h ../../application/ll_example.h)
endif()
