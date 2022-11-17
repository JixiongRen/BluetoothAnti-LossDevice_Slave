#include "STC89XX.h"
#include "math.h"
#include "stdio.h"

#define uchar unsigned char
#define uint unsigned int

SBIT(SETBUTT, 0xB0, 2); // 进入设置时间模式，该引脚也是外部中断0触发引脚
SBIT(MOVBUTT, 0xA0, 0); // 移动位按键
SBIT(ADDBUTT, 0xA0, 1); // 时间加减按键

__xdata uchar num = 0;
__xdata uchar year = 22;
__xdata uchar mon = 1;
__xdata uchar day = 1;
__xdata uchar hour = 0;
__xdata uchar min = 0;
__xdata uchar sec = 0;

void initT0_INT0()
{
    TMOD = 0x00;    // 设定定时器0工作方式0
    TH0 = (8192-4607)/32; 
    TL0 = (8192-4607)%32; 
    EA = 1; // 开启总中断
    ET0 = 1; // 开启定时器0中断
    EX0 = 1; // 开启外部中断0中断 
    PX0 = 1; // 定义外部中断0是高优先级，即进入设置模式时，计时中止
    TR0 = 1; // 开始计时
}



void AutomaticTiming()
{

}

