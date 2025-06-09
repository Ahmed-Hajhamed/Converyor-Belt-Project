#include "TIM.h"
#include <stm32f401xe.h>
#include "GPIO.h"
#define TIMEOUT_COUNT 10000  // Adjust as needed

void TIM_Init(void) {
    Gpio_Init(GPIO_A, 0, GPIO_AF, GPIO_PULL_DOWN);
    GPIOA->AFR[0] &= ~(0xf << (0 * 4));
    GPIOA->AFR[0] |=  (0x2 << (0 * 4));

    TIM5_REG->PSC = 84 - 1;       // Prescaler (1 MHz timer clock if 84 MHz system clock)
    TIM5_REG->ARR = 0xFFFFFFFF;  // Max auto-reload for 32-bit timer

    TIM5_REG->CCMR1 &= ~(0x3 << 0);     // Clear CC1S
    TIM5_REG->CCMR1 |=  (0x1 << 0);     // CC1S = 01 → CC1 channel is configured as input, mapped on TI1

    TIM5_REG->CCER &= ~(1 << 1);  // Rising edge
    TIM5_REG->CCER |= (1 << 0);   // Enable capture on CH1
    TIM5_REG->CR1 |= (1 << 0);    // Enable counter
}

uint32 TIM_GetValue(void) {
    uint32 t1, t2, period;

    uint32_t timeout = 0;

    while (!(TIM5->SR & (1 << 1)))  // Wait for 1st edge
    {
        if (timeout++ > TIMEOUT_COUNT)
        {
            // Timeout occurred
            // Handle error or break
            return 0;
        }
    }

    t1 = TIM5_REG->CCR1;
    TIM5_REG->SR &= ~(0x1 << 1);

    while (!(TIM5->SR & (1 << 1)))  // Wait for 2nd edge
    {
        if (timeout++ > TIMEOUT_COUNT)
        {
            // Timeout occurred
            // Handle error or break
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