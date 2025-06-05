#ifndef ADC_H
#define ADC_H
#include "Std_Types.h"
#include "ADC_Private.h"

#define RES_12_BIT 0x00
#define RES_10_BIT 0x01
#define RES_9_BIT 0x02
#define RES_6_BIT 0x03

#define RIGHT_ALGN 0x00
#define LEFT_ALGN 0x01

typedef struct {
    volatile  uint32  SR;           // status register
    volatile  uint32  CR1;          // control register 1
    volatile  uint32  CR2;          // control register 2
    volatile  uint32  SMPRx[2];     // sample time register 1 - 2
    volatile  uint32  JOFRx[4];     // injected channel data offset register 1 - 4
    volatile  uint32  HTR;          // watchdog higher threshold register
    volatile  uint32  LTR;          // watchdog lower threshold register
    volatile  uint32  SQRx[3];      //  regular sequence register 1 - 3
    volatile  uint32  JSQR;         //  injected sequence register
    volatile  uint32  JDRx[4];      //  injected data register 1 - 4
    volatile  uint32  DR;           // regular data register
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
