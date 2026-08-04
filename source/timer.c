#include <timer.h>

Timer GetTimer(float endTime, uint8_t flags)
{
	return (Timer){
		.currentTime = 0,
		.endTime = endTime,
		.flags = flags,
	};
}

void ResetTimer(Timer* timer)
{
	if(timer->flags & TIMER_INTERRUPTABLE || timer->currentTime == timer->endTime)
	{
		timer->currentTime = 0;
	}
}

bool UpdateTimer(Timer* timer, GameContext gameContext)
{
	if((timer->currentTime == timer->endTime) && (timer->flags & TIMER_SINGLE_PULSE)) return false;
	timer->currentTime += gameContext.deltaTime;
	if(timer->currentTime >= timer->endTime)
	{
		if(timer->flags & TIMER_REPEATING) timer->currentTime = 0;
		else timer->currentTime = timer->endTime;

		return true;
	}
	return false;
}
