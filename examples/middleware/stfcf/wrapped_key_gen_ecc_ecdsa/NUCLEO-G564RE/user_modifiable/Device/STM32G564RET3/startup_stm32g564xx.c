/**
  ******************************************************************************
  * @file    startup_stm32g564xx.c
  * @brief   Startup File for STM32G564xx Devices
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32g564xx.h"

#ifdef __cplusplus
#error "startup_stm32g564xx.c file cannot be compiled with C++ compiler"
#endif /* __cplusplus */

/* External References -------------------------------------------------------*/
extern void SystemInit(void);
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
extern uint64_t __STACK_SEAL;
#endif /* __ARM_FEATURE_CMSE */

extern __NO_RETURN void __PROGRAM_START(void);

/* Private function prototypes -----------------------------------------------*/
/* ISR headers */
__NO_RETURN void Reset_Handler(void) __attribute__((weak));
            void Default_IRQHandler(void);
__NO_RETURN void Default_IRQHandler_Hook(void) __attribute__((weak));

/* Cortex-M interrupts */
void NMI_Handler                       (void) __attribute__((weak, alias("Default_IRQHandler")));
void HardFault_Handler                 (void) __attribute__((weak));
void MemManage_Handler                 (void) __attribute__((weak, alias("Default_IRQHandler")));
void BusFault_Handler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void UsageFault_Handler                (void) __attribute__((weak, alias("Default_IRQHandler")));
void SecureFault_Handler               (void) __attribute__((weak, alias("Default_IRQHandler")));
void SVC_Handler                       (void) __attribute__((weak, alias("Default_IRQHandler")));
void DebugMon_Handler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void PendSV_Handler                    (void) __attribute__((weak, alias("Default_IRQHandler")));
void SysTick_Handler                   (void) __attribute__((weak, alias("Default_IRQHandler")));

