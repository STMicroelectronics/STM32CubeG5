# file-format: 1.0.0
if(CMAKE_BUILD_TYPE STREQUAL "release_GCC_NUCLEO-G564RE")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ../../application/main_gtzc.c ../../application/main.h ../../application/secure_nsclib.c ../generated/hal/mx_gtzc_tee.c)
elseif(CMAKE_BUILD_TYPE STREQUAL "debug_GCC_NUCLEO-G564RE")
  target_sources(${CMAKE_PROJECT_NAME} PRIVATE ../../application/main_gtzc.c ../../application/main.h ../../application/secure_nsclib.c ../generated/hal/mx_gtzc_tee.c)
endif()

# NOTE: mx_gtzc_tee.c is added explicitly above because the checked-in
# generated/hal/STM32_HAL_Driver_codegen.cmake for this board predates the
# GTZC/TEE codegen split and does not reference it (it still expects a plain
# mx_gtzc.c that STM32CubeMX2 no longer generates for TrustZone-enabled
# devices). Regenerating this example for NUCLEO-G564RE via STM32CubeMX2/CEB
# would refresh that script and make this workaround unnecessary.
# mx_cortex_systick.c is intentionally NOT added: mx_system.c already defines
# SysTick_Handler() (calling both HAL_IncTick() and
# HAL_CORTEX_SYSTICK_IRQHandler()), so adding mx_cortex_systick.c would cause
# a "multiple definition of SysTick_Handler" link error.
