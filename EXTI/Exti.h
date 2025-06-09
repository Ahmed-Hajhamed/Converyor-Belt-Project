//
// Created by VICTUS on 5/11/2025.
//

#ifndef EXTI_H
#define EXTI_H
#include "Std_Types.h"
#include "Exti_Private.h"
#include "stm32f4xx.h"

typedef struct {
    uint32  IMR;
    uint32  EMR;
    uint32  RTSR;
    uint32  FTSR;
    uint32  SWIER;
    uint32  PR;
}   Exti_TypeDef;

typedef struct {
    uint32  ISER[8];
    uint32  ICER[8];
}   NVIC_TypeDef;

#define EXTI_REG ((EXTI_TypeDef*) EXTI_BASE_ADDRESS)
#define NVIC_REG ((NVIC_TypeDef*) NVIC_BASE_ADDRESS)

void Exti_Init(uint8 Port_Name, uint8 Line, uint8 Falling_Edge, uint8 Rising_Edge) ;       // Line, Port, Falling/Rising/Bus
void Exti_Enable(uint8 Line);  // Line
void Exti_Disable(uint8 Line); // Line

#endif //EXTI_H
