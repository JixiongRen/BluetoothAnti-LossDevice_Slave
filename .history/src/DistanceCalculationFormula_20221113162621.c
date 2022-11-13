// #include "DistanceCalculationFormula.h"
// #include "stdio.h"
// #include "math.h"

// /**
//  * @brief 使用两种公式对距离进行计算，其中经验值直接参考教程中给出，
//  * 实测时可进行调整
//  * @param txPower 距离信标1m时的RSSI
//  * @param rssi 信号强度值，可以通过读取RSSI寄存器得出
//  * @param flag 标志。当 flag == 0，使用多经验值得计算式，当 flag
//  * == 1，使用少经验值的计算式
//  * @return float 计算得到的距离
//  */
// double Discal(double txPower, double rssi, int flag)
// {
//     double dis;
//     if (flag == 0)
//     {
//         // 多经验值算法
//         dis = (0.89976) * powf((rssi / txPower), 7.7095) + 0.111;
//     } else {
//         // 少经验值算法
//         dis = powf(10, fabsf(rssi - txPower) / (10 * 4));
//     }
//     return dis;
// }