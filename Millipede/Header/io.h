// Functions used to illuminate the LCD Screen. LCD is used to display score as well as winning and losing screens. 
#ifndef __io_h__
#define __io_h__

void LCD_init();
void LCD_ClearScreen(void);
void Display_Points(uint8_t hi, uint8_t current);
void NewHighScore();
void LCD_WriteCommand (unsigned char Command);
void LCD_WriteData(unsigned char Data);
void LCD_Cursor (unsigned char column);
void LCD_DisplayString(unsigned char column ,const unsigned char *string);
void Print_Dont_Clear(unsigned char column ,const unsigned char *string);
void delay_ms(int miliSec);
#endif

