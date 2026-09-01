#ifndef ANIMATION_H
#define ANIMATION_H

#include <raylib.h>
#include <stdbool.h>

#define MAX_KEYFRAME_COUNT 16

typedef struct Keyframe
{
	float timestamp;
	Vector3 position;
} Keyframe;

typedef struct Animation
{
	int keyframeCount;
	Keyframe keyframes[MAX_KEYFRAME_COUNT];
	float time;
	Vector3 *object;
} Animation;

Animation GetAnimation(Vector3 *object);
void AddKeyframe(Animation* animation, float timestamp, Vector3 position);
void Animate(Animation *animation);
bool HasAnimationEnded(Animation animation);

#endif //ANIMATION_H
