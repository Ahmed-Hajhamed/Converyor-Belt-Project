#include "Gpio.h"
#include "Std_Types.h"
#include "Lcd_Config.h"
#include "Lcd_Private.h"

void delay_ms(uint32 ms) {
    for (uint32 i = 0; i < ms * 1000; i++) {}
}

void Lcd_Init() {
    delay_ms(50);

    for (uint8 i =0; i < 3; i++) {
        Gpio_Init(controlPins[i].PortName,controlPins[i].PinNumber,GPIO_OUTPUT, GPIO_PUSH_PULL);
    }

    for (uint8 i =0; i < 8; i++) {
        Gpio_Init(dataPins_8[i].PortName,dataPins_8[i].PinNumber,GPIO_OUTPUT, GPIO_PUSH_PULL);
    }
    Lcd_Enable(HIGH);

    //function set
    #if LCD_MODE==4
    for (uint8 i =0; i < 2; i++) {
        Gpio_WritePin(controlPins[i].PortName,controlPins[i].PinNumber, LOW);
    }
    for (uint8 i =0; i < 4; i++) {
        Gpio_WritePin(dataPins_4[i].PortName, dataPins_4[i].PinNumber, (0x1& (0x20>>(7-i)) ) );
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
#if LCD_MODE ==8
        for (uint8 i =0; i < 2; i++) {
            Gpio_WritePin(controlPins[i].PortName,controlPins[i].PinNumber, LOW);
        }
        for (uint8 i =0; i < 8; i++) {
            Gpio_WritePin(dataPins_8[i].PortName, dataPins_8[i].PinNumber, (0x1& (command >>i )) );
        }
        Lcd_Send_Falling_Edge();
#elif LCD_MODE == 4
    for (uint8 j=0 ; j < 2; j++) {
        for (uint8 i =0; i < 2; i++) {
            Gpio_WritePin(controlPins[i].PortName,controlPins[i].PinNumber, LOW);
        }
        for (uint8 i =0; i < 4; i++) {
            Gpio_WritePin(dataPins_4[i].PortName, dataPins_4[i].PinNumber, (0x1& (command >> (7-(i + 4*j)) ) ) ) ;
        }
        Lcd_Send_Falling_Edge();
    }

#endif
    delay_ms(1);
}

void Lcd_Send_Data(uint8 data) {
#if LCD_MODE == 8// 8 bits mode
        for (uint8 i =0; i < 2; i++) {
            Gpio_WritePin(controlPins[i].PortName,controlPins[i].PinNumber, (0x1&(0x1 >>i )) );
        }
        for (uint8 i =0; i < 8; i++) {
            Gpio_WritePin(dataPins_8[i].PortName, dataPins_8[i].PinNumber, (0x1& (data >>i ) ));
        }
        Lcd_Send_Falling_Edge();
#elif LCD_MODE == 4 // 4 bits mode
    for (uint8 j =0 ; j < 2; j++) {
        for (uint8 i =0; i < 2; i++) {
            Gpio_WritePin(controlPins[i].PortName,controlPins[i].PinNumber, (0x1&(0x1 >>i )) );
        }
        for (uint8 i =0; i < 8; i++) {
            Gpio_WritePin(dataPins_4[i].PortName, dataPins_4[i].PinNumber, (0x1& (data >>(7-(i + 4*j)) ) ));
        }
        Lcd_Send_Falling_Edge();
    }

#endif
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

static void Lcd_Send_Falling_Edge() {
    Lcd_Enable(LOW);
    delay_ms(1);
    Lcd_Enable(HIGH);
    delay_ms(1);
}
