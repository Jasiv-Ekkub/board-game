#include <helpers.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

float GetRandomFloat(float min, float max)
{
	return ((float)rand()) / RAND_MAX * (max - min) + min;
}

float Sigmoid(float x)
{
	return 1 / (1 + exp(-x));
}
