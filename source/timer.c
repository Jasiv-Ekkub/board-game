#include <timer.h>

Timer GetTimer(uint8_t flags)
{
	return (Timer){
		.currentTime = 0,
		.endTime = 0,
		.flags = flags,
	};
}

bool HasTimerEnded(Timer timer)
{
	return timer.currentTime >= timer.endTime;
}

void SetTimer(Timer* timer, float endTime)
{
	if(timer->flags & TIMER_INTERRUPTABLE || HasTimerEnded(*timer))
	{
		timer->currentTime = 0;
		timer->endTime = endTime;
	}
}

bool UpdateTimer(Timer* timer, GameContext gameContext)
{
	if(HasTimerEnded(*timer) && (timer->flags & TIMER_SINGLE_PULSE)) return false;
	timer->currentTime += gameContext.deltaTime;
	if(HasTimerEnded(*timer))
	{
		if(timer->flags & TIMER_REPEATING) timer->currentTime = 0;
		else timer->currentTime = timer->endTime;

		return true;
	}
	return false;
}
