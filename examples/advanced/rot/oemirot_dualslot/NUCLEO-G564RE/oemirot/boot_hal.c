/**
  ******************************************************************************
  * @file    boot_hal.c
  * @brief   This file contains  mcuboot hardware specific implementation
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023-2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "mx_system.h"
#include "boot_hal_cfg.h"
#include "boot_hal.h"
#include "boot_hal_hash_ref.h"
#include "boot_hal_imagevalid.h"
#include "boot_hal_flowcontrol.h"
#include "mcuboot_config.h"
#include "low_level_security.h"
#include "cmsis.h"
#include "low_level_flash.h"
#include "region_defs.h"
#include "low_level_rng.h"
#include "boot_hal_utils.h"
#include "low_level_obkeys.h"

#include "basic_stdio_core.h"
#include "bootutil/boot_record.h"
#include "bootutil_priv.h"
#include "mx_basic_stdio_app.h"
#include "mx_hal_def.h"

#if defined(MCUBOOT_DOUBLE_SIGN_VERIF)
#if (MCUBOOT_IMAGE_NUMBER == 1)
#define OEMIROT_BOOT_IMAGE_NUMBER 1
#elif (MCUBOOT_IMAGE_NUMBER == 2)
#define OEMIROT_BOOT_IMAGE_NUMBER 2
#elif (MCUBOOT_IMAGE_NUMBER == 3)
#define OEMIROT_BOOT_IMAGE_NUMBER 3
#elif (MCUBOOT_IMAGE_NUMBER == 4)
#define OEMIROT_BOOT_IMAGE_NUMBER 4
#else
#error "MCUBOOT_IMAGE_NUMBER not supported"
#endif /* (MCUBOOT_IMAGE_NUMBER == 1)  */

/* Global variables to memorize images validation status */
uint8_t ImageValidStatus[OEMIROT_BOOT_IMAGE_NUMBER] = { INIT_STRUCT(OEMIROT_BOOT_IMAGE_NUMBER, IMAGE_INVALID) };

uint8_t ImageValidEnable = 0U;
uint8_t ImageValidIndex = 0U;
#endif /* MCUBOOT_DOUBLE_SIGN_VERIF */

#define OEMIROT_APPLI_REGION_TO_X ((uint32_t) HAL_CORTEX_MPU_REGION_1)

#if defined(OEMIROT_FLOW_CONTROL)
/* Global variable for Flow Control state */
volatile uint32_t uFlowProtectValue = FLOW_CTRL_INIT_VALUE;
#endif /* OEMIROT_FLOW_CONTROL */
volatile uint32_t uFlowStage = FLOW_STAGE_CFG;

/* External References -------------------------------------------------------*/
extern void SystemInit(void);
extern __NO_RETURN void __PROGRAM_START(void);
extern uint32_t __STACK_LIMIT;

/* Private function prototypes -----------------------------------------------*/
__NO_RETURN void Reset_Handler(void);
void Default_IRQHandler_Hook(void);
void HardFault_Handler(void);
/* Reset_Handler_C: C continuation entered once SRAM1 is confirmed ready.
 * - noinline: prevents the compiler from merging its C prologue (PUSH) into the naked
 *             Reset_Handler, which would write to SRAM before SRAMBUSY clears.
 * - used    : the only call site is 'bl Reset_Handler_C' inside an asm string, which is
 *             invisible to the C-level call graph; without this attribute the function
 *             would be silently eliminated as dead code by the optimizer or Link-Time Optimization. */
static __attribute__((noinline, used)) __NO_RETURN void Reset_Handler_C(void);
static void boot_jump_to_RSSLIB(uint32_t boot_jump_addr1, uint32_t boot_jump_addr2,
                                uint32_t vector_table, uint32_t mpu_region);

/* ---------------------------------------------------------------------------
 * OEMiRoT executes using SRAM1, which is configured as "erase on reset".
 * Execution (stack use) must not start until the SRAM erase is complete.
 * The ASM startup below polls RAMCFG_SRAM1->ISR.SRAMBUSY before
 * branching to Reset_Handler_C, which is the first code allowed to use the stack.
 * ---------------------------------------------------------------------------*/
