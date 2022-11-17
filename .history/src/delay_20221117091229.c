/***************************低精度延时子函数****************************/
#include "delay.h"
void delay (uint xms){
  uint i;
  uint j;
  for(i = xms; i>0; i--){
    for (j = 110;j>0;j--);
  }
}