#include <helpers.h>
#include <stdlib.h>
#include <stdio.h>

float GetRandomFloat(float min, float max)
{
	return ((float)rand()) / RAND_MAX * (max - min) + min;
}
