#include "RCC.h"
#include "Gpio.h"
#include "Std_Types.h"
#include "ADC.h"
#include "LCD.h"
#include "TIM.h"
#include "PWM.h"
#include "Exti.h"

boolean Read_Button(uint8 port_name, uint8 pin_number);

#define ADC_CHANNEL 1
#define OBJECT_COUNT_BUTTON GPIO_B, 2
#define EMERGENCY_INTERRUPT_PORT GPIO_B
#define EMERGENCY_INTERRUPT_LINE 6 /* Note: When changing the interrupt line,
                             make sure to use the corresponding handler function below. */
boolean emergency_stop = 0;

int main() {
    Rcc_Init();
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_GPIOB);
    Rcc_Enable(RCC_GPIOC);
    Rcc_Enable(RCC_SYSCFG);
    Rcc_Enable(RCC_ADC1);
    Rcc_Enable(RCC_TIM5);
    Rcc_Enable(RCC_TIM1);

    ADC_Init(ADC_CHANNEL);
    Lcd_Init();
    TIM_Init();
    PWM_Init();

    Exti_Init(EMERGENCY_INTERRUPT_PORT, EMERGENCY_INTERRUPT_LINE, 1, 0);
    Exti_Enable(EMERGENCY_INTERRUPT_LINE);

    Gpio_Init(EMERGENCY_INTERRUPT_PORT, EMERGENCY_INTERRUPT_LINE,
                GPIO_INPUT, GPIO_PULL_UP);
    Gpio_Init(OBJECT_COUNT_BUTTON, GPIO_INPUT, GPIO_PULL_UP);

    uint8 object_count = 0;
    uint8 prev_percentage = 255;  // impossible initial value
    uint8 prev_object_count = 255;
    uint32 prev_period = 0xFFFFFFFF;
    boolean local_emergency_stop = 0;

    while (1) {
        Exti_Disable(EMERGENCY_INTERRUPT_LINE);    // Handle the Race Condition
        local_emergency_stop = emergency_stop;
        Exti_Enable(EMERGENCY_INTERRUPT_LINE);

        if (local_emergency_stop == 1) {
            PWM_Stop();
            Lcd_Set_Position(0, 0);
            Lcd_Send_String((uint8*)"EMERGENCY STOP  ");
            for (uint32 i=0; i<=50000;i++){} // delay
            Lcd_Clear();
            object_count = 0;

            // Reset prev values so it reprints next time
            prev_percentage = 255;
            prev_object_count = 255;
            prev_period = 0xFFFFFFFF;
        }
        else {
            if (Read_Button(OBJECT_COUNT_BUTTON)) {
                object_count++;
            }

            uint16 adc_value = ADC_Read(ADC_CHANNEL);
            uint8 percentage = ((uint32)adc_value * 100) / 4095;

            PWM_SetDutyCycle(percentage);

            uint32 period = TIM_GetValue();
            uint32 freq = (16000000.0 / period);

            if (percentage != prev_percentage) {
                Lcd_Set_Position(0, 0);
                Lcd_Print_Number(percentage);
                Lcd_Send_String((uint8 *)" % ");  // Add padding
                prev_percentage = percentage;
            }


            if (object_count != prev_object_count) {
                Lcd_Set_Position(0, 7);
                Lcd_Print_Number(object_count);
                Lcd_Send_String((uint8 *)" Object");  // Add padding
                prev_object_count = object_count;
            }

            if (period != prev_period) {
                Lcd_Set_Position(1, 0);
                Lcd_Print_Number(freq);
                Lcd_Send_String((uint8 *)"Hz      ");  // Clear trailing digits
                prev_period = period;
            }
        }
    }
    return 0;
}

void EXTI9_5_IRQHandler() {
    if (Exti_IsPending(EMERGENCY_INTERRUPT_LINE)) {
        Exti_Clear_Flag(EMERGENCY_INTERRUPT_LINE);
        emergency_stop = !emergency_stop;
    }
}

boolean Read_Button(uint8 port_name, uint8 pin_number) {
    static uint8 prev_state = 0x01;
    uint8 state1 = Gpio_ReadPin(port_name, pin_number);

    boolean falling_edge = ((prev_state == 1) && (state1 == 0));
    prev_state = state1;
    return falling_edge;
}