#define RESET_RCC_AHB_ENR_ADDR        0x54020C88
#define RESET_RCC_RAMCFG_EN_MASK      0x00020000
#define RESET_RAMCFG_ROT_ISR_ADDR     0x50026008
#define RESET_SRAMBUSY_MASK           0x100

/**
  * @brief  This function is the Reset Handler called on controller reset.
  *
  * The naked attribute eliminates all compiler-generated prologue/epilogue so
  * that no stack write occurs before SRAM1 availability is confirmed.
  * @param  None
  * @retval None
  */
/* IAR Pe1305: naked function + __NO_RETURN - IAR cannot trace that 'b .' is an
 * unreachable infinite loop after 'bl Reset_Handler_C', so it incorrectly
 * reports that the noreturn function will return. The b . is a safety trap. */
#if defined(__ICCARM__)
#pragma diag_suppress = Pe1305
#endif /* __ICCARM__ */
__NO_RETURN void __attribute__((naked)) Reset_Handler(void)
{
  __ASM volatile(
#if !defined(__ICCARM__)
    ".syntax unified                      \n"
#endif /* !__ICCARM__ */
    /* Step 1: Enable RAMCFG clock in RCC->AHB1ENR (register-only, no stack) */
    "ldr     r0, =" ASM_STR(RESET_RCC_AHB_ENR_ADDR) "         \n"
    "ldr     r1, [r0]                     \n"
    "orr     r1, r1, #" ASM_STR(RESET_RCC_RAMCFG_EN_MASK) "   \n"
    "str     r1, [r0]                     \n"
    "ldr     r1, [r0]                     \n" /* Dummy read: wait for clocking    */
    /* Step 2: Poll RAMCFG_SRAM1->ISR.SRAMBUSY   */
    "ldr     r0, =" ASM_STR(RESET_RAMCFG_ROT_ISR_ADDR) "      \n"
    "wait_srambusy:                       \n"
    "  ldr     r1, [r0]                   \n"
    "  tst     r1, #" ASM_STR(RESET_SRAMBUSY_MASK) "           \n"
    "  bne     wait_srambusy              \n"
    /* Step 3: SRAM ready - call C continuation (uses stack safely)            */
    "bl      Reset_Handler_C              \n"
    "b       .                            \n"
  );
}
#if defined(__ICCARM__)
#pragma diag_default = Pe1305
#endif /* __ICCARM__ */

/**
  * @brief  C continuation of Reset_Handler, entered once SRAM1 is confirmed ready.
  * @param  None
  * @retval None
  */
static __attribute__((noinline, used)) __NO_RETURN void Reset_Handler_C(void)
{
  /* Configure DWT to enable the cycle counter for boot-time performance measurement */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk | CoreDebug_DEMCR_MON_EN_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  __set_MSPLIM((uint32_t)(&__STACK_LIMIT));

  /* Early startup */
  SECURITY_ApplyRamCfgEarlyStartup();
  SECURITY_ApplyTamperEarlyStartup();

  SystemInit();         /* CMSIS System Initialization */

  __PROGRAM_START();    /* Enter PreMain (C library entry point) */
}

/**
  * @brief  This function is the default IRQ handler hook
  *         when the IRQ line is not used by the application.
  * @param  None
  * @retval None
  */
void Default_IRQHandler_Hook(void)
{
  Error_Handler();
}

/**
  * @brief  Hard Fault Handler.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  Error_Handler();
}

/**
  * @brief  Jumping to next image using RSSLIB service, after erasing SRAM twice.
  * @param  boot_jump_addr1 boot_jump_to_RSSLIB address in order to execute twice SRAM erasing
  * @param  boot_jump_addr2 RSSLIB service address
  * @param  vector table vector table of the next image
  * @param  mpu_region MPU region to be enabled
  */
