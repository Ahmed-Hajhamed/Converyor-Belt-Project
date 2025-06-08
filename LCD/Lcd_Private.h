
#ifndef LCD_PRIVATE_H
#define LCD_PRIVATE_H

#include "Lcd.h"

static void Lcd_Enable(boolean enable);
static void Lcd_Send_Falling_Edge();

const lcdPin controlPins[3]= {
    {LCD_RS},
    {LCD_RW},
    {LCD_E},
};

const lcdPin dataPins_8[8]= {
    {LCD_D0},
    {LCD_D1},
    {LCD_D2},
    {LCD_D3},
    {LCD_D4},
    {LCD_D5},
    {LCD_D6},
    {LCD_D7}
};

const lcdPin dataPins_4[4]={
    {LCD_D7},
    {LCD_D6},
    {LCD_D5},
    {LCD_D4}
};


#endif //LCD_PRIVATE_H
