#include "STC89XX.h"
#include "compiler.h"
#include "stdio.h"
#include "DisRelated.h"
#include "math.h"

/***************************引入其他文件的函数****************************/
extern double Discal(double, double, int);

/***************************主函数****************************/
void main()
{
    struct dis_arr
    {
        double DisList[10];
    };
    double txPower = 1.0000; 
    double RSSI = 5.12487;
    int flag = 1;
    double dis = Discal(txPower, RSSI, flag);
}

#include "STC89XX.h"
#include "compiler.h"
#include "stdio.h"
//#include "DisRelated.h"
#include "math.h"

struct dis_arr{
    double DisList[10];
};
/***************************引入其他文件的函数****************************/
//extern double Discal(double, double, int);




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
    if (flag == 0)
    {
        // 多经验值算法
        dis = (0.89976) * powf((rssi / txPower), 7.7095) + 0.111;
    } else {
        // 少经验值算法
        dis = powf(10, fabsf(rssi - txPower) / (10 * 4));
    }
    return dis;
}

struct dis_arr GussianBlur(struct dis_arr DisBeforeGussian)
{
    struct dis_arr DisAfterGussian;
    for(int i=0; i<sizeof (DisBeforeGussian.DisList);i++){
        DisAfterGussian.DisList[i] = DisBeforeGussian.DisList[i];
    }
    return DisAfterGussian;
}


/***************************主函数****************************/
void main()
{
    struct DA
    {
        double DisList[10];
    };
    double txPower = 1.0000; 
    double RSSI = 5.12487;
    int flag = 1;
    double dis = Discal(txPower, RSSI, flag);
}

