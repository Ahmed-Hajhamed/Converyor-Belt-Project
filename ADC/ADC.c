#include "ADC.h"
#include <Gpio.h>

void ADC_Init(void) {
    Gpio_Init(GPIO_A, 0, GPIO_ANALOG, GPIO_NO_PULL_DOWN); // set PA0 to analog

    ADC_COMMON->CCR &= ~(0x3 << 16);
    ADC_COMMON->CCR |= (0x3 << 16);        // pre scaler division by 8 = 10.5 MHz

    ADC_REG->SMPRx[1] &= ~(0x7 << (0 * 2));
    ADC_REG->SMPRx[1] |= (0x2 << (0 * 2)); // set sampling time to 28 cycles

    ADC_REG->CR1 &= ~(0x3 << 24);
    ADC_REG->CR1 |= (RES_12_BIT << 24);     // set resolution

    ADC_REG->CR2 &= ~(0x1 << 11);
    ADC_REG->CR2 |= (RIGHT_ALGN << 11);     // set data alignment

    ADC_REG->CR2 |= (0x1 << 0);            // adc enable
}

uint16 ADC_Read(void) {
    ADC_REG->SQRx[2] &= ~(0x1F);        // total number of conversions = one

    ADC_REG->SQRx[0] &= ~(0xF <<(20));  // set the first conversion to ADC CH0

    ADC_REG->CR2 |= (0x1 << 30);        // start conversion

    while (! (ADC_REG->SR & (0x1 << 1))){}    // wait for EOC, cleared by hardware

    return ADC_REG->DR;
}

