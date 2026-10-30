# file-format: 1.0.0
if(CMAKE_BUILD_TYPE STREQUAL "release_GCC_NUCLEO-G564RE")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/boot_hal.c
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/boot_hal_cfg.h
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/boot_hal_flowcontrol.h
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/boot_hal_hash_ref.c
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/boot_hal_hash_ref.h
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/boot_hal_imagevalid.h
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/boot_hal_legacy.h
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/boot_hal_utils.h
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/boot_nv_counters.c
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/cmsis.h
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/flash_layout.h
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/keys_map.c
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/low_level_device.c
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/low_level_flash.c
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/low_level_flash.h
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/low_level_obkeys.c
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/low_level_obkeys.h
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/low_level_obkeys_device.c
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/low_level_rng.c
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/low_level_rng.h
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/low_level_security.c
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/low_level_security.h
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/low_level_security_config.h
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/mcuboot_config.h
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/region_defs.h
    ${CMAKE_CURRENT_LIST_DIR}/../../oemirot/tick.c
  )
endif()
