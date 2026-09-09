#include <helpers.h>
#include <stdlib.h>
#include <stdio.h>

float GetRandomFloat(float min, float max)
{
	float random = ((float)rand()) / RAND_MAX * (max - min) + min;
	printf("%f <= %f <= %f\n", min, random, max);
	return random;
}
