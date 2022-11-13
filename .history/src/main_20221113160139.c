#include "STC89XX.h"
#include "compiler.h"
#include "stdio.h"
#include "DistanceCalculationFormula.h"

void main()
{
    float txPower = 1.0000;
    float RSSI = 5.12487;
    float dis = Discal(txPower, RSSI, 1);
    printf("dis is %f m", dis, 0);
}