#pragma once
#include "../Resources/ResourceCubemap.h"

NAMESPACE_BEGAN

class Environment 
{
public:
	void SetSkybox(Cubemap* skybox);

	std::shared_ptr<Cubemap> GetSkybox() { return skybox;  }
	std::shared_ptr<Cubemap> GetIrradiance() { return irradiance;  }

private:
	std::shared_ptr<Cubemap> skybox;
	std::shared_ptr<Cubemap> irradiance;

};

NAMESPACE_END