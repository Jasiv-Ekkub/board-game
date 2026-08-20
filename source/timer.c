#include <timer.h>
#include <game_context.h>

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
	return timer.currentTime <= 0;
}

void SetTimer(Timer* timer, float endTime)
{
	if(timer->flags & TIMER_INTERRUPTABLE || HasTimerEnded(*timer))
	{
		timer->currentTime = endTime;
		timer->endTime = endTime;
	}
}

void ResetTimer(Timer* timer)
{
	timer->currentTime = 0;
	timer->endTime = 0;
}

bool UpdateTimer(Timer* timer)
{
	if(HasTimerEnded(*timer) && (timer->flags & TIMER_SINGLE_PULSE)) return false;
	timer->currentTime -= GetDeltaTime();
	if(HasTimerEnded(*timer))
	{
		if(timer->flags & TIMER_REPEATING) timer->currentTime = timer->endTime;
		else timer->currentTime = 0;

		return true;
	}
	return false;
}
