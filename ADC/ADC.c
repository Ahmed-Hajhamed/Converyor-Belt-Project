#include "ADC.h"
#include <Gpio.h>
#include "ADC_Config.h"
#include "ADC_Private.h"
uint8 adc1ChannelsMap [16][2] = {{ADC1_IN0}, {ADC1_IN1}, {ADC1_IN2}, {ADC1_IN3}, {ADC1_IN4}, {ADC1_IN5},
                                {ADC1_IN6}, {ADC1_IN7}, {ADC1_IN8}, {ADC1_IN9}, {ADC1_IN10},
                                {ADC1_IN11}, {ADC1_IN12}, {ADC1_IN13}, {ADC1_IN14}, {ADC1_IN15}};

void ADC_Init(uint8 channel) {
    Gpio_Init(adc1ChannelsMap[channel][0],adc1ChannelsMap[channel][1],
                                GPIO_ANALOG, GPIO_NO_PULL_DOWN); // set Channel to analog

    ADC_COMMON->CCR &= ~(0x3 << 16);
    ADC_COMMON->CCR |= (PSC_DIVISION);        // pre scaler division by 8 = 10.5 MHz, (max = 36 MHz)

    ADC_REG->SMPRx[1] &= ~(0x7 << (0 * 3));
    ADC_REG->SMPRx[1] |= (ADC_SMP_TIME << (0 * 3)); // set sampling time to 28 cycles

    ADC_REG->CR1 &= ~(0x3 << 24);
    ADC_REG->CR1 |= (ADC_RESOLUTION);     // set resolution

    ADC_REG->CR2 &= ~(0x1 << 11);
    ADC_REG->CR2 |= (DATA_ALIGNMENT);     // set data alignment

    ADC_REG->SQRx[0] &= ~(0xF <<(20));  // total number of conversions = one

    ADC_REG->CR2 |= (ADC_ADON);            // adc enable
}

uint16 ADC_Read(uint8 channel) {
    ADC_REG->SQRx[2] &= ~(0x1F);
    ADC_REG->SQRx[2] |= (channel);       // set the first conversion to selected channel

    ADC_REG->CR2 |= (ADC_SWSTART);        // start conversion

    while (! (ADC_REG->SR & ADC_EOC)){}    // wait for EOC, cleared by hardware

    return ADC_REG->DR;
}
