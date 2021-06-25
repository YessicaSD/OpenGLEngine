#pragma once
#include "../Resources/ResourceCubemap.h"

NAMESPACE_BEGAN

class Environment 
{
public:
	~Environment();
	void SetSkybox(std::shared_ptr<Cubemap> skybox);

	std::shared_ptr<Cubemap> GetSkybox() { return skybox;  }
	std::shared_ptr<Cubemap> GetIrradiance() { return irradiance;  }
	std::shared_ptr<Cubemap> GetPrefilter() { return prefilter; }

private:
	std::shared_ptr<Cubemap> skybox;
	std::shared_ptr<Cubemap> irradiance;
	std::shared_ptr<Cubemap> prefilter;
};

NAMESPACE_END