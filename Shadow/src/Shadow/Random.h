#pragma once
#include <random>
#include "pcg_random.hpp"

NAMESPACE_BEGAN

typedef unsigned long long UID;

class Random
{
	Random();

	static UID RandomUID();
	static Random* instance;
private:
	pcg32 rng;
};

NAMESPACE_END
