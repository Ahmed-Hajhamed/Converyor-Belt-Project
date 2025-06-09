// Created by ABKAR on 6/7/2025.

#include "PWM.h"

#include <stdio.h>

#include "PWM_Private.h"
#include "Gpio.h"
#include "Rcc.h"

void PWM_Init(void) {
    // 1. Enable peripheral clocks using RCC driver
    Rcc_Enable(RCC_GPIOA);  // Enable GPIOA clock for PA6 pin
    Rcc_Enable(RCC_TIM3);   // Enable TIM3 peripheral clock

    // 2. Configure PA6 as Alternate Function (AF1) for TIM3_CH1
    Gpio_Init(GPIO_A, 6, GPIO_AF, GPIO_PUSH_PULL);  // Set PA6 to alternate function mode
    volatile uint32_t* afrl = (volatile uint32_t *)0x40020020; // GPIOA_AFRL register address
    *afrl &= ~(0xFUL << (6 * 4));  // Clear the 4 bits corresponding to pin 6
    *afrl |= (1UL << (6 * 4));     // Set AF1 (TIM3_CH1) for pin 6

    // 3. Configure TIM3 for 1 kHz PWM signal
    // Prescaler = 41 → Timer clock = 42MHz / (41+1) = 1MHz
    TIM3->PSC  = 41;     // Set prescaler
    TIM3->ARR  = 999;    // Auto-reload value to get 1 kHz frequency
    TIM3->CR1  = 0;      // Clear CR1 to reset configuration
    TIM3->CCR1 = 0;      // Start with 0% duty cycle

    // 4. Configure TIM3 Channel 1 for PWM Mode 1
    TIM3->CCMR1 &= ~(7UL << 4);         // Clear output compare mode bits for channel 1
    TIM3->CCMR1 |= TIM_CCMR1_OC1M_PWM1; // Set PWM mode 1 (active until match)
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;     // Enable preload for CCR1

    // 5. Enable TIM3 Channel 1 output on the pin
    TIM3->CCER |= TIM_CCER_CC1E;        // Enable output for channel 1

    // 6. Start the timer with configuration
    TIM3->CR1 |= TIM_CR1_ARPE;          // Enable auto-reload preload
    TIM3->EGR |= TIM_EGR_UG;            // Generate update event to load registers
    TIM3->CR1 |= TIM_CR1_CEN;           // Enable the timer counter
}

void PWM_SetDutyCycle(uint16_t duty_percent) {
    // Cap duty cycle at 100% if needed
    if (duty_percent > 100) duty_percent = 100;

    // Compute CCR1 value from percentage and ARR
    TIM3->CCR1 = (duty_percent * (TIM3->ARR + 1)) / 100;
}

void PWM_Stop(void) {
    // Set output compare to 0% (low signal)
    TIM3->CCR1 = 0;

    // Disable the timer
    TIM3->CR1 &= ~TIM_CR1_CEN;
}
