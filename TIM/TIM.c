#include "TIM.h"
#include "GPIO.h"
#include "TIM_Private.h"

#define TIMEOUT_COUNT 10000  // Adjust as needed

void TIM_Init(void) {
    Gpio_Init(GPIO_A, 0, GPIO_AF, GPIO_PULL_DOWN);

    volatile uint32* afrl = (volatile uint32 *)0x40020020; // GPIOA AFRH register
    *afrl &= ~(0xFUL << (0* 4));  // Clear bits for PA8
    *afrl |= (0x2 << (0 * 4));     // Set AF2 for TIM5_CH1

    TIM5_REG->PSC = 0;       // Prescaler (1 MHz timer clock if 84 MHz system clock)
    TIM5_REG->ARR = 0xFFFFFFFF;  // Max auto-reload for 32-bit timer

    TIM5_REG->CCMR1 &= ~(0x3 << 0);     // Clear CC1S
    TIM5_REG->CCMR1 |=  (0x1 << 0);     // CC1S = 01 → CC1 channel is configured as input, mapped on TI1

    TIM5_REG->CCER &= ~(1 << 1);  // Rising edge
    TIM5_REG->CCER |= (1 << 0);   // Enable capture on CH1
    TIM5_REG->CR1 |= (1 << 0);    // Enable counter
}

uint32 TIM_GetValue(void) {
    uint32 t1, t2, period;

    uint32 timeout = 0;

    while (!(TIM5_REG->SR & (1 << 1)))  // Wait for 1st edge
    {
        if (timeout++ > TIMEOUT_COUNT)
        {
            // Timeout occurred
            return 0;
        }
    }

    t1 = TIM5_REG->CCR1;
    TIM5_REG->SR &= ~(0x1 << 1);

    while (!(TIM5_REG->SR & (1 << 1)))  // Wait for 2nd edge
    {
        if (timeout++ > TIMEOUT_COUNT)
        {
            // Timeout occurred
            return 0;
        }
    }

    t2 = TIM5_REG->CCR1;
    TIM5_REG->SR &= ~(0x1 << 1);

    if (t2 > t1)
        period = t2 - t1;
    else
        period = (0xFFFFFFFF - t1) + t2; // Handle timer overflow

    return period;
}