#include "delay.h"
void delay (uchar xms){
  uchar i;
  uchar j;
  for(i = xms; i>0; i--){
    for (j = 110;j>0;j--);
  }
}