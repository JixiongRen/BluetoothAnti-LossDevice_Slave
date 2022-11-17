#ifndef __CLOCK_H
#define __CLOCK_H

void initT0(void);
void T0_itrpt(void) __interrupt(1);
void AutomaticTiming(void);
void ClearTime(void);
void TimeSysMain(void);

extern void initT0(void);
extern void T0_itrpt(void) __interrupt(1);
extern void AutomaticTiming(void);
extern void ClearTime(void);
extern void TimeSysMain(void);

#endif