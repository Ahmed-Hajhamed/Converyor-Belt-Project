#include "Std_Types.h"
#include "Gpio.h"
#include "Exti.h"

uint8 NvicLines[16] = NVIC_ENABLE_LINES;

void Exti_Init(uint8 Port_Name, uint8 Line, uint8 Falling_Edge, uint8 Rising_Edge) {
    uint8 ExticrRegIdx = Line / 4;
    uint8 ExticrBitsIndex = Line % 4;

    SYSCFG->EXTICR[ExticrRegIdx] &= ~(0x0f << (ExticrBitsIndex * 4));
    SYSCFG->EXTICR[ExticrRegIdx] |= ((Port_Name - GPIO_A) << (ExticrBitsIndex * 4));

    if (Falling_Edge)
        EXTI_REG->FTSR  |= (0x01 << Line);

    if (Rising_Edge)
        EXTI_REG->RTSR  |= (0x01 << Line);
}

void Exti_Enable(uint8 Line) {
    EXTI_REG->IMR |= (0x01 << Line);
    NVIC_REG->ISER[NvicLines[Line] / 32] |= (0x01 << (NvicLines[Line] % 32));
}

void Exti_Disable(uint8 Line) {
    EXTI_REG->IMR &= ~(0x01 << Line);
    NVIC_REG->ICER[NvicLines[Line] / 32] |= (0x01 << (NvicLines[Line] % 32));
}
