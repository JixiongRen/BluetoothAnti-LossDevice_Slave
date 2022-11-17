#include "STC89XX.h"
#include "compiler.h"
#include "stdio.h"
#include "math.h"
#include "delay.h"
#include "algorithm.h"
#include "lcd_driver.h"


/***************************宏定义区****************************/
#define uchar unsigned char
#define uint unsigned int 


/***************************变量定义区****************************/
SBIT(SETBUTT, 0xB0, 2); // 进入设置时间模式，该引脚也是外部中断0触发引脚
SBIT(MOVBUTT, 0xB0, 0); // 移动位按键
SBIT(ADDBUTT, 0xB0, 1); // 时间加减按键

// __xdata uint CursorPosition = 1;
// __xdata uchar HL = 0;
// __xdata uchar HH = 0;
// __xdata uchar mL = 0;
// __xdata uint mH = 0;
// __xdata uchar SL = 0;
// __xdata uchar SH = 0;
// __xdata uint* PositionPointer;
// __xdata uchar num = 0; // 计时器工作轮次



// /***************************电子时钟子函数****************************/

// /**
//  * 初始化定时器T0，用于实现时间记录的功能
// */
// void initT0_INT0()
// {
//     TMOD = 0x00;    // 设定定时器0工作方式0
//     TH0 = (8192-4607)/32; 
//     TL0 = (8192-4607)%32; 
//     EA = 1; // 开启总中断
//     ET0 = 1; // 开启定时器0中断
//     EX0 = 1; // 开启外部中断0中断 
//     PX0 = 1; // 定义外部中断0是高优先级，即进入设置模式时，计时中止
//     TR0 = 1; // 开始计时
// }



// /**
//  * @brief 实现自动计时的函数，即钟表走时，理论上在主函数调用本函数就可以实现时间管理
//  */
// void AutomaticTiming()
// {
//     // 计时器连续计时200次为1s
//     if (num == 200 ){
//         num = 0;
//         SL ++;
//         if (SL = 10){
//             SL = 0;
//             SH ++;
//             if (SH = 6){
//                 SH = 0;
//                 mL ++;
//                 if (mL = 10){
//                     mL = 0;
//                     mH ++;
//                     if (mH = 6){
//                         mH = 0;
//                         HL ++;
//                         // 检查小时低位是否为4。若是4则检查高位是否是2，
//                         // 若是清零，否则继续等到低位为10,给高位加1。
//                         // 判4在判10之前，不可能出现类似28h这样的时间
//                         if(HL = 4){
//                             if(HH = 2){
//                                 HH = 0;
//                                 HL = 0;
//                             }
//                         } else if(HL = 10){
//                             HL = 0;
//                             HH ++;
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }



// /**
//  * @brief 此函数的作用在于当用户在设置模式下按下移动光标位置键，
//  * 根据用户按动的次数匹配存放不同日期信息（年月日）的数据的地址
//  */
// void SelectPosition(){
//     // 检测是否为抖动
//     if (SETBUTT == 0 && MOVBUTT == 1){
//         delay(8);
//         if (SETBUTT == 0 && MOVBUTT == 1){
//             CursorPosition ++;  // 光标位置加1，即光标从右向左移动1位，由于日期格式为HH-MM-SS，光标最小是1，最大是6
//             if(CursorPosition>6){
//                 // 光标超过6就重新计算
//                 CursorPosition = 1;
//             }
//         }
//     }
//     switch (CursorPosition)
//     {
//         // 根据不同的光标位置选择要操作的位
//         case '1': PositionPointer = &SL; break;
//         case '2': PositionPointer = &SH; break;   
//         case '3': PositionPointer = &mL; break;    
//         case '4': PositionPointer = &mH; break;
//         case '5': PositionPointer = &HL; break;
//         case '6': PositionPointer = &HH; break;
//     }
// }

// /**
//  * @brief 时钟指针加1子函数
//  */
// void AdjustTheValue()
// {
//     if (SETBUTT == 0 && ADDBUTT == 1){
//         // 判断是否为抖动
//         delay(8);
//         if (SETBUTT == 0 && ADDBUTT == 1)
//         {
//             *PositionPointer ++; 
//             if((PositionPointer == &SL || PositionPointer == &mL) && *PositionPointer == 10){
//                 *PositionPointer = 0;   // 秒、分的低位十进制
//             }
//             if((PositionPointer == &SH || PositionPointer == &mH) && *PositionPointer == 6){
//                 *PositionPointer = 0;   // 秒、分的高位六进制
//             }
//             if(PositionPointer == &HL && *PositionPointer == 10){
//                 *PositionPointer = 0;   // 默认设置小时的低位为十进制
//             }
//             if(PositionPointer == &HH){
//                 if ((*PositionPointer == 2 && HL > 3) || (*PositionPointer > 2)){
//                     // 判断小时高位是否大于等于2
//                     HL = 0;HH = 0; // 小时高低位清零
//                 }
//             }
//         }
//     }
// }

// /**
//  * @brief 设置键被按下（触发外部中断0）则进入外部中断0中断服务程序
//  * 调用选位、调值函数进行对时操作
//  * 检测到设置键弹起则退出服务程序
//  */
// void INT0_itrpt() __interrupt(0)
// {
//     while(1){
//         SelectPosition();
//         AdjustTheValue();
//         if (SETBUTT == 1){
//             delay(10);
//             if (SETBUTT == 1){
//                 break;
//             }
//         }
//     }
// }

// /**
//  * @brief 定时器T0的中断服务函数，用于增加num以及为TH0，TL0装填初值
//  */
// void T0_itrpt() __interrupt(1) // __interrupt(1) 是定时器T0的中断号，定时器T0中断后程序会自动进入中断号为1的中断服务程序
// {
//     TH0 = (8192-4607)/32; 
//     TL0 = (8192-4607)%32;
//     num ++;
// }


/***************************主函数****************************/
void main()
{   
//     // while(1){
//     //     LcdMain();
//     //     delay(100000);
//     // }
//     // __xdata uchar code Time[] = 
//     __xdata double txPower = 1.0000; 
//     __xdata double RSSI = 5.12487;
//     __xdata int flag = 1;
//     double dis = Discal(txPower, RSSI, flag);
//     __xdata double DistanceArry[10];
//     for(int i=0;i<10;i++){
//         DistanceArry[i] = i;
//     }
//     GussianBlurWeight(DistanceArry);
}