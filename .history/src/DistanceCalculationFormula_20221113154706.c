#include "DistanceCalculationFormula.h"
#include "stdio.h"

float Discal(float txPower, float rssi, bool flag)
{
    if (flag == 0)
    {
        // 多经验值算法
        return (0.89976) * Math.pow(rssi / txPower, 7.7095) + 0.111;
    } else {

    }
}