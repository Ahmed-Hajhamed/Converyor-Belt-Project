#ifndef ADC_H
#define ADC_H
#include "Std_Types.h"
#include "ADC_Private.h"

#define RES_12_BIT 0x0
#define RES_10_BIT 0x1
#define RES_9_BIT 0x2
#define RES_6_BIT 0x3

#define RIGHT_ALGN 0x0
#define LEFT_ALGN 0x1

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

#define ADC_COMMON ((ADC_Common_Type *) ADC_COMMON_BASE)
#define ADC_REG ((Adc_Type*) ADC_BASE_ADDRESS)

void ADC_Init(void);                    // Initialize ADC
uint16 ADC_Read(void);                  // Read raw ADC value

#endif //ADC_H
