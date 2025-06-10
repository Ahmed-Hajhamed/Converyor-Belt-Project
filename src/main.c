#include "RCC.h"
#include "Gpio.h"
#include "Std_Types.h"
#include "ADC.h"
#include "LCD.h"
#include "TIM.h"
#include "PWM.h"
#include "Exti.h"

void ConvertToVoltage(uint16 adc_value, uint8 digits[4]);
void Display_Voltage_On_LCD(uint8 digits[4]);
void Lcd_Print_Number(uint32 num);
boolean ReadButton(uint8 port_name, uint8 pin_number);

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

    ADC_Init();
    Lcd_Init();
    TIM_Init();
    PWM_Init();

    Exti_Init(GPIO_A, 3, 1, 0);
    Exti_Enable(3);

    Gpio_Init(GPIO_A, 3, GPIO_INPUT, GPIO_PULL_UP);
    Gpio_Init(GPIO_A, 4, GPIO_INPUT, GPIO_PULL_UP);
    Gpio_Init(GPIO_A, 7, GPIO_OUTPUT, GPIO_PUSH_PULL);

    uint8 object_count = 0;
    uint8 prev_percentage = 255;  // impossible initial value
    uint8 prev_object_count = 255;
    uint32 prev_period = 0xFFFFFFFF;

    while (1) {
        if (emergency_stop == 1) {
            PWM_Stop();
            Lcd_Set_Position(0, 0);
            Lcd_Send_String((uint8*)"EMERGENCY STOP  ");
            Lcd_Clear();
            object_count = 0;

            // Reset prev values so it reprints next time
            prev_percentage = 255;
            prev_object_count = 255;
            prev_period = 0xFFFFFFFF;
        }
        else {
            uint16 adc_value = ADC_Read();
            uint8 percentage = ((uint32)adc_value * 100) / 4095;
            PWM_SetDutyCycle(percentage);

            if (percentage != prev_percentage) {
                Lcd_Set_Position(0, 0);
                Lcd_Print_Number(percentage);
                Lcd_Send_String((uint8 *)" % ");  // Add padding
                prev_percentage = percentage;
            }

            if (ReadButton(GPIO_A, 4)) {
                object_count++;
            }

            if (object_count != prev_object_count) {
                Lcd_Set_Position(0, 7);
                Lcd_Print_Number(object_count);
                Lcd_Send_String((uint8 *)" Object");  // Add padding
                prev_object_count = object_count;
            }

            uint32 period = TIM_GetValue();
            float32 freq = 1.0 / (period / 16000000.0);
            if (period != prev_period) {
                Lcd_Set_Position(1, 0);
                Lcd_Print_Number(freq);
                Lcd_Send_String((uint8 *)"Hz    ");  // Clear trailing digits
                prev_period = period;
            }
        }
    }
    return 0;
}


void ConvertToVoltage(uint16 adc_value, uint8 digits[4]) {
    uint16 mv = ((uint32)adc_value * 3300) / 4095;  // 3.3V VREF, 12-bit ADC

    digits[0] = mv / 1000;          // Thousands
    digits[1] = (mv / 100) % 10;    // Hundreds
    digits[2] = (mv / 10) % 10;     // Tens
    digits[3] = mv % 10;            // Units
}

void Display_Voltage_On_LCD(uint8 digits[4]) {
    // Lcd_Clear();
    Lcd_Set_Position(0, 0);

    for (int i = 0; i < 4; i++) {
        Lcd_Send_Data('0' + digits[i]);  // Convert digit to ASCII
    }

    Lcd_Send_String((uint8 *)" mV");
}


void Lcd_Print_Number(uint32 num) {
    char buffer[10];
    int i = 0;

    // Handle zero explicitly
    if (num == 0) {
        Lcd_Send_Data('0');
        return;
    }

    // Convert each digit in reverse order
    while (num > 0 && i < 10) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    // Print digits in correct order
    while (i > 0) {
        Lcd_Send_Data(buffer[--i]);
    }
}
void EXTI3_IRQHandler() {
    EXTI_REG->PR |= (0x01 << 3);
    emergency_stop = !emergency_stop;
}

boolean ReadButton(uint8 port_name, uint8 pin_number) {
    static uint8 prev_state = 0x01;
    uint8 state1 = Gpio_ReadPin(port_name, pin_number);

    boolean falling_edge = ((prev_state == 1) && (state1 == 0));
    prev_state = state1;
    return falling_edge;
}
