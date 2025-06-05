#include "Keypad.h"

static uint8 pressedKey = 0;
static boolean keyIsUsed = 1;

void Keypad_Init(void) {
    for (uint8 i = 0; i < KEYPAD_NUM_ROWS; i++) {
        Gpio_Init(rowPins[i].PortName, rowPins[i].PinNumber, GPIO_OUTPUT, GPIO_PUSH_PULL);
        Gpio_WritePin(rowPins[i].PortName, rowPins[i].PinNumber, HIGH);
    }

    for (uint8 i = 0; i < KEYPAD_NUM_COLS; i++) {
        Gpio_Init(columnPins[i].PortName, columnPins[i].PinNumber, GPIO_INPUT, GPIO_PULL_UP);
    }
}

void Keypad_Manage(void) {
    for (uint8 i = 0; i < KEYPAD_NUM_ROWS; i++) {
        Gpio_WritePin(rowPins[i].PortName, rowPins[i].PinNumber, LOW);
        for (uint8 j = 0; j < KEYPAD_NUM_COLS; j++) {
            if (ReadButton(columnPins[j].PortName, columnPins[j].PinNumber)) {
                if (keyIsUsed == 1) {
                    pressedKey = table[i * KEYPAD_NUM_COLS + j];
                    keyIsUsed = 0;
                    KeypadCallout_KeyPressNotification();
                }
            }
        }
        Gpio_WritePin(rowPins[i].PortName, rowPins[i].PinNumber, HIGH);
    }
}

uint8 Keypad_Get_Key(void) {
    keyIsUsed = 1;
    return pressedKey;
}

boolean ReadButton(uint8 port_name, uint8 pin_number) {
    static uint8 prev_state = 0x01;
    uint8 state1 = Gpio_ReadPin(port_name, pin_number);

    boolean falling_edge = ((prev_state == 1) && (state1 == 0));
    prev_state = state1;
    return falling_edge;
}
