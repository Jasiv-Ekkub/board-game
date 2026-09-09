#include <timer.h>
#include <game_context.h>

Timer Timer_Get() { return (Timer){0}; }

bool Timer_HasEnded(Timer timer)
{
	return timer.time <= 0;
}

void Timer_Reset(Timer* timer)
{
	timer->time = timer->length;
}

void Timer_Set(Timer* timer, float length)
{
	timer->length = length;
	Timer_Reset(timer);
}

void Timer_Update(Timer* timer)
{
	if(!Timer_HasEnded(*timer))
		timer->time -= GetDeltaTime();
	else
		timer->time = 0;
}
