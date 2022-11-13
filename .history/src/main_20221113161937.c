#include "STC89XX.h"
#include "compiler.h"
#include "stdio.h"
#include "DistanceCalculationFormula.h"

void main()
{
    float txPower = 1.0000;
    float RSSI = 5.12487;
    int flag = 1;
    float dis = Discal(txPower, RSSI, flag);
}