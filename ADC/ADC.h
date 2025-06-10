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

void ADC_Init(void);                    // Initialize ADC
uint16 ADC_Read(void);                  // Read raw ADC value

#endif //ADC_H
