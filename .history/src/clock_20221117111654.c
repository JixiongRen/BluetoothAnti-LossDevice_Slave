#include "STC89XX.h"
#include "math.h"
#include "stdio.h"
#include "string.h"
#include "lcd_driver.h"
#include "delay.h"

#define uchar unsigned char
#define uint unsigned int

SBIT(CLRBUTT, 0xA0, 2); // 按下时说明失而复得，时间清零重新计算。

__xdata uchar num = 0;
__xdata uchar year = 0;
__xdata uchar mon = 0;
__xdata uchar day = 0;
__xdata uchar hour = 0;
__xdata uchar min = 0;
__xdata uchar sec = 0;

void initT0()
{
    TMOD = 0x00;    // 设定定时器0工作方式0
    TH0 = (8192-4607)/32; 
    TL0 = (8192-4607)%32; 
    EA = 1; // 开启总中断
    ET0 = 1; // 开启定时器0中断
    // while(/**蓝牙断开*/)      //TODO 与硬件部分联调，蓝牙断开时启动计时器
    TR0 = 1; // 开始计时
}

/**
 * @brief 定时器T0的中断服务函数，用于增加num以及为TH0，TL0装填初值
 */
void T0_itrpt() __interrupt(1) // __interrupt(1) 是定时器T0的中断号，定时器T0中断后程序会自动进入中断号为1的中断服务程序
{
    TH0 = (8192-4607)/32; 
    TL0 = (8192-4607)%32;
    num ++;
}

void AutomaticTiming()
{
    if (num == 200){
        sec ++;
        if (sec >= 60){
            sec = 0;
            min ++;
            if (min >= 60){
                min = 0;
                hour ++;
                if (hour >= 24){
                    hour = 0;
                    day ++;
                    if (mon == 4 || mon == 6 || mon == 9 || mon == 11) {
                        while(day < 31){
                            day = 1;
                            mon ++;
                        }
                    } else if (mon == 2){
                        if(((year%4==0)&&(year%100!=0))||(year%400==0)){
                            while (day < 30){
                                day = 1;
                                mon ++;
                            }
                        } else {
                            while (day < 29){
                                day = 1;
                                mon ++;
                            }
                        }
                    } else {
                        if (day > 31){
                            day = 1;
                            mon ++;
                        }
                    }  
                } if (mon > 12){
                    mon = 0;
                    year ++;
                }
            }
        }
    }
}

void ClearTime(){
    while (CLRBUTT == 0)
    {
        delay (20);
        while (CLRBUTT == 0);
        delay (20);
        
    }
    
}

