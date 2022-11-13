#include "STC89XX.h"
#include "compiler.h"
#include "stdio.h"
#include "DistanceCalculationFormula.h"
#include "math.h"


void main()
{
    double txPower = 1.0000;
    double RSSI = 5.12487;
    int flag = 1;
    double dis = Discal(txPower, RSSI, flag);
}