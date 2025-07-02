/********************************** (C) COPYRIGHT  *******************************
 * File Name          : core_riscv.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : RISC-V Core Peripheral Access Layer Header File for CH32V30x
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/
#ifndef __CORE_RISCV_H__
#define __CORE_RISCV_H__

#include <stdint.h>

// #include "../Peripheral/inc/ch32v30x.h"
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#if !defined(CH32V30x_D8) && !defined(CH32V30x_D8C)
// #define CH32V30x_D8              /* CH32V303x */
#define CH32V30x_D8C /* CH32V307x-CH32V305x */

#endif

/* IO definitions */
#ifdef __cplusplus
#define __I volatile /* defines 'read only' permissions */
#else
#define __I volatile const /* defines 'read only' permissions */
#endif
#define __O volatile  /* defines 'write only' permissions */
#define __IO volatile /* defines 'read / write' permissions */

/* Interrupt Number Definition, according to the selected device */
typedef enum IRQn {
    /******  RISC-V Processor Exceptions Numbers *******************************************************/
    NonMaskableInt_IRQn = 2, /* 2 Non Maskable Interrupt                             */
    EXC_IRQn = 3,            /* 3 Exception Interrupt                                */
    Ecall_M_Mode_IRQn = 5,   /* 5 Ecall M Mode Interrupt                             */
    Ecall_U_Mode_IRQn = 8,   /* 8 Ecall U Mode Interrupt                             */
    Break_Point_IRQn = 9,    /* 9 Break Point Interrupt                              */
    SysTick_IRQn = 12,       /* 12 System timer Interrupt                            */
    Software_IRQn = 14,      /* 14 software Interrupt                                */

    /******  RISC-V specific Interrupt Numbers *********************************************************/
    WWDG_IRQn = 16,            /* Window WatchDog Interrupt                            */
    PVD_IRQn = 17,             /* PVD through EXTI Line detection Interrupt            */
    TAMPER_IRQn = 18,          /* Tamper Interrupt                                     */
    RTC_IRQn = 19,             /* RTC global Interrupt                                 */
    FLASH_IRQn = 20,           /* FLASH global Interrupt                               */
    RCC_IRQn = 21,             /* RCC global Interrupt                                 */
    EXTI0_IRQn = 22,           /* EXTI Line0 Interrupt                                 */
    EXTI1_IRQn = 23,           /* EXTI Line1 Interrupt                                 */
    EXTI2_IRQn = 24,           /* EXTI Line2 Interrupt                                 */
    EXTI3_IRQn = 25,           /* EXTI Line3 Interrupt                                 */
    EXTI4_IRQn = 26,           /* EXTI Line4 Interrupt                                 */
    DMA1_Channel1_IRQn = 27,   /* DMA1 Channel 1 global Interrupt                      */
    DMA1_Channel2_IRQn = 28,   /* DMA1 Channel 2 global Interrupt                      */
    DMA1_Channel3_IRQn = 29,   /* DMA1 Channel 3 global Interrupt                      */
    DMA1_Channel4_IRQn = 30,   /* DMA1 Channel 4 global Interrupt                      */
    DMA1_Channel5_IRQn = 31,   /* DMA1 Channel 5 global Interrupt                      */
    DMA1_Channel6_IRQn = 32,   /* DMA1 Channel 6 global Interrupt                      */
    DMA1_Channel7_IRQn = 33,   /* DMA1 Channel 7 global Interrupt                      */
    ADC_IRQn = 34,             /* ADC1 and ADC2 global Interrupt                       */
    USB_HP_CAN1_TX_IRQn = 35,  /* USB Device High Priority or CAN1 TX Interrupts       */
    USB_LP_CAN1_RX0_IRQn = 36, /* USB Device Low Priority or CAN1 RX0 Interrupts       */
    CAN1_RX1_IRQn = 37,        /* CAN1 RX1 Interrupt                                   */
    CAN1_SCE_IRQn = 38,        /* CAN1 SCE Interrupt                                   */
    EXTI9_5_IRQn = 39,         /* External Line[9:5] Interrupts                        */
    TIM1_BRK_IRQn = 40,        /* TIM1 Break Interrupt                                 */
    TIM1_UP_IRQn = 41,         /* TIM1 Update Interrupt                                */
    TIM1_TRG_COM_IRQn = 42,    /* TIM1 Trigger and Commutation Interrupt               */
    TIM1_CC_IRQn = 43,         /* TIM1 Capture Compare Interrupt                       */
    TIM2_IRQn = 44,            /* TIM2 global Interrupt                                */
    TIM3_IRQn = 45,            /* TIM3 global Interrupt                                */
    TIM4_IRQn = 46,            /* TIM4 global Interrupt                                */
    I2C1_EV_IRQn = 47,         /* I2C1 Event Interrupt                                 */
    I2C1_ER_IRQn = 48,         /* I2C1 Error Interrupt                                 */
    I2C2_EV_IRQn = 49,         /* I2C2 Event Interrupt                                 */
    I2C2_ER_IRQn = 50,         /* I2C2 Error Interrupt                                 */
    SPI1_IRQn = 51,            /* SPI1 global Interrupt                                */
    SPI2_IRQn = 52,            /* SPI2 global Interrupt                                */
    USART1_IRQn = 53,          /* USART1 global Interrupt                              */
    USART2_IRQn = 54,          /* USART2 global Interrupt                              */
    USART3_IRQn = 55,          /* USART3 global Interrupt                              */
    EXTI15_10_IRQn = 56,       /* External Line[15:10] Interrupts                      */
    RTCAlarm_IRQn = 57,        /* RTC Alarm through EXTI Line Interrupt                */

#ifdef CH32V30x_D8
    TIM8_BRK_IRQn = 59,        /* TIM8 Break Interrupt                                 */
    TIM8_UP_IRQn = 60,         /* TIM8 Update Interrupt                                */
    TIM8_TRG_COM_IRQn = 61,    /* TIM8 Trigger and Commutation Interrupt               */
    TIM8_CC_IRQn = 62,         /* TIM8 Capture Compare Interrupt                       */
    RNG_IRQn = 63,             /* RNG global Interrupt                                 */
    FSMC_IRQn = 64,            /* FSMC global Interrupt                                */
    SDIO_IRQn = 65,            /* SDIO global Interrupt                                */
    TIM5_IRQn = 66,            /* TIM5 global Interrupt                                */
    SPI3_IRQn = 67,            /* SPI3 global Interrupt                                */
    UART4_IRQn = 68,           /* UART4 global Interrupt                               */
    UART5_IRQn = 69,           /* UART5 global Interrupt                               */
    TIM6_IRQn = 70,            /* TIM6 global Interrupt                                */
    TIM7_IRQn = 71,            /* TIM7 global Interrupt                                */
    DMA2_Channel1_IRQn = 72,   /* DMA2 Channel 1 global Interrupt                      */
    DMA2_Channel2_IRQn = 73,   /* DMA2 Channel 2 global Interrupt                      */
    DMA2_Channel3_IRQn = 74,   /* DMA2 Channel 3 global Interrupt                      */
    DMA2_Channel4_IRQn = 75,   /* DMA2 Channel 4 global Interrupt                      */
    DMA2_Channel5_IRQn = 76,   /* DMA2 Channel 5 global Interrupt                      */
    OTG_FS_IRQn = 83,          /* OTGFS global Interrupt                               */
    UART6_IRQn = 87,           /* UART6 global Interrupt                               */
    UART7_IRQn = 88,           /* UART7 global Interrupt                               */
    UART8_IRQn = 89,           /* UART8 global Interrupt                               */
    TIM9_BRK_IRQn = 90,        /* TIM9 Break Interrupt                                 */
    TIM9_UP_IRQn = 91,         /* TIM9 Update Interrupt                                */
    TIM9_TRG_COM_IRQn = 92,    /* TIM9 Trigger and Commutation Interrupt               */
    TIM9_CC_IRQn = 93,         /* TIM9 Capture Compare Interrupt                       */
    TIM10_BRK_IRQn = 94,       /* TIM10 Break Interrupt                                */
    TIM10_UP_IRQn = 95,        /* TIM10 Update Interrupt                               */
    TIM10_TRG_COM_IRQn = 96,   /* TIM10 Trigger and Commutation Interrupt              */
    TIM10_CC_IRQn = 97,        /* TIM10 Capture Compare Interrupt                      */
    DMA2_Channel6_IRQn = 98,   /* DMA2 Channel 6 global Interrupt                      */
    DMA2_Channel7_IRQn = 99,   /* DMA2 Channel 7 global Interrupt                      */
    DMA2_Channel8_IRQn = 100,  /* DMA2 Channel 8 global Interrupt                      */
    DMA2_Channel9_IRQn = 101,  /* DMA2 Channel 9 global Interrupt                      */
    DMA2_Channel10_IRQn = 102, /* DMA2 Channel 10 global Interrupt                     */
    DMA2_Channel11_IRQn = 103, /* DMA2 Channel 11 global Interrupt                     */

#elif defined(CH32V30x_D8C)
    USBWakeUp_IRQn = 58,       /* USB Device WakeUp from suspend through EXTI Line Interrupt */
    TIM8_BRK_IRQn = 59,        /* TIM8 Break Interrupt                                 */
    TIM8_UP_IRQn = 60,         /* TIM8 Update Interrupt                                */
    TIM8_TRG_COM_IRQn = 61,    /* TIM8 Trigger and Commutation Interrupt               */
    TIM8_CC_IRQn = 62,         /* TIM8 Capture Compare Interrupt                       */
    RNG_IRQn = 63,             /* RNG global Interrupt                                 */
    FSMC_IRQn = 64,            /* FSMC global Interrupt                                */
    SDIO_IRQn = 65,            /* SDIO global Interrupt                                */
    TIM5_IRQn = 66,            /* TIM5 global Interrupt                                */
    SPI3_IRQn = 67,            /* SPI3 global Interrupt                                */
    UART4_IRQn = 68,           /* UART4 global Interrupt                               */
    UART5_IRQn = 69,           /* UART5 global Interrupt                               */
    TIM6_IRQn = 70,            /* TIM6 global Interrupt                                */
    TIM7_IRQn = 71,            /* TIM7 global Interrupt                                */
    DMA2_Channel1_IRQn = 72,   /* DMA2 Channel 1 global Interrupt                      */
    DMA2_Channel2_IRQn = 73,   /* DMA2 Channel 2 global Interrupt                      */
    DMA2_Channel3_IRQn = 74,   /* DMA2 Channel 3 global Interrupt                      */
    DMA2_Channel4_IRQn = 75,   /* DMA2 Channel 4 global Interrupt                      */
    DMA2_Channel5_IRQn = 76,   /* DMA2 Channel 5 global Interrupt                      */
    ETH_IRQn = 77,             /* ETH global Interrupt                                 */
    ETH_WKUP_IRQn = 78,        /* ETH WakeUp Interrupt                                 */
    CAN2_TX_IRQn = 79,         /* CAN2 TX Interrupts                                   */
    CAN2_RX0_IRQn = 80,        /* CAN2 RX0 Interrupts                                  */
    CAN2_RX1_IRQn = 81,        /* CAN2 RX1 Interrupt                                   */
    CAN2_SCE_IRQn = 82,        /* CAN2 SCE Interrupt                                   */
    OTG_FS_IRQn = 83,          /* OTGFS global Interrupt                               */
    USBHSWakeup_IRQn = 84,     /* USBHS WakeUp Interrupt                               */
    USBHS_IRQn = 85,           /* USBHS global Interrupt                               */
    DVP_IRQn = 86,             /* DVP global Interrupt                                 */
    UART6_IRQn = 87,           /* UART6 global Interrupt                               */
    UART7_IRQn = 88,           /* UART7 global Interrupt                               */
    UART8_IRQn = 89,           /* UART8 global Interrupt                               */
    TIM9_BRK_IRQn = 90,        /* TIM9 Break Interrupt                                 */
    TIM9_UP_IRQn = 91,         /* TIM9 Update Interrupt                                */
    TIM9_TRG_COM_IRQn = 92,    /* TIM9 Trigger and Commutation Interrupt               */
    TIM9_CC_IRQn = 93,         /* TIM9 Capture Compare Interrupt                       */
    TIM10_BRK_IRQn = 94,       /* TIM10 Break Interrupt                                */
    TIM10_UP_IRQn = 95,        /* TIM10 Update Interrupt                               */
    TIM10_TRG_COM_IRQn = 96,   /* TIM10 Trigger and Commutation Interrupt              */
    TIM10_CC_IRQn = 97,        /* TIM10 Capture Compare Interrupt                      */
    DMA2_Channel6_IRQn = 98,   /* DMA2 Channel 6 global Interrupt                      */
    DMA2_Channel7_IRQn = 99,   /* DMA2 Channel 7 global Interrupt                      */
    DMA2_Channel8_IRQn = 100,  /* DMA2 Channel 8 global Interrupt                      */
    DMA2_Channel9_IRQn = 101,  /* DMA2 Channel 9 global Interrupt                      */
    DMA2_Channel10_IRQn = 102, /* DMA2 Channel 10 global Interrupt                     */
    DMA2_Channel11_IRQn = 103, /* DMA2 Channel 11 global Interrupt                     */

#endif
} IRQn_Type;

