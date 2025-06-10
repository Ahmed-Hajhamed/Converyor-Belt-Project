#include "Gpio.h"
#include "Std_Types.h"
#include "Lcd_Config.h"
#include "Lcd_Private.h"

const lcdPin controlPins[3]= {
    {LCD_RS},
    {LCD_RW},
    {LCD_E},
};

const lcdPin dataPins[8]= {
    {LCD_D7},
    {LCD_D6},
    {LCD_D5},
    {LCD_D4},
    {LCD_D3},
    {LCD_D2},
    {LCD_D1},
    {LCD_D0}
};

void delay_ms(uint32 ms) {
    for (uint32 i = 0; i < ms * 1000; i++) {}
}

void Lcd_Init() {
    delay_ms(50);

    for (uint8 i =0; i < 3; i++) {
        Gpio_Init(controlPins[i].PortName,controlPins[i].PinNumber,GPIO_OUTPUT, GPIO_PUSH_PULL);
    }

    for (uint8 i =0; i < 8; i++) {
        Gpio_Init(dataPins[i].PortName,dataPins[i].PinNumber,GPIO_OUTPUT, GPIO_PUSH_PULL);
    }
    Lcd_Enable(HIGH);

    //function set
    #if LCD_MODE==4
    for (uint8 i =0; i < 2; i++) {
        Gpio_WritePin(controlPins[i].PortName,controlPins[i].PinNumber, LOW);
    }
    for (uint8 i =0; i < 4; i++) {
        Gpio_WritePin(dataPins[i].PortName, dataPins[i].PinNumber, (0x1& (0x20>>(7-i)) ) );
    }
    Lcd_Send_Falling_Edge();
    #endif

    Lcd_Send_Command(FUNCTION_SET);
    delay_ms(1);

    //Display ON/OFF
    Lcd_Send_Command(LCD_DISPLAY_ON_CURSOR_OFF);
    delay_ms(1);

    //Display clear
    Lcd_Clear();

    //Entry mode set
    Lcd_Send_Command(LCD_ENTRY_MODE);
}

void Lcd_Send_Command(uint8 command) {
for (uint8 i =0; i < 2; i++) {
        Gpio_WritePin(controlPins[i].PortName,controlPins[i].PinNumber, LOW);
}

#if LCD_MODE ==8
        for (uint8 i =0; i < 8; i++) {
            Gpio_WritePin(dataPins[i].PortName, dataPins[i].PinNumber, (0x1& (command >>(7-i) )) );
        }
#elif LCD_MODE == 4
    for (uint8 j=0 ; j < 2; j++) {
        for (uint8 i =0; i < 4; i++) {
            Gpio_WritePin(dataPins[i].PortName, dataPins[i].PinNumber, (0x1& (command >> (7-(i + 4*j)) ) ) ) ;
        }
    }
#endif

Lcd_Send_Falling_Edge();
delay_ms(1);
}

void Lcd_Send_Data(uint8 data) {

for (uint8 i =0; i < 2; i++) {
    Gpio_WritePin(controlPins[i].PortName,controlPins[i].PinNumber, (0x1&(0x1 >>i )) );
}

#if LCD_MODE == 8// 8 bits mode
        for (uint8 i =0; i < 8; i++) {
            Gpio_WritePin(dataPins[i].PortName, dataPins[i].PinNumber, (0x1& (data >>(7-i) ) ));
        }
#elif LCD_MODE == 4 // 4 bits mode
    for (uint8 j =0 ; j < 2; j++) {
        for (uint8 i =0; i < 8; i++) {
            Gpio_WritePin(dataPins[i].PortName, dataPins[i].PinNumber, (0x1& (data >>(7-(i + 4*j)) ) ));
        }
    }

#endif
Lcd_Send_Falling_Edge();
delay_ms(1);
}

void Lcd_Send_String(const uint8 *string) {
    uint8 localIterator = 0;
    while (string[localIterator] != '\0') {
        Lcd_Send_Data(string[localIterator]);
        localIterator++;
    }
}

void Lcd_Clear() {
    Lcd_Send_Command(LCD_CLEAR_DISPLAY);
    delay_ms(10);
}

void Lcd_Enable(boolean enable) {
    Gpio_WritePin(controlPins[2].PortName, controlPins[2].PinNumber,enable);
}

void Lcd_Set_Position(uint8 row, uint8 column) {
    uint8 localVariable = 0;

    if ( (row > 1) || (column > 15) ) {
        localVariable = LCD_SET_CURSOR;
    }
    else if(row == 0) {
        localVariable = LCD_SET_CURSOR + (column);
    }
    else if (row == 1){
        localVariable = LCD_SET_CURSOR +(64)+ (column);
    }
    Lcd_Send_Command(localVariable);
    delay_ms(1);
}

void Lcd_Print_Number(uint32 num) {
    char buffer[10];
    int i = 0;

    // Handle zero explicitly
    if (num == 0) {
        Lcd_Send_Data('0');
        return;
    }

    // Convert each digit in reverse order
    while (num > 0 && i < 10) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    // Print digits in correct order
    while (i > 0) {
        Lcd_Send_Data(buffer[--i]);
    }
}

static void Lcd_Send_Falling_Edge() {
    Lcd_Enable(LOW);
    delay_ms(1);
    Lcd_Enable(HIGH);
    delay_ms(1);
}