__attribute__((naked)) void boot_jump_to_RSSLIB(uint32_t boot_jump_addr1, uint32_t boot_jump_addr2,
                                                uint32_t vector_table, uint32_t mpu_region)
{
  __ASM volatile(
#if !defined(__ICCARM__)
    ".syntax unified                 \n"
#endif /* !__ICCARM__ */
    "mov     r7, r0                  \n"
    "mov     r8, r1                  \n"
    "mov     r9, r2                  \n"
    "mov     r10, r3                 \n"
    "bl      boot_clear_ram_area     \n" /* Clear RAM before jump */
    "movs    r0, #0                  \n" /* Clear registers: R0-R12, */
    "mov     r1, r0                  \n" /* except R7/R8/R9/R10 */
    "mov     r2, r0                  \n"
    "mov     r3, r0                  \n"
    "mov     r4, r0                  \n"
    "mov     r5, r0                  \n"
    "mov     r6, r0                  \n"
    "mov     r11, r0                 \n"
    "mov     r12, r0                 \n"
    "mov     lr,  r0                 \n"
    "mov     r0, r8                  \n"
    "mov     r1, r9                  \n"
    "mov     r2, r10                 \n"
    "mov     r8, r3                  \n"
    "mov     r9, r3                  \n"
    "mov     r10, r3                 \n"
    "bx      r7                      \n" /* Jump to RSSLIB service */
  );
}

#if defined(OEMIROT_FAST_WAKE_UP)
/**
  * @brief This function checks if system wakes-up from low-power mode:
  *        Stand-by mode and shut down mode (depending on HW capability).
  * @note
  * @retval FIH_SUCCESS in case of wake-up
  */
fih_ret boot_platform_wakeup(void)
{
  fih_ret fih_rc = FIH_FAILURE;

  /* Check Stand-By Flag */
  if (HAL_PWR_GetPreviousSystemPowerMode() == HAL_PWR_SYSTEM_STANDBY_MODE)
  {
    fih_rc = FIH_SUCCESS;
  }

  FIH_RET(fih_rc);
}

/**
  * @brief This function returns the configured secure application vector table.
  * @param vt  pointer to secure application vector table descriptor
  * @retval FIH_SUCCESS in case of vt has well been set
  */
fih_ret boot_platform_vector_table(struct boot_arm_vector_table **vt)
{
  fih_ret fih_rc = FIH_FAILURE;

  *vt = (struct boot_arm_vector_table *)(CODE_START);
  fih_rc = FIH_SUCCESS;

  FIH_RET(fih_rc);
}
#endif /* OEMIROT_FAST_WAKE_UP */

/**
  * @brief This function manage the jump to secure application.
  * @note
  * @retval void
  */
