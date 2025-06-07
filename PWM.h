//
// Created by ABKAR on 6/7/2025.
//

#ifndef PWM_H
#define PWM_H
#include "Std_Types.h"

void PWM_Init(void);                    // Initialize PWM on PA6 (TIM3_CH1)
void PWM_SetDutyCycle(uint16 duty_percent); // Set PWM duty cycle (0-100%)
void PWM_Stop(void);                    // Stop PWM signal

#endif // PWM_H