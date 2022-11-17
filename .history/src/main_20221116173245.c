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
SBIT(SETBUTT, 0xB0, 2); // 进入设置时间模式，该引脚也是外部中断0触发引脚
SBIT(MOVBUTT, 0xB0, 0); // 移动位按键
SBIT(ADDBUTT, 0xB0, 1); // 时间加减按键
SBIT(lcden, 0xB0, 4); // 液晶使能端
SBIT(lcdrs, 0xB0, 5); // 液晶数据命令选择端
SBIT(dula, 0xA0, 6); // 声明U1锁存器的锁存端
SBIT(wela, 0xA0, 7); // 声明U2锁存器的锁存端

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
__xdata uint* PositionPointer;
__xdata uchar num = 0; // 计时器工作轮次

uchar __code time[] = "222";

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



/**
 * @brief 实现自动计时的函数，即钟表走时，理论上在主函数调用本函数就可以实现时间管理
 */
void AutomaticTiming()
{
    // 计时器连续计时200次为1s
    if (num == 200 ){
        num = 0;
        SL ++;
        if (SL = 10){
            SL = 0;
            SH ++;
            if (SH = 6){
                SH = 0;
                mL ++;
                if (mL = 10){
                    mL = 0;
                    mH ++;
                    if (mH = 6){
                        mH = 0;
                        HL ++;
                        // 检查小时低位是否为4。若是4则检查高位是否是2，
                        // 若是清零，否则继续等到低位为10,给高位加1。
                        // 判4在判10之前，不可能出现类似28h这样的时间
                        if(HL = 4){
                            if(HH = 2){
                                HH = 0;
                                HL = 0;
                            }
                        } else if(HL = 10){
                            HL = 0;
                            HH ++;
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief 此函数的作用在于当用户在设置模式下按下移动光标位置键，
 * 根据用户按动的次数匹配存放不同日期信息（年月日）的数据的地址
 */
void SelectPosition(){
    // 检测是否为抖动
    if (SETBUTT == 0 && MOVBUTT == 1){
        delay(8);
        if (SETBUTT == 0 && MOVBUTT == 1){
            CursorPosition ++;  // 光标位置加1，即光标从右向左移动1位，由于日期格式为HH-MM-SS，光标最小是1，最大是6
            if(CursorPosition>6){
                // 光标超过6就重新计算
                CursorPosition = 1;
            }
        }
    }
    switch (CursorPosition)
    {
        // 根据不同的光标位置选择要操作的位
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
    if (SETBUTT == 0 && ADDBUTT == 1){
        // 判断是否为抖动
        delay(8);
        if (SETBUTT == 0 && ADDBUTT == 1)
        {
            *PositionPointer ++; 
            if((PositionPointer == &SL || PositionPointer == &mL) && *PositionPointer == 10){
                *PositionPointer = 0;   // 秒、分的低位十进制
            }
            if((PositionPointer == &SH || PositionPointer == &mH) && *PositionPointer == 6){
                *PositionPointer = 0;   // 秒、分的高位六进制
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

/**
 * @brief 设置键被按下（触发外部中断0）则进入外部中断0中断服务程序
 * 调用选位、调值函数进行对时操作
 * 检测到设置键弹起则退出服务程序
 */
void INT0_itrpt() __interrupt(0)
{
    while(1){
        SelectPosition();
        AdjustTheValue();
        if (SETBUTT == 1){
            delay(10);
            if (SETBUTT == 1){
                break;
            }
        }
    }
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

/***************************LCD屏幕显示程序****************************/


void LcdWriteCom(uchar com)
{
    lcdrs = 0;
    P0 = com;
    delay(5);
    lcden = 1;
    delay(5);
    lcden = 0;
}

void LcdWriteData(uchar data)
{
    lcdrs = 1;
    P0 = data;
    delay(5);
    lcden = 1;
    delay(5);
    lcden = 0;
}

void initLcd(){
    dula = 0;
    wela = 0;
    lcden = 0;
    LcdWriteCom(0x38);
    LcdWriteCom(0x0c);
    LcdWriteCom(0x06);
    LcdWriteCom(0x01);
}

void LcdMain(){
    initLcd();
    LcdWriteCom(0x80);
    LcdWriteData(time[1]);
}
/***************************主函数****************************/
void main()
{   
    while(1){
        LcdMain();
        delay(100000);
    }
    // __xdata uchar code Time[] = 
    // __xdata double txPower = 1.0000; 
    // __xdata double RSSI = 5.12487;
    // __xdata int flag = 1;
    // __xdata double dis = Discal(txPower, RSSI, flag);
    // __xdata double DistanceArry[10];
    // for(int i=0;i<10;i++){
    //     DistanceArry[i] = i;
    // }
    // GussianBlurWeight(DistanceArry);
}