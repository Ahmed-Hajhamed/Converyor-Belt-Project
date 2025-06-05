#include "ADC.h"
#include <Gpio.h>

void ADC_Init(void) {
    Gpio_Init(GPIO_A, 0, GPIO_ANALOG, GPIO_NO_PULL_DOWN); // set PA0 to analog

    ADC_COMMON->CCR &= ~(0x03 << 16);
    ADC_COMMON->CCR |= (0x03 << 16);  // pre scaler division by 8 = 10.5 MHz

    ADC_REG->SMPRx[1] &= ~(0x07 << (0 * 2));
    ADC_REG->SMPRx[1] |= (0x05 << (0 * 2)); // set sampling time to 28 cycles

    ADC_REG->CR1 &= ~(0x03 << 24);
    ADC_REG->CR1 |= (RES_12_BIT << 24); // set resolution

    ADC_REG->CR2 &= ~(0x01 << 11);
    ADC_REG->CR2 |= (RIGHT_ALGN << 11);  // set data alignment

    ADC_REG->CR2 &= ~(1 << 1);  // CONT = 0

    ADC_REG->CR2 |= (0x01 << 0);       // adc enable
}

uint16 ADC_Read(void) {
    ADC_REG->SQRx[2] &= ~(0x1F);    // total #conversion is one

    ADC_REG->SQRx[0] &= ~(0xF <<(20)); // set 1 conversion

    ADC_REG->CR2 |= (0x1 << 30);   // start conversion

    while (! (ADC_REG->SR & (0x1 << 1)));    // wait for EOC, cleared by hardware

    // return ADC_REG->DR;
    return ADC_REG->DR;
}