/* Standard Peripheral Library old types (maintained for legacy purpose) */
typedef __I uint64_t vuc64; /* Read Only */
typedef __I uint32_t vuc32; /* Read Only */
typedef __I uint16_t vuc16; /* Read Only */
typedef __I uint8_t vuc8;   /* Read Only */

typedef const uint64_t uc64; /* Read Only */
typedef const uint32_t uc32; /* Read Only */
typedef const uint16_t uc16; /* Read Only */
typedef const uint8_t uc8;   /* Read Only */

typedef __I int64_t vsc64; /* Read Only */
typedef __I int32_t vsc32; /* Read Only */
typedef __I int16_t vsc16; /* Read Only */
typedef __I int8_t vsc8;   /* Read Only */

typedef const int64_t sc64; /* Read Only */
typedef const int32_t sc32; /* Read Only */
typedef const int16_t sc16; /* Read Only */
typedef const int8_t sc8;   /* Read Only */

typedef __IO uint64_t vu64;
typedef __IO uint32_t vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t vu8;

typedef __IO int64_t vs64;
typedef __IO int32_t vs32;
typedef __IO int16_t vs16;
typedef __IO int8_t vs8;

typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

#define DISABLE (false)
#define ENABLE (true)

typedef bool FunctionalState;

typedef enum { NoREADY = 0, READY = !NoREADY } ErrorStatus;
// typedef enum {DISABLE = 0, ENABLE = 1} FunctionalState;
#define RESET (false)
#define SET (true)
typedef bool FlagStatus;
typedef bool ITStatus;

