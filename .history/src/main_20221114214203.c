#include "STC89XX.h"
#include "compiler.h"
#include "stdio.h"
#include "math.h"
#define E 2.71828
// 下面一行宏定义是为了减少编辑器的红色波浪线，编译时注释掉
#define __xdata long

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

/***************************硬件类子函数****************************/


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

