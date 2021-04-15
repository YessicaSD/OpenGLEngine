#pragma once

#include "Resource.h"

#include "glm/glm.hpp"

NAMESPACE_BEGAN

class Program : public Resource
{
public:
	Program(std::string& vs, std::string& fs);
	~Program();

	unsigned int CreateShader(std::string& vs, std::string& fs);
	inline unsigned int GetProgramID() { return programID; };
	void Bind();
	void UnBind();
	void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
	void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

private:
	unsigned int programID = 0;
};

NAMESPACE_END