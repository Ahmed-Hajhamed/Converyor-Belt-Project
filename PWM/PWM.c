// Created by ABKAR on 6/7/2025.

#include "PWM.h"
#include "PWM_Private.h"
#include "Gpio.h"

void PWM_Init(void) {
    // 2. Configure PA6 as Alternate Function (AF1) for TIM3_CH1
    Gpio_Init(GPIO_A, 8, GPIO_AF, GPIO_PUSH_PULL);  // Set PA8 to alternate function mode
    volatile uint32* afrh = (volatile uint32 *)0x40020024; // GPIOA_AFRH register
    *afrh &= ~(0xFUL << ((8 - 8) * 4));  // Clear bits for PA8
    *afrh |= (1UL << ((8 - 8) * 4));     // Set AF1 for TIM1_CH1

    // 3. Configure TIM3 for 1 kHz PWM signal
    // Prescaler = 41 → Timer clock = 42MHz / (41+1) = 1MHz
    TIM1_REG->PSC  = 41;     // Set prescaler
    TIM1_REG->ARR  = 999;    // Auto-reload value to get 1 kHz frequency
    TIM1_REG->CR1  = 0;      // Clear CR1 to reset configuration
    TIM1_REG->CCR1 = 0;      // Start with 0% duty cycle

    // 4. Configure TIM1 Channel 1 for PWM Mode 1
    TIM1_REG->CCMR1 &= ~(7UL << 4);         // Clear output compare mode bits for channel 1
    TIM1_REG->CCMR1 |= TIM_CCMR1_OC1M_PWM1; // Set PWM mode 1 (active until match)
    TIM1_REG->CCMR1 |= TIM_CCMR1_OC1PE;     // Enable preload for CCR1

    // 5. Enable TIM3 Channel 1 output on the pin
    TIM1_REG->CCER |= TIM_CCER_CC1E;        // Enable output for channel 1
    TIM1_REG->BDTR |= (1 << 15);            // MOE = 1 (Main Output Enable)

    // 6. Start the timer with configuration
    TIM1_REG->CR1 |= TIM_CR1_ARPE;          // Enable auto-reload preload
    TIM1_REG->EGR |= TIM_EGR_UG;            // Generate update event to load registers
    TIM1_REG->CR1 |= TIM_CR1_CEN;           // Enable the timer counter
}

void PWM_SetDutyCycle(uint16 duty_percent) {
    // Cap duty cycle at 100% if needed
    if (duty_percent > 100) duty_percent = 100;

    // Compute CCR1 value from percentage and ARR
    TIM1_REG->CCR1 = (duty_percent * (TIM1_REG->ARR + 1)) / 100;
}

void PWM_Stop(void) {
    // Set output compare to 0% (low signal)
    TIM1_REG->CCR1 = 0;
    // Disable the timer
    TIM1_REG->CR1 &= ~TIM_CR1_CEN;
}
