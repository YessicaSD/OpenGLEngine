#pragma once
#include "Resource.h"
#include "ResourceProgram.h"

NAMESPACE_BEGAN

class Material : public Resource
{
public:
	Material();
	Material(std::string vs, std::string fs);
	void UseMaterial();
	std::shared_ptr<Program> GetProgram() { return program; };

private:
	std::shared_ptr<Program> program;
	std::string vs;
	std::string fs;
};

NAMESPACE_END