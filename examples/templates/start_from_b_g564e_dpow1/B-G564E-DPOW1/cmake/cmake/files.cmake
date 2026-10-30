# file-format: 1.0.0
if(CMAKE_BUILD_TYPE STREQUAL "release_GCC_B-G564E-DPOW1")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ../../application/main.c ../../application/main.h)
elseif(CMAKE_BUILD_TYPE STREQUAL "debug_GCC_B-G564E-DPOW1")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ../../application/main.c ../../application/main.h)
endif()
