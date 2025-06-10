#include "ADC.h"
#include <Gpio.h>
#include "ADC_Config.h"
#include "ADC_Private.h"

void ADC_Init(void) {
    Gpio_Init(GPIO_A, 1, GPIO_ANALOG, GPIO_NO_PULL_DOWN); // set PA0 to analog

    ADC_COMMON->CCR &= ~(0x3 << 16);
    ADC_COMMON->CCR |= (PSC_DIVISION);        // pre scaler division by 8 = 10.5 MHz, (max = 36 MHz)

    ADC_REG->SMPRx[1] &= ~(0x7 << (0 * 3));
    ADC_REG->SMPRx[1] |= (ADC_SMP_TIME << (0 * 3)); // set sampling time to 28 cycles

    ADC_REG->CR1 &= ~(0x3 << 24);
    ADC_REG->CR1 |= (ADC_RESOLUTION);     // set resolution

    ADC_REG->CR2 &= ~(0x1 << 11);
    ADC_REG->CR2 |= (DATA_ALIGNMENT);     // set data alignment

    ADC_REG->SQRx[0] &= ~(0xF <<(20));  // total number of conversions = one

    ADC_REG->SQRx[2] &= ~(0x1F);
    ADC_REG->SQRx[2] |= (0x01);       // set the first conversion to ADC CH1

    ADC_REG->CR2 |= (ADC_ADON);            // adc enable
}

uint16 ADC_Read(void) {
    ADC_REG->CR2 |= (ADC_SWSTART);        // start conversion

    while (! (ADC_REG->SR & ADC_EOC)){}    // wait for EOC, cleared by hardware

    return ADC_REG->DR;
}
