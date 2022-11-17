#ifndef __ALGORITHM_H
#define __ALGORITHM_H
#define E 2.71828
#include "stdio.h"
#include "math.h"

double Discal(double, double, int);
double NormalDistribution(double, double, double);
void GussianBlurWeight(double*);

extern double Discal(double, double, int);
extern double NormalDistribution(double, double, double);
extern void GussianBlurWeight(double*);

#endif