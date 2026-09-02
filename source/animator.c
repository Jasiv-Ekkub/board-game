#include <animator.h>

#define MAX_ANIMATION_COUNT 16

Animation animations[MAX_ANIMATION_COUNT] = {0};

void QueueAnimation(Animation animation)
{
	for(int i=0; i<MAX_ANIMATION_COUNT; ++i)
	{
		if(HasAnimationEnded(animations[i]))
		{
			animations[i] = animation;
			break;
		}
	}
}

void UpdateAnimator()
{
	for(int i=0; i<MAX_ANIMATION_COUNT; ++i)
	{
		Animate(&animations[i]);
	}
}
