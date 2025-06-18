#ifndef ADC_H
#define ADC_H
#include "Std_Types.h"

#define CR1_RES_12_BIT  (0x0 << 24)
#define CR1_RES_10_BIT  (0x1 << 24)
#define CR1_RES_8_BIT   (0x2 << 24)
#define CR1_RES_6_BIT   (0x3 << 24)

#define RIGHT_ALIGN  (0x0 << 11)
#define LEFT_ALIGN   (0x1 << 11)

#define SMP_3_CYCLES    0x0
#define SMP_15_CYCLES   0x1
#define SMP_28_CYCLES   0x2
#define SMP_56_CYCLES   0x3
#define SMP_84_CYCLES   0x4
#define SMP_112_CYCLES  0x5
#define SMP_144_CYCLES  0x6
#define SMP_480_CYCLES  0x7

#define PSC_DIV_2       (0x0 << 16)
#define PSC_DIV_4       (0x1 << 16)
#define PSC_DIV_6       (0x2 << 16)
#define PSC_DIV_8       (0x3 << 16)

#define ADC_ADON    (0x1 << 0)      // ADC ON/OFF
#define ADC_SWSTART (0x1 << 30)     // Start conversion of regular channels
#define ADC_EOC     (0x1 << 1)      // end of conversion flag

#define ADC1_IN0    GPIO_A, 0
#define ADC1_IN1    GPIO_A, 1
#define ADC1_IN2    GPIO_A, 2
#define ADC1_IN3    GPIO_A, 3
#define ADC1_IN4    GPIO_A, 4
#define ADC1_IN5    GPIO_A, 5
#define ADC1_IN6    GPIO_A, 6
#define ADC1_IN7    GPIO_A, 7
#define ADC1_IN8    GPIO_B, 0
#define ADC1_IN9    GPIO_B, 1
#define ADC1_IN10   GPIO_C, 0
#define ADC1_IN11   GPIO_C, 1
#define ADC1_IN12   GPIO_C, 2
#define ADC1_IN13   GPIO_C, 3
#define ADC1_IN14   GPIO_C, 4
#define ADC1_IN15   GPIO_C, 15

typedef struct {
    volatile  uint32  SR;           // Status register
    volatile  uint32  CR1;          // Control register 1
    volatile  uint32  CR2;          // Control register 2
    volatile  uint32  SMPRx[2];     // Sample time register 1 -> 2
    volatile  uint32  JOFRx[4];     // Injected channel data offset register 1 -> 4
    volatile  uint32  HTR;          // Watchdog higher threshold register
    volatile  uint32  LTR;          // Watchdog lower threshold register
    volatile  uint32  SQRx[3];      // Regular sequence register 1 -> 3
    volatile  uint32  JSQR;         // Injected sequence register
    volatile  uint32  JDRx[4];      // Injected data register 1 -> 4
    volatile  uint32  DR;           // Regular data register
}Adc_Type;

typedef struct {
    volatile uint32 CSR;   // Common status register
    volatile uint32 CCR;   // Common control register
    volatile uint32 CDR;   // Common regular data register
} ADC_Common_Type;

void ADC_Init(uint8 channel);                    // Initialize ADC
uint16 ADC_Read(uint8 channel);                  // Read raw ADC value

#endif //ADC_H
