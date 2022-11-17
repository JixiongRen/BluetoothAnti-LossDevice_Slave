#ifndef __CLOCK_H
#define __CLOCK_H

void initT0(void);
void T0_itrpt(void);
void AutomaticTiming(void);
void ClearTime(void);
void TimeSysMain(void);

extern void initT0(void);
extern void T0_itrpt(void);
extern void AutomaticTiming(void);
extern void ClearTime(void);
extern void TimeSysMain(void);

#endif