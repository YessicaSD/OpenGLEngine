#include "swpch.h"
#include "Random.h"
#include "Log.h"
NAMESPACE_BEGAN

Random* Random::instance = new Random();
Random::Random()
{
	SW_LOG_TRACE("Initializing random seed source from Module Random");
	// Seed with a real random value, if available
	pcg_extras::seed_seq_from<std::random_device> seed_source;

	// Make a random number engine
	//rng = new pcg32 (seed_source);
	rng.seed(seed_source);
}
UID Random::RandomUID()
{
	//INFO: We don't use 0 because we consider it INVALID_RESOURCE_UID and INVALID_GAMEOBJECT_UID
	std::uniform_int_distribution<UID> uniform_dist(1, ULLONG_MAX);
	return uniform_dist(instance->rng);
}

NAMESPACE_END
