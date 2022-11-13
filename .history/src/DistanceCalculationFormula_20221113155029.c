#include "DistanceCalculationFormula.h"
#include "stdio.h"

/**
 * @brief 
 * 
 * @param txPower 
 * @param rssi 
 * @param flag 
 * @return float 
 */
float Discal(float txPower, float rssi, int flag)
{
    if (flag == 0)
    {
        // 多经验值算法
        return (0.89976) * pow(rssi / txPower, 7.7095) + 0.111;
    } else {
        // 少经验值算法
        return pow(10, abs(rssi - txPower) / (10 * 4));
    }
}