#define RV_STATIC_INLINE static inline

/* memory mapped structure for Program Fast Interrupt Controller (PFIC) */
typedef struct {
    __I uint32_t ISR[8];
    __I uint32_t IPR[8];
    __IO uint32_t ITHRESDR;
    __IO uint32_t RESERVED;
    __IO uint32_t CFGR;
    __I uint32_t GISR;
    __IO uint8_t VTFIDR[4];
    uint8_t RESERVED0[12];
    __IO uint32_t VTFADDR[4];
    uint8_t RESERVED1[0x90];
    __O uint32_t IENR[8];
    uint8_t RESERVED2[0x60];
    __O uint32_t IRER[8];
    uint8_t RESERVED3[0x60];
    __O uint32_t IPSR[8];
    uint8_t RESERVED4[0x60];
    __O uint32_t IPRR[8];
    uint8_t RESERVED5[0x60];
    __IO uint32_t IACTR[8];
    uint8_t RESERVED6[0xE0];
    __IO uint8_t IPRIOR[256];
    uint8_t RESERVED7[0x810];
    __IO uint32_t SCTLR;
} PFIC_Type;

/* memory mapped structure for SysTick */
typedef struct {
    vu32 CTLR;
    vu32 SR;
    vu64 CNT;
    vu64 CMP;
} SysTick_Type;

