// Created by ABKAR on 6/7/2025.

#ifndef PWM_PRIVATE_H
#define PWM_PRIVATE_H

#include <stdint.h>

typedef struct {
    volatile uint32_t CR1;       // 0x00
    volatile uint32_t CR2;       // 0x04
    volatile uint32_t SMCR;      // 0x08
    volatile uint32_t DIER;      // 0x0C
    volatile uint32_t SR;        // 0x10
    volatile uint32_t EGR;       // 0x14
    volatile uint32_t CCMR1;     // 0x18
    volatile uint32_t CCMR2;     // 0x1C
    volatile uint32_t CCER;      // 0x20
    volatile uint32_t CNT;       // 0x24
    volatile uint32_t PSC;       // 0x28
    volatile uint32_t ARR;       // 0x2C
    volatile uint32_t RCR;       // 0x30
    volatile uint32_t CCR1;      // 0x34
    volatile uint32_t CCR2;      // 0x38
    volatile uint32_t CCR3;      // 0x3C
    volatile uint32_t CCR4;      // 0x40
    volatile uint32_t BDTR;      // 0x44
    volatile uint32_t DCR;       // 0x48
    volatile uint32_t DMAR;      // 0x4C
} TIM_TypeDef;

#define TIM3 ((TIM_TypeDef *)0x40000400UL)  // TIM3 base address

// TIM3 Bit Definitions
#define TIM_CR1_CEN         (1UL << 0)  // Counter Enable
#define TIM_CR1_ARPE        (1UL << 7)  // Auto-reload Preload Enable
#define TIM_CCMR1_OC1M_PWM1 (6UL << 4)  // PWM Mode 1
#define TIM_CCMR1_OC1PE     (1UL << 3)  // Output Compare Preload Enable
#define TIM_CCER_CC1E       (1UL << 0)  // Capture/Compare 1 Enable
#define TIM_EGR_UG          (1UL << 0)  // Update Generation

#endif // PWM_PRIVATE_H
