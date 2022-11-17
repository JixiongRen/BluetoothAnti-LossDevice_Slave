#ifndef __CLOCK_H
#define __CLOCK_H

void initT0();
void T0_itrpt() __interrupt(1);
void AutomaticTiming();

#endif