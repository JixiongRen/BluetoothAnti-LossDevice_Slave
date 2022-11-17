#ifndef __LCD_DRIVER_H
#define __LCD_DRIVER_H
#include "STC89XX.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"
#define LCD_DataPort P0
#define _nop_() __asm NOP __endasm

void LCD_Delay();
void LCD_WriteCommand(unsigned char);
void LCD_WriteData(unsigned char);
void LCD_SetCursor(unsigned char,unsigned char);
void LCD_Init();
void LCD_ShowString(unsigned char,unsigned char,char *);
void LCD_ShowNum(unsigned char,unsigned char,unsigned int,unsigned char);

extern void LCD_Delay();
extern void LCD_WriteCommand(unsigned char);
extern void LCD_WriteData(unsigned char);
extern void LCD_SetCursor(unsigned char,unsigned char);
extern void LCD_Init();
extern void LCD_ShowString(unsigned char,unsigned char,char *);
extern void LCD_ShowNum(unsigned char,unsigned char,unsigned int,unsigned char);

#endif