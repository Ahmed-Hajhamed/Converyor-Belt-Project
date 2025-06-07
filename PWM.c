//
// Created by ABKAR on 6/7/2025.
//
#include "PWM.h"

#include <stdio.h>

#include "PWM_Private.h"
#include "Gpio.h"
#include "Rcc.h"

void PWM_Init(void) {
    // 1. Enable clocks using RCC driver
    Rcc_Enable(RCC_GPIOA);  // Enable GPIOA clock
    Rcc_Enable(RCC_TIM3);   // Enable TIM3 clock

    // 2. Configure PA6 as Alternate Function (AF1) for TIM3_CH1 on STM32F401VE
    Gpio_Init(GPIO_A, 6, GPIO_AF, GPIO_PUSH_PULL);  // Set PA6 to alternate function
    volatile uint32* afrl = (volatile uint32 *)0x40020020; // GPIOA_AFRL
    *afrl &= ~(0xFUL << (6 * 4));  // Clear AF bits for PA6
    *afrl |= (1UL << (6 * 4));     // Set AF1 for TIM3_CH1 (STM32F401VE)

    // 3. Configure TIM3 for 1 kHz PWM
    // APB1 clock = 42 MHz, Prescaler = 41 → Timer clock = 1 MHz
    // ARR = 999 → PWM freq = 1 MHz / 1000 = 1 kHz
    TIM3_PSC  = 41;           // Prescaler for 1 MHz timer clock
    TIM3_ARR  = 999;          // Auto-reload for 1 kHz period
    TIM3_CR1  = 0;            // Clear CR1
    TIM3_CCR1 = 0;            // Start with 0% duty cycle

    // 4. Configure TIM3 Channel 1 for PWM Mode 1 with Preload
    TIM3_CCMR1 &= ~(7UL << 4);         // Clear OC1M bits (mode selection)
    TIM3_CCMR1 |= TIM_CCMR1_OC1M_PWM1; // Set PWM Mode 1
    TIM3_CCMR1 |= TIM_CCMR1_OC1PE;     // Enable Output Compare Preload

    // 5. Enable Channel 1 output
    TIM3_CCER |= TIM_CCER_CC1E;        // Enable Channel 1 output

    // 6. Apply settings and start the timer
    TIM3_CR1 |= TIM_CR1_ARPE;          // Enable auto-reload preload
    TIM3_EGR |= TIM_EGR_UG;            // Generate update event to apply settings
    TIM3_CR1 |= TIM_CR1_CEN;           // Start the timer
}

void PWM_SetDutyCycle(uint16 duty_percent) {
    if (duty_percent > 100) duty_percent = 100; // Cap at 100%
    // Map percentage to CCR1 value (0 to ARR)
    TIM3_CCR1 = (duty_percent * (TIM3_ARR + 1)) / 100;
}

void PWM_Stop(void) {
    TIM3_CCR1 = 0;            // Set duty cycle to 0%
    TIM3_CR1 &= ~TIM_CR1_CEN; // Disable the timer
}