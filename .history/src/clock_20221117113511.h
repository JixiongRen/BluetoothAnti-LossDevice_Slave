#ifndef __CLOCK_H
#define __CLOCK_H

void initT0();
void T0_itrpt() __interrupt(1);
void AutomaticTiming();
void ClearTime();
void TimeSysMain();

extern initT0();
extern T0_itrpt() __interrupt(1);
extern void AutomaticTiming();
extern void ClearTime();
extern void TimeSysMain();

#endif