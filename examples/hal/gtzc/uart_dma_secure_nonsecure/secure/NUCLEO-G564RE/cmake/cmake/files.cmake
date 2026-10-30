# file-format: 1.0.0
if(CMAKE_BUILD_TYPE STREQUAL "release_GCC_NUCLEO-G564RE")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ../../application/main.c ../../application/main.h ../../application/example.c ../../application/example.h ../../application/secure_nsclib.c)
elseif(CMAKE_BUILD_TYPE STREQUAL "debug_GCC_NUCLEO-G564RE")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ../../application/main.c ../../application/main.h ../../application/example.c ../../application/example.h ../../application/secure_nsclib.c)
endif()
