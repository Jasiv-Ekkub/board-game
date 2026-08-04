#ifndef TIMER_H
#define TIMER_H

#include <game_context.h>
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
void SetTimer(Timer* timer, float endTime);
bool UpdateTimer(Timer* timer, GameContext gameContext);

#endif //TIMER_H
