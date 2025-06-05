#include "Keypad.h"

const keypadPin rowPins[KEYPAD_NUM_ROWS] = {{Keypad_R0},
                                      {Keypad_R1},
                                      {Keypad_R2},
                                      {Keypad_R3}};

const keypadPin columnPins[KEYPAD_NUM_COLS] = {{Keypad_C0},
                                         {Keypad_C1},
                                         {Keypad_C2}};

const uint8 table[12] = LOOK_UP_TABLE;