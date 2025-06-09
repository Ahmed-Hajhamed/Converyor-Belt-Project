#include <Speed_Sensor.h>

#include "RCC.h"
#include "Gpio.h"
#include "Std_Types.h"
#include "ADC.h"
#include "LCD.h"

void SevenSegDisplay(uint8 number);
void ConvertToVoltage(uint16 adc_value, uint8 digits[4]);
void Display_Voltage_On_LCD(uint8 digits[4]);
void Lcd_Print_Number(uint32 num);

#define SEGMENT_A  GPIO_B, 0
#define SEGMENT_B  GPIO_B, 1
#define SEGMENT_C  GPIO_B, 2
#define SEGMENT_D  GPIO_B, 3
#define SEGMENT_E  GPIO_B, 4
#define SEGMENT_F  GPIO_B, 5
#define SEGMENT_G  GPIO_B, 6

uint8 sevenSegmentMap[7][2] = {
    {SEGMENT_A},
    {SEGMENT_B},
    {SEGMENT_C},
    {SEGMENT_D},
    {SEGMENT_E},
    {SEGMENT_F},
    {SEGMENT_G}
};

int main() {
    Rcc_Init();
    Rcc_Enable(RCC_GPIOA);
    Rcc_Enable(RCC_GPIOB);
    Rcc_Enable(RCC_GPIOC);
    Rcc_Enable(RCC_ADC1);

    // ADC_Init();
    Lcd_Init();
    Speed_Sensor_Init();

    uint32 i;
    for (i = 0; i < 7; i++) {
        Gpio_Init(sevenSegmentMap[i][0], sevenSegmentMap[i][1],
         GPIO_OUTPUT, GPIO_PUSH_PULL);
    }

    Gpio_Init(GPIO_B, 7, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(GPIO_B, 8, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(GPIO_B, 9, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_Init(GPIO_B, 10, GPIO_OUTPUT, GPIO_PUSH_PULL);

    Gpio_WritePin(GPIO_B, 7, HIGH);
    Gpio_WritePin(GPIO_B, 8, HIGH);
    Gpio_WritePin(GPIO_B, 9, HIGH);
    Gpio_WritePin(GPIO_B, 10,HIGH);

    while (1) {
        uint8 digits[4];
        // uint16 adc_value = ADC_Read();
        // ConvertToVoltage(adc_value, digits);
        // Display_Voltage_On_LCD(digits);
        uint32 period;
        uint32 freq;
        Speed_Sensor_MeasurePeriod(&period);
        Speed_Sensor_CalculateFrequency(period, &freq);
        Lcd_Clear();
        Lcd_Print_Number(period);

        // Use the code below to use the 7 segment display
        Gpio_WritePin(GPIO_B, 8, LOW);
        SevenSegDisplay(digits[3]);
        for (i = 0; i < (1000000 / 1000) * 5; i++) {}
        Gpio_WritePin(GPIO_B, 8, HIGH);

        Gpio_WritePin(GPIO_B, 7, LOW);
        SevenSegDisplay(digits[2]);
        for (i = 0; i < (1000000 / 1000) * 5; i++) {}
        Gpio_WritePin(GPIO_B, 7, HIGH);

        Gpio_WritePin(GPIO_B, 10, LOW);
        SevenSegDisplay(digits[1]);
        for (i = 0; i < (1000000 / 1000) * 5; i++) {}
        Gpio_WritePin(GPIO_B, 10, HIGH);

        Gpio_WritePin(GPIO_B, 9, LOW);
        SevenSegDisplay(digits[0]);
        for (i = 0; i < (1000000 / 1000) * 5; i++) {}
        Gpio_WritePin(GPIO_B, 9, HIGH);

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

void SevenSegDisplay(uint8 number) {
    if (number > 9) {
        return;
    }
    const uint8 numbers_array[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
    for (uint8 i = 0; i < 7; i++) {
        Gpio_WritePin(sevenSegmentMap[i][0], sevenSegmentMap[i][1],
                      ((numbers_array[number] >> i) & 0x01));
    }
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