void boot_platform_quit(struct boot_arm_vector_table *vector)
{
  struct boot_arm_vector_table *vt = NULL;
  uint32_t rsslib_jump_hdp_lvl2 = 0U;
#if defined(MCUBOOT_DOUBLE_SIGN_VERIF)
  uint32_t image_index;

  (void)fih_delay();
#if defined(OEMIROT_FAST_WAKE_UP)
  fih_ret fih_rc = FIH_FAILURE;

  /* Check if wake-up from low-power to bypass images revalidation */
  FIH_CALL(boot_platform_wakeup, fih_rc);
  if (FIH_NOT_EQ(fih_rc, FIH_SUCCESS))
#endif /* OEMIROT_FAST_WAKE_UP */
  {
    /* Check again if images have been validated, to resist to basic hw attacks */
    for (image_index = 0U; image_index < MCUBOOT_IMAGE_NUMBER; image_index++)
    {
      if (ImageValidStatus[image_index] != IMAGE_VALID)
      {
        ROT_LOG_ERR("Error while double controlling images validation");
        Error_Handler();
      }
    }
  }
#endif /* MCUBOOT_DOUBLE_SIGN_VERIF */

  /* Init RSS jump function descriptor */
  rsslib_jump_hdp_lvl2 = (uintptr_t)(RSSLIB_PFUNC->S.JumpHDPLvl2);

#if defined(MCUBOOT_USE_HASH_REF)
  /* Store new hash references in flash for next boot */
  if (boot_hash_ref_store() != 0)
  {
    ROT_LOG_ERR("Error while storing hash references");
    Error_Handler();
  }
#endif /* MCUBOOT_USE_HASH_REF */

  /* Check Flow control state */
  FLOW_CONTROL_CHECK(uFlowProtectValue, FLOW_CTRL_STAGE_2);
  uFlowStage = FLOW_STAGE_CFG;

  /* Invalidate ICache before jumping to application */
  hal_icache_handle_t *p_icache = mx_rot_icache_gethandle();
  (void)HAL_ICACHE_Invalidate(p_icache);

  /* Update run time protections for application execution */
  SECURITY_UpdateRunTimeProtections();

  vt = (struct boot_arm_vector_table *)vector;

  /* Check Flow control state */
  FLOW_CONTROL_CHECK(uFlowProtectValue, FLOW_CTRL_STAGE_3_A);
  uFlowStage = FLOW_STAGE_CHK;

  /* Double the update of run time protections, to resist to basic hardware attacks */
  SECURITY_UpdateRunTimeProtections();

  /* Disable the cryptography clocks */
  SECURITY_DisableCryptoClocks();

  /* Check Flow control for dynamic protections */
  FLOW_CONTROL_CHECK(uFlowProtectValue, FLOW_CTRL_STAGE_4_A);
  /*  change stack limit  */
  __set_MSPLIM(0);

  boot_jump_to_RSSLIB((uintptr_t)&boot_jump_to_RSSLIB, rsslib_jump_hdp_lvl2, (uint32_t) vt,
                      OEMIROT_APPLI_REGION_TO_X);

  /* Avoid compiler to pop registers after having changed MSP */
#if !defined(__ICCARM__)
  __builtin_unreachable();
#else
  for (;;) {}
#endif /* defined(__ICCARM__) */
}

/**
  * @brief This function is called to clear all RAM area before jumping in
  * in Secure application .
  * @note All toolchains: naked suppresses prologue/epilogue unconditionally.
  *       Any push/pop generated by a non-naked function would corrupt the stack
  *       region being zeroed, causing a fault on return.
  *       Constants are embedded as numeric literals via preprocessor stringification
  *       (ROT_DATA_START_ASM / ROT_DATA_SIZE_ASM in region_defs.h) so no linker
  *       symbol is required - this works identically for GCC, IAR, and AC6.
  * @retval void
  */
__attribute__((naked)) void boot_clear_ram_area(void)
{
  __ASM volatile(
#if !defined(__ICCARM__)
    ".syntax unified                 \n"
#endif /* !__ICCARM__ */
    "ldr     r0, =" ASM_STR(ROT_DATA_START_ASM) " \n"
    "ldr     r1, =" ASM_STR(ROT_DATA_SIZE_ASM)  " \n"
    "lsrs    r1, r1, #2              \n" /* count = size / 4 */
    "movs    r2, #0                  \n"
    "clear_ram_loop:                 \n"
    "str     r2, [r0], #4            \n" /* *pt++ = 0 */
    "subs    r1, r1, #1              \n"
    "bne     clear_ram_loop          \n"
    "bx      lr                      \n" /* return to caller */
  );
}

/**
  * @brief  Platform init.
  * @param  None
  * @retval status
  */
