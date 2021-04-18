#pragma once
#include "Shadow/Resources/ResourceProgram.h"

NAMESPACE_BEGAN

class OpenGLProgram : public Program
{
public:
	OpenGLProgram(std::string& vs, std::string& fs);
	
	virtual void Bind() override;
	virtual void UnBind() override;
	virtual void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
	virtual void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	virtual inline unsigned int GetProgramID() { return programID; };

private:
	virtual void Delete() override;
	void LoadAttributes();
private:
	unsigned int programID = 0;
};

NAMESPACE_END