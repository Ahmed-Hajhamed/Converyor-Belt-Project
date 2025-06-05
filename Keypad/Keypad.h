
#ifndef KEYPAD_H
#define KEYPAD_H

#include "Gpio.h"
#include "Std_Types.h"

#define KEYPAD_NUM_ROWS   4
#define Keypad_R0 GPIO_A, 9
#define Keypad_R1 GPIO_B, 6
#define Keypad_R2 GPIO_B, 0
#define Keypad_R3 GPIO_C, 7

#define KEYPAD_NUM_COLS   3
#define Keypad_C0 GPIO_A, 0
#define Keypad_C1 GPIO_A, 10
#define Keypad_C2 GPIO_B, 9

#define LOOK_UP_TABLE {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0}

typedef struct __attribute__((packed)) {
    uint8 PortName;
    uint8 PinNumber;
} keypadPin;

extern const keypadPin rowPins[];
extern const keypadPin columnPins[];
extern const uint8 table[];

// Provided Interface
void Keypad_Init(void);

void Keypad_Manage(void);

uint8 Keypad_Get_Key(void);

// Required Interface
void KeypadCallout_KeyPressNotification(void);

boolean ReadButton(uint8 port_name, uint8 pin_number);

#endif //KEYPAD_H