int32_t boot_platform_init(void)
{
  /* Enable the cryptography clocks */
  SECURITY_EnableCryptoClocks();

  /**
    * System Init: this code placed in targets folder initializes your system.
    * It calls the initialization (and sets the initial configuration) of the peripherals.
    * You can use STM32CubeMX to generate and call this code or not in this project.
    * It also contains the HAL initialization and the initial clock configuration.
    */
  if (mx_system_init() != SYSTEM_OK)
  {
    Error_Handler();
  }

#if defined(MCUBOOT_HAVE_LOGGING)
  /* Initialize basic_stdio separately, but after system init. */
  (void)mx_basic_stdio_init();
#endif /* MCUBOOT_HAVE_LOGGING */

  ROT_LOG_INF("Starting OEMiRoT");

  /* Display the system core clock frequency */
  ROT_LOG_INF("SystemCoreClock: %" PRIu32 " Hz", SystemCoreClock);

  /* Start HW randomization */
  RNG_Init();

  (void)fih_delay_init();

  if (FLASH_DEV_NAME.Initialize(NULL) != ARM_DRIVER_OK)
  {
    ROT_LOG_ERR("Error while initializing Flash Interface");
    Error_Handler();
  }

  /* Apply Run time Protection */
  SECURITY_ApplyRunTimeProtections();
  /* Check static protections */
  SECURITY_CheckStaticProtections();

  /* Configure keys */
  OBK_ReadHdpl1Config(&OBK_Hdpl1Cfg);

  /* Check Flow control state */
  FLOW_CONTROL_CHECK(uFlowProtectValue, FLOW_CTRL_STAGE_1);
  uFlowStage = FLOW_STAGE_CHK;
  /* Double protections apply / check to resist to basic fault injections */
  /* Apply Run time Protection */
  (void)fih_delay();
  SECURITY_ApplyRunTimeProtections();
  /* Check static protections */
  SECURITY_CheckStaticProtections();

  /* Control Hdpl1 configuration */
  OBK_VerifyHdpl1Config(&OBK_Hdpl1Cfg);

#if defined(MCUBOOT_USE_HASH_REF)
  /* Load all images hash references (for mcuboot) */
  if (boot_hash_ref_load() != 0)
  {
    ROT_LOG_ERR("Error while loading Hash references from FLash");
    Error_Handler();
  }
#endif /* MCUBOOT_USE_HASH_REF */

  return 0;
}

/**
  * @brief  This function is executed in case of error occurrence.
  *         This function does not return.
  * @param  None
  * @retval None
  */
#if defined(OEMIROT_STOP_ON_ERROR)
/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".ROT_Error_Code"
#else
__attribute__((section(".ROT_Error_Code")))
#endif /* __ICCARM__ */
__NO_RETURN void Error_Handler(void)
{
  while (1);
}
#else /* OEMIROT_STOP_ON_ERROR */
/* Place code in a specific section */
#if defined(__ICCARM__)
#pragma default_function_attributes = @ ".ROT_Error_Code"
#else
__attribute__((section(".ROT_Error_Code")))
#endif /* __ICCARM__ */
__NO_RETURN void Error_Handler(void)
{
  /* It is customizable */
  NVIC_SystemReset();
#if !defined(__ICCARM__)
  /* Avoid bx lr instruction (for fault injection) */
  __builtin_unreachable();
#endif /* defined(__ICCARM__) */
}
#endif /* OEMIROT_STOP_ON_ERROR */

/* Stop placing data in specified section */
#if defined(__ICCARM__)
#pragma default_function_attributes =
#endif /* __ICCARM__ */

#if defined(__ARMCC_VERSION)
/* reimplement the function to reach Error Handler */
void __aeabi_assert(const char *expr, const char *file, int line)
{
#if defined(OEMIROT_DEV_MODE)
  ROT_LOG_INF("assertion \" %s \" failed: file %s %d\n", expr, file, line);
#endif /* OEMIROT_DEV_MODE */
  Error_Handler();
}
#endif  /*  __ARMCC_VERSION */
#if defined(USE_FULL_ASSERT)

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  Error_Handler();
}
#endif /* USE_FULL_ASSERT */

#if defined(__ARMCC_VERSION)
/** Reimplement the exit function to invoke the Error Handler.
  * This is necessary because the __ARMCC_VERSION toolchain uses the microlib,
  * which does not provide a default implementation of the exit function.
  * By redirecting exit calls to Error_Handler(), we ensure the system enters
  * a safe error state in case of unexpected termination.
  */
void exit(int status)
{
  Error_Handler();
}
#endif /* __ARMCC_VERSION */

/**
  * @brief  User hook function called after the HAL_Init() and Peripheral init functions.
  * @retval system_status_t System status
  */
system_status_t post_system_init_hook(void)
{
#if defined(OEMIROT_DEV_MODE)
  if (mx_rot_uart_init() == NULL)
  {
    return SYSTEM_PERIPHERAL_ERROR;
  }
#endif /* OEMIROT_DEV_MODE */

  return SYSTEM_OK;
}