#define PFIC ((PFIC_Type *)0xE000E000)
#define NVIC PFIC
#define NVIC_KEY1 ((uint32_t)0xFA050000)
#define NVIC_KEY2 ((uint32_t)0xBCAF0000)
#define NVIC_KEY3 ((uint32_t)0xBEEF0000)

#define SysTick ((SysTick_Type *)0xE000F000)

/*********************************************************************
 * @fn      __enable_irq
 *
 * @brief   Enable Global Interrupt
 *
 * @return  none
 */
RV_STATIC_INLINE void __enable_irq() { __asm volatile("csrw 0x800, %0" : : "r"(0x6088)); }

/*********************************************************************
 * @fn      __disable_irq
 *
 * @brief   Disable Global Interrupt
 *
 * @return  none
 */
RV_STATIC_INLINE void __disable_irq() { __asm volatile("csrw 0x800, %0" : : "r"(0x6000)); }

/*********************************************************************
 * @fn      __NOP
 *
 * @brief   nop
 *
 * @return  none
 */
RV_STATIC_INLINE void __NOP() { __asm volatile("nop"); }

/*********************************************************************
 * @fn      NVIC_EnableIRQ
 *
 * @brief   Enable Interrupt
 *
 * @param   IRQn: Interrupt Numbers
 *
 * @return  none
 */
