#include "STC89XX.h"
#include "math.h"
#include "stdio.h"

#define uchar unsigned char
#define uint unsigned int

SBIT(SETBUTT, 0xB0, 2); // 进入设置时间模式，该引脚也是外部中断0触发引脚
SBIT(MOVBUTT, 0xA0, 0); // 移动位按键
SBIT(ADDBUTT, 0xA0, 1); // 时间加减按键

