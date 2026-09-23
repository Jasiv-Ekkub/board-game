#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>

typedef struct Timer {
	float time;
	float length;
} Timer;

Timer Timer_Get();
bool Timer_HasEnded(Timer timer);
bool Timer_HasBeenSet(Timer timer);
void Timer_Set(Timer* timer, float length);
void Timer_Reset(Timer* timer);
void Timer_Update(Timer* timer);

#endif //TIMER_H