/* Device interrupts */
void WWDG_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void IWDG_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void PWR_PVD_AVD_IRQHandler            (void) __attribute__((weak, alias("Default_IRQHandler")));
void TAMP_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void GTZC1_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void FLASH_S_IRQHandler                (void) __attribute__((weak, alias("Default_IRQHandler")));
void FLASH_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void RAMCFG_IRQHandler                 (void) __attribute__((weak, alias("Default_IRQHandler")));
void RCC_S_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void RCC_IRQHandler                    (void) __attribute__((weak, alias("Default_IRQHandler")));
void ICACHE_IRQHandler                 (void) __attribute__((weak, alias("Default_IRQHandler")));
void DCACHE_IRQHandler                 (void) __attribute__((weak, alias("Default_IRQHandler")));
void FPU_IRQHandler                    (void) __attribute__((weak, alias("Default_IRQHandler")));
void RTC_S_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void RTC_IRQHandler                    (void) __attribute__((weak, alias("Default_IRQHandler")));
void EXTI0_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void EXTI1_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void EXTI2_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void EXTI3_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void EXTI4_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void EXTI5_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void EXTI6_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void EXTI7_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void EXTI8_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void EXTI9_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void EXTI10_IRQHandler                 (void) __attribute__((weak, alias("Default_IRQHandler")));
void EXTI11_IRQHandler                 (void) __attribute__((weak, alias("Default_IRQHandler")));
void EXTI12_IRQHandler                 (void) __attribute__((weak, alias("Default_IRQHandler")));
void EXTI13_IRQHandler                 (void) __attribute__((weak, alias("Default_IRQHandler")));
void EXTI14_IRQHandler                 (void) __attribute__((weak, alias("Default_IRQHandler")));
void EXTI15_IRQHandler                 (void) __attribute__((weak, alias("Default_IRQHandler")));
void GPDMA1_CH0_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void GPDMA1_CH1_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void GPDMA1_CH2_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void GPDMA1_CH3_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void GPDMA1_CH4_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void GPDMA1_CH5_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void GPDMA1_CH6_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void GPDMA1_CH7_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void SAES_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void ADC1_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void DAC1_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void FDCAN1_IT0_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void FDCAN1_IT1_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM1_BRK_TERR_IERR_IRQHandler     (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM1_UPD_IRQHandler               (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM1_TRGI_COM_DIR_IDX_IRQHandler  (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM1_CC_IRQHandler                (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM2_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM3_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM4_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM5_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM6_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM7_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void I2C1_EV_IRQHandler                (void) __attribute__((weak, alias("Default_IRQHandler")));
void I2C1_ERR_IRQHandler               (void) __attribute__((weak, alias("Default_IRQHandler")));
void I2C2_EV_IRQHandler                (void) __attribute__((weak, alias("Default_IRQHandler")));
void I2C2_ERR_IRQHandler               (void) __attribute__((weak, alias("Default_IRQHandler")));
void SPI1_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void SPI2_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void SPI3_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void USART1_IRQHandler                 (void) __attribute__((weak, alias("Default_IRQHandler")));
void USART2_IRQHandler                 (void) __attribute__((weak, alias("Default_IRQHandler")));
void USART3_IRQHandler                 (void) __attribute__((weak, alias("Default_IRQHandler")));
void UART4_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void UART5_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void LPUART1_IRQHandler                (void) __attribute__((weak, alias("Default_IRQHandler")));
void LPTIM1_IRQHandler                 (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM8_BRK_TERR_IERR_IRQHandler     (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM8_UPD_IRQHandler               (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM8_TRGI_COM_DIR_IDX_IRQHandler  (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM8_CC_IRQHandler                (void) __attribute__((weak, alias("Default_IRQHandler")));
void ADC2_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void ADC3_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM15_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM16_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM17_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void USB_DRD_FS_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void CRS_IRQHandler                    (void) __attribute__((weak, alias("Default_IRQHandler")));
void UCPD1_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void XSPI1_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM9_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void I2C3_EV_IRQHandler                (void) __attribute__((weak, alias("Default_IRQHandler")));
void I2C3_ERR_IRQHandler               (void) __attribute__((weak, alias("Default_IRQHandler")));
void UART6_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM20_BRK_TERR_IERR_IRQHandler    (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM20_UPD_IRQHandler              (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM20_TRGI_COM_DIR_IDX_IRQHandler (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM20_CC_IRQHandler               (void) __attribute__((weak, alias("Default_IRQHandler")));
void GPDMA2_CH0_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void GPDMA2_CH1_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void GPDMA2_CH2_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void GPDMA2_CH3_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void GPDMA2_CH4_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void GPDMA2_CH5_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void GPDMA2_CH6_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void GPDMA2_CH7_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void FDCAN3_IT0_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void FDCAN3_IT1_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void FMAC2_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void FDCAN2_IT0_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void FDCAN2_IT1_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void CORDIC1_IRQHandler                (void) __attribute__((weak, alias("Default_IRQHandler")));
void FMAC1_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void DTS_IRQHandler                    (void) __attribute__((weak, alias("Default_IRQHandler")));
void RNG_IRQHandler                    (void) __attribute__((weak, alias("Default_IRQHandler")));
void OTFDEC1_IRQHandler                (void) __attribute__((weak, alias("Default_IRQHandler")));
void AES_IRQHandler                    (void) __attribute__((weak, alias("Default_IRQHandler")));
void HASH_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void PKA_IRQHandler                    (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM10_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM12_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM13_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM14_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void I3C1_EV_IRQHandler                (void) __attribute__((weak, alias("Default_IRQHandler")));
void I3C1_ERR_IRQHandler               (void) __attribute__((weak, alias("Default_IRQHandler")));
void I2C4_EV_IRQHandler                (void) __attribute__((weak, alias("Default_IRQHandler")));
void I2C4_ERR_IRQHandler               (void) __attribute__((weak, alias("Default_IRQHandler")));
void POSENC1_IRQHandler                (void) __attribute__((weak, alias("Default_IRQHandler")));
void POSENC2_IRQHandler                (void) __attribute__((weak, alias("Default_IRQHandler")));
void HRTIM1_M_IRQHandler               (void) __attribute__((weak, alias("Default_IRQHandler")));
void HRTIM1_TIMA_IRQHandler            (void) __attribute__((weak, alias("Default_IRQHandler")));
void HRTIM1_TIMB_IRQHandler            (void) __attribute__((weak, alias("Default_IRQHandler")));
void HRTIM1_TIMC_IRQHandler            (void) __attribute__((weak, alias("Default_IRQHandler")));
void HRTIM1_TIMD_IRQHandler            (void) __attribute__((weak, alias("Default_IRQHandler")));
void HRTIM1_TIME_IRQHandler            (void) __attribute__((weak, alias("Default_IRQHandler")));
void HRTIM1_TIMF_IRQHandler            (void) __attribute__((weak, alias("Default_IRQHandler")));
void HRTIM1_FLT_IRQHandler             (void) __attribute__((weak, alias("Default_IRQHandler")));
void PLAY1_S_IRQHandler                (void) __attribute__((weak, alias("Default_IRQHandler")));
void PLAY1_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void COMP1_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void COMP2_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void COMP3_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void COMP4_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void COMP5_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void COMP6_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void COMP7_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void COMP8_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void DAC2_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void DAC5_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void DAC6_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void DAC7_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void DAC8_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void DAC9_IRQHandler                   (void) __attribute__((weak, alias("Default_IRQHandler")));
void DAC10_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM11_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM26_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));
void TIM27_IRQHandler                  (void) __attribute__((weak, alias("Default_IRQHandler")));

/* Private variables ---------------------------------------------------------*/
/**
  Vector table
  */

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif /* __GNUC__ */

extern const VECTOR_TABLE_Type __VECTOR_TABLE[];
const VECTOR_TABLE_Type __VECTOR_TABLE[] __VECTOR_TABLE_ATTRIBUTE =
{
  (VECTOR_TABLE_Type)(&__INITIAL_SP), /* The initial stack pointer */
  Reset_Handler,
  NMI_Handler,
  HardFault_Handler,
  MemManage_Handler,
  BusFault_Handler,
  UsageFault_Handler,
  SecureFault_Handler,
  0,
  0,
  0,
  SVC_Handler,
  DebugMon_Handler,
  0,
  PendSV_Handler,
  SysTick_Handler,
  WWDG_IRQHandler,
  0,
  IWDG_IRQHandler,
  0,
  PWR_PVD_AVD_IRQHandler,
  TAMP_IRQHandler,
  GTZC1_IRQHandler,
  FLASH_S_IRQHandler,
  FLASH_IRQHandler,
  0,
  0,
  RAMCFG_IRQHandler,
  RCC_S_IRQHandler,
  RCC_IRQHandler,
  ICACHE_IRQHandler,
  DCACHE_IRQHandler,
  FPU_IRQHandler,
  RTC_S_IRQHandler,
  RTC_IRQHandler,
  EXTI0_IRQHandler,
  EXTI1_IRQHandler,
  EXTI2_IRQHandler,
  EXTI3_IRQHandler,
  EXTI4_IRQHandler,
  EXTI5_IRQHandler,
  EXTI6_IRQHandler,
  EXTI7_IRQHandler,
  EXTI8_IRQHandler,
  EXTI9_IRQHandler,
  EXTI10_IRQHandler,
  EXTI11_IRQHandler,
  EXTI12_IRQHandler,
  EXTI13_IRQHandler,
  EXTI14_IRQHandler,
  EXTI15_IRQHandler,
  GPDMA1_CH0_IRQHandler,
  GPDMA1_CH1_IRQHandler,
  GPDMA1_CH2_IRQHandler,
  GPDMA1_CH3_IRQHandler,
  GPDMA1_CH4_IRQHandler,
  GPDMA1_CH5_IRQHandler,
  GPDMA1_CH6_IRQHandler,
  GPDMA1_CH7_IRQHandler,
  SAES_IRQHandler,
  ADC1_IRQHandler,
  DAC1_IRQHandler,
  FDCAN1_IT0_IRQHandler,
  FDCAN1_IT1_IRQHandler,
  TIM1_BRK_TERR_IERR_IRQHandler,
  TIM1_UPD_IRQHandler,
  TIM1_TRGI_COM_DIR_IDX_IRQHandler,
  TIM1_CC_IRQHandler,
  TIM2_IRQHandler,
  TIM3_IRQHandler,
  TIM4_IRQHandler,
  TIM5_IRQHandler,
  TIM6_IRQHandler,
  TIM7_IRQHandler,
  I2C1_EV_IRQHandler,
  I2C1_ERR_IRQHandler,
  I2C2_EV_IRQHandler,
  I2C2_ERR_IRQHandler,
  SPI1_IRQHandler,
  SPI2_IRQHandler,
  SPI3_IRQHandler,
  USART1_IRQHandler,
  USART2_IRQHandler,
  USART3_IRQHandler,
  UART4_IRQHandler,
  UART5_IRQHandler,
  LPUART1_IRQHandler,
  LPTIM1_IRQHandler,
  TIM8_BRK_TERR_IERR_IRQHandler,
  TIM8_UPD_IRQHandler,
  TIM8_TRGI_COM_DIR_IDX_IRQHandler,
  TIM8_CC_IRQHandler,
  ADC2_IRQHandler,
  ADC3_IRQHandler,
  TIM15_IRQHandler,
  TIM16_IRQHandler,
  TIM17_IRQHandler,
  USB_DRD_FS_IRQHandler,
  CRS_IRQHandler,
  UCPD1_IRQHandler,
  0,
  XSPI1_IRQHandler,
  TIM9_IRQHandler,
  I2C3_EV_IRQHandler,
  I2C3_ERR_IRQHandler,
  0,
  0,
  0,
  UART6_IRQHandler,
  TIM20_BRK_TERR_IERR_IRQHandler,
  TIM20_UPD_IRQHandler,
  TIM20_TRGI_COM_DIR_IDX_IRQHandler,
  TIM20_CC_IRQHandler,
  GPDMA2_CH0_IRQHandler,
  GPDMA2_CH1_IRQHandler,
  GPDMA2_CH2_IRQHandler,
  GPDMA2_CH3_IRQHandler,
  GPDMA2_CH4_IRQHandler,
  GPDMA2_CH5_IRQHandler,
  GPDMA2_CH6_IRQHandler,
  GPDMA2_CH7_IRQHandler,
  0,
  0,
  FDCAN3_IT0_IRQHandler,
  FDCAN3_IT1_IRQHandler,
  0,
  0,
  FMAC2_IRQHandler,
  FDCAN2_IT0_IRQHandler,
  FDCAN2_IT1_IRQHandler,
  CORDIC1_IRQHandler,
  FMAC1_IRQHandler,
  DTS_IRQHandler,
  RNG_IRQHandler,
  OTFDEC1_IRQHandler,
  AES_IRQHandler,
  HASH_IRQHandler,
  PKA_IRQHandler,
  TIM10_IRQHandler,
  TIM12_IRQHandler,
  TIM13_IRQHandler,
  TIM14_IRQHandler,
  I3C1_EV_IRQHandler,
  I3C1_ERR_IRQHandler,
  I2C4_EV_IRQHandler,
  I2C4_ERR_IRQHandler,
  0,
  0,
  0,
  0,
  0,
  0,
  POSENC1_IRQHandler,
  POSENC2_IRQHandler,
  HRTIM1_M_IRQHandler,
  HRTIM1_TIMA_IRQHandler,
  HRTIM1_TIMB_IRQHandler,
  HRTIM1_TIMC_IRQHandler,
  HRTIM1_TIMD_IRQHandler,
  HRTIM1_TIME_IRQHandler,
  HRTIM1_TIMF_IRQHandler,
  HRTIM1_FLT_IRQHandler,
  PLAY1_S_IRQHandler,
  PLAY1_IRQHandler,
  COMP1_IRQHandler,
  COMP2_IRQHandler,
  COMP3_IRQHandler,
  COMP4_IRQHandler,
  COMP5_IRQHandler,
  COMP6_IRQHandler,
  COMP7_IRQHandler,
  COMP8_IRQHandler,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  DAC2_IRQHandler,
  0,
  0,
  DAC5_IRQHandler,
  DAC6_IRQHandler,
  DAC7_IRQHandler,
  DAC8_IRQHandler,
  DAC9_IRQHandler,
  DAC10_IRQHandler,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  TIM11_IRQHandler,
  TIM26_IRQHandler,
  TIM27_IRQHandler
};

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif /* __GNUC__ */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function is the Reset Handler called on controller reset.
  * @param  None
  * @retval None
  */
__WEAK __NO_RETURN void Reset_Handler(void)
{
  __set_PSP((uint32_t)(&__INITIAL_SP));

  __set_MSPLIM((uint32_t)(&__STACK_LIMIT));
  __set_PSPLIM((uint32_t)(&__STACK_LIMIT));

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  __TZ_set_STACKSEAL_S((uint32_t *)(&__STACK_SEAL));
#endif /* __ARM_FEATURE_CMSE */

  SystemInit();         /* CMSIS System Initialization */
  __PROGRAM_START();    /* Enter PreMain (C library entry point) */
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif /* __ARMCC_VERSION */

/**
  * @brief  Hard Fault Handler.
  * @param  None
  * @retval None
  */
__WEAK void HardFault_Handler(void)
{
  while (1);
}

/**
  * @brief  This function is the default IRQ handler
  *         when the IRQ line is not used by the application.
  * @param  None
  * @retval None
  */
void Default_IRQHandler(void)
{
  Default_IRQHandler_Hook();
}

/**
  * @brief  This function is the default IRQHandler implementation hook
  *         could be overridden by the application.
  * @param  None
  * @retval None
  */
__WEAK void Default_IRQHandler_Hook(void)
{
  while (1);
}

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic pop
#endif /* __ARMCC_VERSION */
