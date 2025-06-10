#ifndef TIM_H
#define TIM_H
#include <Std_Types.h>

typedef struct {
    volatile uint32 CR1;    // Control Register 1
    volatile uint32 CR2;    // Control Register 2
    volatile uint32 SMCR;
    volatile uint32 DIER;
    volatile uint32 SR;     // State Register
    volatile uint32 EGR;
    volatile uint32 CCMR1;
    volatile uint32 CCMR2;
    volatile uint32 CCER;
    volatile uint32 CNT;
    volatile uint32 PSC;    // Prescaler Value
    volatile uint32 ARR;    // Auto Reload Register
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

#endif //TIM_H
