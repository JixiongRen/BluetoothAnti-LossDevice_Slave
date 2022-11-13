#ifdef _DISRELATED_H
#define _DISRELATED_H
#include "math.h"
#include "stdio.h"
    typedef struct dis_arr{
        double DisList[10];
    } dis_arr;

    double Discal(double, double, int);
    dis_arr GaussianBlur(dis_arr);
#endif