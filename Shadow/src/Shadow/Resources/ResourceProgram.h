#pragma once

#include "Resource.h"

#include "glm/glm.hpp"
#include "Shadow/Renderer/ShaderData.h"

NAMESPACE_BEGAN
struct ProgramAttribute
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
	virtual void UploadUniformFloat(const std::string& name, const float& value) {};
	virtual void UploadUniformFloat3(const std::string& name, const glm::vec3& values) {};
	virtual void UploadUniformFloat3(const std::string& name, std::vector<glm::vec3>& values) {};
	virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& values) {};
	virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) {};
	virtual void UploadUniformInt(const std::string& name, const int& value) {};
			void LogAttributes();
protected:
	virtual void Delete() {};

protected:
	std::vector<ProgramAttribute> attributes;
};

Program* CreateShader(std::string& vs, std::string& fs);
Program* LoadProgram(const char* vsPath, const char* fsPath, const char* gsPath = nullptr);
Program* LoadProgram(const char* path);
NAMESPACE_END