RV_STATIC_INLINE void NVIC_EnableIRQ(IRQn_Type IRQn) { NVIC->IENR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)); }

/*********************************************************************
 * @fn      NVIC_DisableIRQ
 *
 * @brief   Disable Interrupt
 *
 * @param   IRQn: Interrupt Numbers
 *
 * @return  none
 */
RV_STATIC_INLINE void NVIC_DisableIRQ(IRQn_Type IRQn) { NVIC->IRER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)); }

/*********************************************************************
 * @fn      NVIC_GetStatusIRQ
 *
 * @brief   Get Interrupt Enable State
 *
 * @param   IRQn: Interrupt Numbers
 *
 * @return  1 - Interrupt Enable
 *          0 - Interrupt Disable
 */
RV_STATIC_INLINE uint32_t NVIC_GetStatusIRQ(IRQn_Type IRQn) {
    return ((uint32_t)((NVIC->ISR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F))) ? 1 : 0));
}

/*********************************************************************
 * @fn      NVIC_GetPendingIRQ
 *
 * @brief   Get Interrupt Pending State
 *
 * @param   IRQn: Interrupt Numbers
 *
 * @return  1 - Interrupt Pending Enable
 *          0 - Interrupt Pending Disable
 */
RV_STATIC_INLINE uint32_t NVIC_GetPendingIRQ(IRQn_Type IRQn) {
    return ((uint32_t)((NVIC->IPR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F))) ? 1 : 0));
}

/*********************************************************************
 * @fn      NVIC_SetPendingIRQ
 *
 * @brief   Set Interrupt Pending
 *
 * @param   IRQn: Interrupt Numbers
 *
 * @return  None
 */
RV_STATIC_INLINE void NVIC_SetPendingIRQ(IRQn_Type IRQn) { NVIC->IPSR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)); }

/*********************************************************************
 * @fn      NVIC_ClearPendingIRQ
 *
 * @brief   Clear Interrupt Pending
 *
 * @param   IRQn: Interrupt Numbers
 *
 * @return  None
 */
RV_STATIC_INLINE void NVIC_ClearPendingIRQ(IRQn_Type IRQn) { NVIC->IPRR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)); }

/*********************************************************************
 * @fn      NVIC_GetActive
 *
 * @brief   Get Interrupt Active State
 *
 * @param   IRQn: Interrupt Numbers
 *
 * @return  1 - Interrupt Active
 *          0 - Interrupt No Active
 */
RV_STATIC_INLINE uint32_t NVIC_GetActive(IRQn_Type IRQn) {
    return ((uint32_t)((NVIC->IACTR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F))) ? 1 : 0));
}

