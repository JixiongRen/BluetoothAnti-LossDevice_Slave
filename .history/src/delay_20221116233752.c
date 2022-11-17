#include "delay.h"
void delay (unsigned char xms){
  unsigned int i;
  unsigned int j;
  for(i = xms; i>0; i--){
    for (j = 110;j>0;j--);
  }
}