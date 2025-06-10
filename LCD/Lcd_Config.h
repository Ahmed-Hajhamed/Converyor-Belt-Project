
#ifndef LCD_CONFIG_H
#define LCD_CONFIG_H


#define LCD_MODE  8  // 4 or 8

// VSS--> 0v | VDD-->5v | VO-->Potentiometer

#define LCD_RS  GPIO_C,3
#define LCD_RW  GPIO_C,4
#define LCD_E   GPIO_C,5
#define LCD_D7  GPIO_C,7
#define LCD_D6  GPIO_C,8
#define LCD_D5  GPIO_C,9
#define LCD_D4  GPIO_C,10
#define LCD_D3  GPIO_C,11
#define LCD_D2  GPIO_C,12
#define LCD_D1  GPIO_C,13
#define LCD_D0  GPIO_C,14



#endif //LCD_CONFIG_H
