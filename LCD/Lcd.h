#ifndef LCD_H
#define LCD_H

#include "Gpio.h"
#include "Lcd_Config.h"

#if LCD_MODE == 8
#define FUNCTION_SET  0x38  // 8-bit, 2-line, 5x8 font
#else

#define FUNCTION_SET  0x28  // 4-bit, 2-line, 5x8 font  0b 0010 1000

#endif

#define LCD_DISPLAY_ON_CURSOR_ON        0x0c
#define LCD_DISPLAY_ON_CURSOR_OFF       0x0e
#define LCD_DISPLAY_OFF_CURSOR_OFF      0x08
#define LCD_CLEAR_DISPLAY               0x01
#define LCD_ENTRY_MODE                  0x06    // shift cursor from lift to right on read/write
#define LCD_RETURN_HOME                 0x02    // return AC(address counter) to its original position in DDRAM
#define LCD_CGRAM                       0x40    // shift AC to first location in CGRAM
#define LCD_SET_CURSOR                  0x80    // set position of cursor to write
#define LCD_FUNCTION_RESET              0x30    // As function set (0011 0000) 8-bit, 2-line, 5x11 font

typedef struct __attribute__((packed)) {
    uint8 PortName;
    uint8 PinNumber;
} lcdPin;

void Lcd_Init();
void Lcd_Send_Data(uint8 data);
void Lcd_Send_Command(uint8 command);
void Lcd_Send_String(const uint8 *string);
void Lcd_Clear();
void Lcd_Set_Position(uint8 row, uint8 column);
void Lcd_Print_Number(uint32 num);

#endif //LCD_H
