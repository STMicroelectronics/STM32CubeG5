# file-format: 1.0.0
if(CMAKE_BUILD_TYPE STREQUAL "release_GCC_NUCLEO-G564RE")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/com.c
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/common.c
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/data.c
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/fw_update_app.c
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/low_level_device.c
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/low_level_flash.c
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/low_level_obkeys.c
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/main.c
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/ymodem.c
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/Driver_Common.h
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/Driver_Flash.h
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/appli_flash_layout.h
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/com.h
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/common.h
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/data.h
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/fw_update_app.h
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/low_level_flash.h
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/low_level_obkeys.h
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/main.h
    ${CMAKE_CURRENT_LIST_DIR}/../../appli/ymodem.h
  )
endif()
