#ifndef EXTI_H
#define EXTI_H
#include "Std_Types.h"

typedef struct {
    volatile uint32  IMR;    /*!< Interrupt mask register (EXTI_IMR) Address offset: 0x00 */
    volatile uint32  EMR;    /*!< Event mask register (EXTI_EMR) Address offset: 0x04 */
    volatile uint32  RTSR;   /*!<  Rising trigger selection register (EXTI_RTSR) Address offset: 0x08 */
    volatile uint32  FTSR;   /*!<  Falling trigger selection register (EXTI_FTSR) Address offset: 0x0C */
    volatile uint32  SWIER;  /*!< Software interrupt event register (EXTI_SWIER) Address offset: 0x10 */
    volatile uint32  PR;     /*!< Pending register (EXTI_PR) Address offset: 0x14 */
}   EXTI_Type;

typedef struct {
    volatile uint32  ISER[8];
    volatile uint32  ICER[8];
}   NVIC_TypeDef;

typedef struct {
    volatile uint32 MEMRMP;  // 0x00
    volatile uint32 PMC;     // 0x04
    volatile uint32 EXTICR[4];  // 0x08 to 0x14
    uint32 RESERVED[2];         // 0x18 & 0x1C
    volatile uint32 CMPCR;   // 0x20
}   SYSCFG_Type;

void Exti_Init(uint8 Port_Name, uint8 Line, uint8 Falling_Edge, uint8 Rising_Edge) ;  // Line, Port, Falling/Rising/Bus
uint8 Exti_IsPending(uint8 Line);
void Exti_Clear_Flag(uint8 Line);
void Exti_Enable(uint8 Line);  // Line
void Exti_Disable(uint8 Line); // Line

#endif //EXTI_H
