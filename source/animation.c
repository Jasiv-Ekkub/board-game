#include <animation.h>
#include <stdio.h>
#include <game_context.h>

Animation GetAnimation(Vector3 *object)
{
	return (Animation){
		.keyframeCount = 1,
		.keyframes = {
			{
				.timestamp = 0,
				.position = *object
			}
		},
		.object = object,
		.time = 0
	};
}

void AddKeyframe(Animation* animation, float timestamp, Vector3 position)
{
	animation->keyframes[animation->keyframeCount++] = (Keyframe){
			.timestamp = timestamp,
			.position = position
		};
}

void Animate(Animation *animation)
{
	if(HasAnimationEnded(*animation)) return;
	animation->time += GetDeltaTime();
	for(int i=1; i<animation->keyframeCount; ++i)
	{
		Keyframe start = animation->keyframes[i-1];
		Keyframe end = animation->keyframes[i];
		if(start.timestamp > animation->time || end.timestamp <= animation->time) continue;
		
		float factor = (animation->time - start.timestamp) / (end.timestamp - start.timestamp);
		animation->object->x = (end.position.x - start.position.x) * factor + start.position.x;
		animation->object->y = (end.position.y - start.position.y) * factor + start.position.y;
		animation->object->z = (end.position.z - start.position.z) * factor + start.position.z;
		return;
	}
	animation->object->x = animation->keyframes[animation->keyframeCount-1].position.x;
	animation->object->y = animation->keyframes[animation->keyframeCount-1].position.y;
	animation->object->z = animation->keyframes[animation->keyframeCount-1].position.z;
	animation->time = -1;
}

bool HasAnimationEnded(Animation animation)
{
	return animation.time < 0;
}
