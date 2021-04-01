#pragma once

#include "Resource.h"

NAMESPACE_BEGAN

class Program : public Resource
{
public:
	Program(std::string& vs, std::string& fs);
	~Program();

	unsigned int CreateShader(std::string& vs, std::string& fs);

	void Bind();
	void UnBind();
private:
	unsigned int programID = 0;
};

NAMESPACE_END