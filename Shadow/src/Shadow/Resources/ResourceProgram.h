#pragma once

#include "Resource.h"

#include "glm/glm.hpp"
#include "Shadow/Renderer/ShaderData.h"

NAMESPACE_BEGAN
struct VertexShaderAttribute
{
	unsigned int location;
	ShaderDataType type;
	std::string name;
};
class Program : public Resource
{
public:
	Program() {};
	virtual ~Program();

	virtual inline unsigned int GetProgramID() { return 0; };
	virtual void Bind() {};
	virtual void UnBind() {};
	virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& values) {};
	virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {};

protected:
	virtual void Delete() {};

protected:
	std::vector<VertexShaderAttribute> attributes;
};

Program* CreateShader(std::string& vs, std::string& fs);

NAMESPACE_END