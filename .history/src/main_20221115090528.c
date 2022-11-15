#include "STC89XX.h"
#include "compiler.h"
#include "stdio.h"
#include "math.h"

/***************************宏定义区****************************/
#define E 2.71828
#define uchar unsigned char
#define uint unsigned int 
// #define __xdata long

/***************************变量定义区****************************/

SBIT(SETBUTT, 0x80, 0); // 进入设置时间模式
SBIT(MOVBUTT, 0x80, 1); // 移动位按键
SBIT(ADDBUTT, 0x80, 2); // 时间加减按键

__xdata uint CursorPosition = 1;
__xdata uint ML = 1;
__xdata uint MH = 0;
__xdata uint DL = 1;
__xdata uint DH = 0;
__xdata uint HL = 0;
__xdata uint HH = 0;
__xdata uint mL = 0;
__xdata uint mH = 0;
__xdata uint SL = 0;
__xdata uint SH = 0;
uint* PositionPointer;

/***************************算法类子函数****************************/
/**
 * @brief 使用两种公式对距离进行计算，其中经验值直接参考教程中给出，
 * 实测时可进行调整
 * @param txPower 距离信标1m时的RSSI
 * @param rssi 信号强度值，可以通过读取RSSI寄存器得出
 * @param flag 标志。当 flag == 0，使用多经验值得计算式，当 flag
 * == 1，使用少经验值的计算式
 * @return float 计算得到的距离
 */
double Discal(double txPower, double rssi, int flag)
{
    double dis;
    if (flag == 0){
        // 多经验值算法
        dis = (0.89976) * powf((rssi / txPower), 7.7095) + 0.111;
    } else {
        // 少经验值算法
        dis = powf(10, fabsf(rssi - txPower) / (10 * 4));
    }
    return dis;
}

/**
 * @brief 计算高斯模糊权重的函数
 * @param x 需要计算权重的变量
 * @param mu 该组数据的样本均值
 * @param sigma 该组数据的样本标准差
 * @return double 高斯模糊权重
 */
double NormalDistribution(double x, double mu, double sigma)
{
    return 1/(sigma * sqrtf(2*PI)) * powf(E, (-1) * powf((x - mu), 2)/(2 * powf(sigma, 2)));
}

/**
 * @brief 对一个测量列中的结果进行高斯模糊处理
 * @param ArryAddr 需要处理的测量列的首地址
 */
void GussianBlurWeight(double* ArryAddr){
    // 89C52的片内RAM仅有256B，为节约空间，变量一律置于片外RAM(64KB)，即__xdata
    __xdata char i = 0;
    __xdata double DisArry[10]; 
    __xdata double Weight[10];
    __xdata double sumWeight = 0;
    __xdata double sum = 0;
    __xdata double Pow = 0;
    for (i=0;i<10;i++){
        DisArry[i] = *(ArryAddr + i);
        sum = sum + DisArry[i];
    }
    double mu = sum / 10;
    for (i=0;i<10;i++){
        Pow = Pow + powf(DisArry[i] - mu, 2);
    }
    double sigma = sqrtf(Pow/(10 - 1));
    for (i=0;i<10;i++){
        // 计算每个结果分配的高斯权重
        Weight[i] = NormalDistribution(ArryAddr[i], mu, sigma);
        sumWeight = sumWeight + Weight[i];
    }
    for (i=0;i<10;i++){
        // 权重归一化
        Weight[i] = Weight[i] / sumWeight;
        ArryAddr[i] = ArryAddr[i] * Weight[i];
    }
}

/***************************电子时钟子函数****************************/

void delay (uchar xms){
  uchar i;
  uchar j;
  for(i = xms; i>0; i--){
    for (j = 110;j>0;j--);
  }
}

/**
 * 初始化定时器T0，用于实现时间记录的功能
*/
void initT0()
{
    TMOD = 0x00;    // 设定定时器0工作方式0
    TH0 = (8192-4607)/32; 
    TL0 = (8192-4607)%32; 
    EA = 1; // 开启总中断
    ET0 = 1; // 开启定时器0中断
    TR0 = 1; // 开始计时
}

/**
 * @brief 此函数的作用在于当用户在设置模式下按下移动光标位置键，
 * 根据用户按动的次数匹配存放不同日期信息（年月日）的数据的地址
 */
void SelectPosition(){
    // 检测是否为抖动
    if (SETBUTT == 1 && MOVBUTT == 1){
        delay(8);
        if (SETBUTT == 1 && MOVBUTT == 1){
            CursorPosition ++;
            if(CursorPosition>6){
                CursorPosition = 1;
            }
        }
    }
    switch (CursorPosition)
    {
        case '1': PositionPointer = &SL; break;
        case '2': PositionPointer = &SH; break;   
        case '3': PositionPointer = &mL; break;    
        case '4': PositionPointer = &mH; break;
        case '5': PositionPointer = &HL; break;
        case '6': PositionPointer = &HH; break;
    }
}

/**
 * @brief 时钟指针加1子函数
 */
void AdjustTheValue()
{
    if (SETBUTT == 1 && ADDBUTT == 1){
        // 判断是否为抖动
        delay(8);
        if (SETBUTT == 1 && ADDBUTT == 1)
        {
            *PositionPointer ++; 
            if(PositionPointer == &SL || PositionPointer == &mL){
                while(*PositionPointer == 10){
                    *PositionPointer = 0;   // 秒、分的低位十进制
                }
            }
            if(PositionPointer == &SH || PositionPointer == &mH || *PositionPointer == 6){
                while){
                    *PositionPointer = 0;   // 秒、分的高位六进制
                }
            }
            if(PositionPointer == &HL && *PositionPointer == 10){
                    *PositionPointer = 0;   // 默认设置小时的低位为十进制
            }
            if(PositionPointer == &HH){
                if ((*PositionPointer == 2 && HL > 3) || (*PositionPointer > 2)){
                    // 判断小时高位是否大于等于2
                    HL = 0;HH = 0; // 小时高低位清零
                }
            }
        }
    }
}


/***************************主函数****************************/
void main()
{
    __xdata double txPower = 1.0000; 
    __xdata double RSSI = 5.12487;
    __xdata int flag = 1;
    __xdata double dis = Discal(txPower, RSSI, flag);
    __xdata double DistanceArry[10];
    for(int i=0;i<10;i++){
        DistanceArry[i] = i;
    }
    GussianBlurWeight(DistanceArry);
}

