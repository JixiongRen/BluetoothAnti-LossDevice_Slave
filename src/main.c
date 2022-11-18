#include "STC89XX.h"
#include "compiler.h"
#include "stdio.h"
#include "math.h"
#include "delay.h"
#include "algorithm.h"
#include "lcd_driver.h"
#include "clock.h"


/***************************宏定义区****************************/
#define uchar unsigned char
#define uint unsigned int 

/***************************主函数****************************/
void main()
{   while(1){
    P13 = 1;
}
    
    TimeSysMain();
}