//Add commentMore actions
// Created by ABKAR on 6/7/2025.
//

#ifndef PWM_PRIVATE_H
#define PWM_PRIVATE_H


// TIM3 Register Addresses
#define TIM3_CR1      (*(volatile uint32 *)0x40000400)
#define TIM3_EGR      (*(volatile uint32 *)0x40000414)
#define TIM3_CCMR1    (*(volatile uint32 *)0x40000418)
#define TIM3_CCER     (*(volatile uint32 *)0x40000420)
#define TIM3_PSC      (*(volatile uint32 *)0x40000428)
#define TIM3_ARR      (*(volatile uint32 *)0x4000042C)
#define TIM3_CCR1     (*(volatile uint32 *)0x40000434)

// TIM3 Bit Definitions
#define TIM_CR1_CEN         (1UL << 0)  // Counter Enable
#define TIM_CR1_ARPE        (1UL << 7)  // Auto-reload Preload Enable
#define TIM_CCMR1_OC1M_PWM1 (6UL << 4)  // PWM Mode 1
#define TIM_CCMR1_OC1PE     (1UL << 3)  // Output Compare Preload Enable
#define TIM_CCER_CC1E       (1UL << 0)  // Capture/Compare 1 Enable
#define TIM_EGR_UG          (1UL << 0)  // Update Generation



#endif // PWM_PRIVATE_H