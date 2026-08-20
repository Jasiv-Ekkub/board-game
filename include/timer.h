#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include <stdint.h>

#define TIMER_INTERRUPTABLE 1
#define TIMER_REPEATING 2
#define TIMER_SINGLE_PULSE 4

typedef struct Timer {
	float currentTime;
	float endTime;
	uint8_t flags;
} Timer;

Timer GetTimer(uint8_t flags);
bool HasTimerEnded(Timer timer);
void SetTimer(Timer* timer, float endTime);
void ResetTimer(Timer* timer);
bool UpdateTimer(Timer* timer);

#endif //TIMER_H
