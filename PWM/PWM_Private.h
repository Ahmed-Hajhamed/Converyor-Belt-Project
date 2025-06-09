// Created by ABKAR on 6/7/2025.

#ifndef PWM_PRIVATE_H
#define PWM_PRIVATE_H

#include <stdint.h>

typedef struct {
    volatile uint32 CR1;       // 0x00
    volatile uint32 CR2;       // 0x04
    volatile uint32 SMCR;      // 0x08
    volatile uint32 DIER;      // 0x0C
    volatile uint32 SR;        // 0x10
    volatile uint32 EGR;       // 0x14
    volatile uint32 CCMR1;     // 0x18
    volatile uint32 CCMR2;     // 0x1C
    volatile uint32 CCER;      // 0x20
    volatile uint32 CNT;       // 0x24
    volatile uint32 PSC;       // 0x28
    volatile uint32 ARR;       // 0x2C
    volatile uint32 RCR;       // 0x30
    volatile uint32 CCR1;      // 0x34
    volatile uint32 CCR2;      // 0x38
    volatile uint32 CCR3;      // 0x3C
    volatile uint32 CCR4;      // 0x40
    volatile uint32 BDTR;      // 0x44
    volatile uint32 DCR;       // 0x48
    volatile uint32 DMAR;      // 0x4C
} TIM_Type;

#define TIM1_REG ((TIM_Type *) 0x40010000UL)  // TIM3 base address

// TIM3 Bit Definitions
#define TIM_CR1_CEN         (1UL << 0)  // Counter Enable
#define TIM_CR1_ARPE        (1UL << 7)  // Auto-reload Preload Enable
#define TIM_CCMR1_OC1M_PWM1 (6UL << 4)  // PWM Mode 1
#define TIM_CCMR1_OC1PE     (1UL << 3)  // Output Compare Preload Enable
#define TIM_CCER_CC1E       (1UL << 0)  // Capture/Compare 1 Enable
#define TIM_EGR_UG          (1UL << 0)  // Update Generation

#endif // PWM_PRIVATE_H
