
#ifndef SEVENSEG_H
#define SEVENSEG_H

#include "Std_Types.h"
#include "Gpio.h"

#define SEGMENT_A   GPIO_A, 2
#define SEGMENT_B   GPIO_A, 11
#define SEGMENT_C   GPIO_B, 4
#define SEGMENT_D   GPIO_C, 6
#define SEGMENT_E   GPIO_C, 2
#define SEGMENT_F   GPIO_A, 12
#define SEGMENT_G   GPIO_A, 4

typedef struct __attribute__((packed)) {
    uint8 PortName;
    uint8 PinNumber;
} Segment;

void Seven_Seg_Init();

void Display_Digit(uint8 number);

#endif //SEVENSEG_H