/*********************************************************************
 * @fn      NVIC_SetPriority
 *
 * @brief   Set Interrupt Priority
 *
 * @param   IRQn - Interrupt Numbers
 *          priority -
 *              bit7 - pre-emption priority
 *              bit6~bit4 - subpriority
 * @return  None
 */
RV_STATIC_INLINE void NVIC_SetPriority(IRQn_Type IRQn, uint8_t priority) { NVIC->IPRIOR[(uint32_t)(IRQn)] = priority; }

/*********************************************************************
 * @fn      __WFI
 *
 * @brief   Wait for Interrupt
 *
 * @return  None
 */
__attribute__((always_inline)) RV_STATIC_INLINE void __WFI(void) {
    NVIC->SCTLR &= ~(1u << 3);  // wfi
    __asm volatile("wfi");
}

/*********************************************************************
 * @fn      __WFE
 *
 * @brief   Wait for Events
 *
 * @return  None
 */
__attribute__((always_inline)) RV_STATIC_INLINE void __WFE(void) {
    uint32_t t;

    t = NVIC->SCTLR;
    NVIC->SCTLR |= (1 << 3) | (1 << 5);  // (wfi->wfe)+(__sev)
    NVIC->SCTLR = (NVIC->SCTLR & ~(1u << 5)) | (t & (1u << 5));
    __asm volatile("wfi");
    __asm volatile("wfi");
}

/*********************************************************************
 * @fn      SetVTFIRQ
 *
 * @brief   Set VTF Interrupt
 *
 * @param   add - VTF interrupt service function base address.
 *          IRQn -Interrupt Numbers
 *          num - VTF Interrupt Numbers
 *          NewState - DISABLE or ENABLE
 * @return  None
 */
RV_STATIC_INLINE void SetVTFIRQ(uint32_t addr, IRQn_Type IRQn, uint8_t num, FunctionalState NewState) {
    if (num > 3) return;

    if (NewState != DISABLE) {
        NVIC->VTFIDR[num] = IRQn;
        NVIC->VTFADDR[num] = ((addr & 0xFFFFFFFE) | 0x1);
    } else {
        NVIC->VTFIDR[num] = IRQn;
        NVIC->VTFADDR[num] = ((addr & 0xFFFFFFFE) & (~0x1u));
    }
}

/*********************************************************************
 * @fn      NVIC_SystemReset
 *
 * @brief   Initiate a system reset request
 *
 * @return  None
 */
RV_STATIC_INLINE void NVIC_SystemReset(void) { NVIC->CFGR = NVIC_KEY3 | (1 << 7); }

/* Core_Exported_Functions */
extern uint32_t __get_FFLAGS(void);
extern void __set_FFLAGS(uint32_t value);
extern uint32_t __get_FRM(void);
extern void __set_FRM(uint32_t value);
extern uint32_t __get_FCSR(void);
extern void __set_FCSR(uint32_t value);
extern uint32_t __get_MSTATUS(void);
extern void __set_MSTATUS(uint32_t value);
extern uint32_t __get_MISA(void);
extern void __set_MISA(uint32_t value);
extern uint32_t __get_MIE(void);
extern void __set_MIE(uint32_t value);
extern uint32_t __get_MTVEC(void);
extern void __set_MTVEC(uint32_t value);
extern uint32_t __get_MSCRATCH(void);
extern void __set_MSCRATCH(uint32_t value);
extern uint32_t __get_MEPC(void);
extern void __set_MEPC(uint32_t value);
extern uint32_t __get_MCAUSE(void);
extern void __set_MCAUSE(uint32_t value);
extern uint32_t __get_MTVAL(void);
extern void __set_MTVAL(uint32_t value);
extern uint32_t __get_MVENDORID(void);
extern uint32_t __get_MARCHID(void);
extern uint32_t __get_MIMPID(void);
extern uint32_t __get_MHARTID(void);
extern uint32_t __get_SP(void);

#ifdef __cplusplus
}
#endif

#endif
