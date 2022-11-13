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
