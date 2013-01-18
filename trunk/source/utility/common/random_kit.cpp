#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "random_kit.h"

static double pow_10[] = {1,10,100,1000,10000,100000,1000000};

int rand_x(int min,int max)
{
	assert(min >= 0 && max >= 0 && max > min);

	int r_val = rand();
	int delta = max - min;

	return min + (r_val > delta ? r_val %= delta : r_val);
}

void random_kit::init(unsigned long long seed)
{
	srand((unsigned int)seed);
}

int random_kit::rand_i(int from,int to)
{
	int max = from >= to ? from : to;
	int min = from < to ? from : to;

	if(max == min)
	{
		return min;
	}
	else if(min >= 0)
	{
		return rand_x(min,max);
	}
	else 
	{
		return rand_x(0,max - min) + min;
	}
}

float random_kit::rand_f(float from,float to,int accu)
{
	float max = from >= to ? from : to;
	float min = from < to ? from : to;
	
	if(accu < 0) accu = 0;
	else if(accu > 6) accu = 6;

	float l_p = (float)pow_10[accu];
	
	int min_i = (int)(min*l_p);
	int max_i = (int)(max*l_p);

	int r_val = rand_i(min_i,max_i);

	return r_val/l_p;
}

bool random_kit::within_prob(float prob,int accu)
{
	if(prob >= 1.0f)
		return true;
	else if(prob <= 0.0f)
		return false;
	else
	{
		float r_val = rand_f(0.0f,1.0f,accu);
		return prob > r_val; 
	}
}