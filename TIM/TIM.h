#ifndef TIM_H
#define TIM_H
#include <Std_Types.h>
#include "TIM_Private.h"

typedef struct {
    volatile uint32 CR1;
    volatile uint32 CR2;
    volatile uint32 SMCR;
    volatile uint32 DIER;
    volatile uint32 SR;
    volatile uint32 EGR;
    volatile uint32 CCMR1;
    volatile uint32 CCMR2;
    volatile uint32 CCER;
    volatile uint32 CNT;
    volatile uint32 PSC;
    volatile uint32 ARR;
    uint32 RESERVED1;
    volatile uint32 CCR1;
    volatile uint32 CCR2;
    volatile uint32 CCR3;
    volatile uint32 CCR4;
    uint32 RESERVED2;
    volatile uint32 DCR;
    volatile uint32 DMAR;
    volatile uint32 OR;
} TIM_Type;

void TIM_Init(void);
uint32 TIM_GetValue(void);

#define TIM5_REG ((TIM_Type *)TIM5_BASE_ADDRESS)

#endif //TIM_H
