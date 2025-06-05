
#include "Sevenseg.h"
#include "Gpio.h"

Segment segmentTable[7] = {
                    {SEGMENT_A},
                    {SEGMENT_B},
                    {SEGMENT_C},
                    {SEGMENT_D},
                    {SEGMENT_E},
                    {SEGMENT_F},
                    {SEGMENT_G}};

uint8 displayTable[10] = {0x3F, // 0    0011-1111
                        0x06, // 1
                        0x5B, // 2
                        0x4F, // 3
                        0x66, // 4
                        0x6D, // 5
                        0x7D, // 6
                        0x07, // 7
                        0x7F, // 8
                        0x6F, // 9
                        };

void Seven_Seg_Init() {
    for (uint8 i = 0 ; i < 7; i++) {
        Gpio_Init(segmentTable[i].PortName, segmentTable[i].PinNumber,
                                GPIO_OUTPUT, GPIO_PUSH_PULL);
    }
}

void Display_Digit(uint8 number) {
    uint8 hexNumber = displayTable[number]; // 0011-1111 -> 0
    uint8 data;
    for (uint8 i = 0 ; i < 7; i++) {
        data = (hexNumber >> i) & 0x1;
        Gpio_WritePin(segmentTable[i].PortName, segmentTable[i].PinNumber, data);
    }
}