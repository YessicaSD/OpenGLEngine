#include "swpch.h"
#include "OpenGLProgram.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "Shadow/Log.h"
#include "Platform/OpenGL/OpenGLDataType.h"


NAMESPACE_BEGAN
OpenGLProgram::OpenGLProgram(std::string& vertexSource, std::string& fragmentSource)
{
	// Vertex shader === 
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* source = (const GLchar*)vertexSource.c_str();

	glShaderSource(vertexShader, 1, &source, 0);
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(vertexShader);

		SW_LOG_ERROR("There was an error with the vertex shader, Error Info: {0}", infoLog.data());
		return;
	}

	// Fragment shader === 
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	source = (const GLchar*)fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);

	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);

		SW_LOG_ERROR("There was an error with the fragment shader, Error Info: {0}", infoLog.data());

		return;
	}

	GLuint program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(program);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		SW_LOG_ERROR("There was an error with the program, Error Info: {0}", infoLog.data());

		return;
	}

	// Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	programID = program;

	LoadAttributes();

	
}

void OpenGLProgram::LoadAttributes()
{
	GLint attributeCount = 0;
	glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &attributeCount);

	for (auto i = 0; i < attributeCount; i++)
	{
		ProgramAttribute attribute;

		char name[255];
		GLsizei attributeLength, attributeSize;
		GLenum attributeType;

		glGetActiveAttrib(programID, i,
			ARRAY_COUNT(name),
			&attributeLength,
			&attributeSize,
			&attributeType,
			name);

		attribute.location = i;
		attribute.name = name;
		attribute.type = OpenGLBaseTypeToShaderDataType(attributeType);
		attributes.push_back(attribute);
	}
	LogAttributes();
}

void OpenGLProgram::Bind()
{
	glUseProgram(programID);
}

void OpenGLProgram::UnBind()
{
	glUseProgram(0);
}

void OpenGLProgram::UploadUniformFloat(const std::string& name, const float& value)
{
	GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform1f(location, value);
}

void OpenGLProgram::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
{
	GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform4f(location,values.x, values.y, values.z, values.w);
}

void OpenGLProgram::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
{
	GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform3f(location, values.x, values.y, values.z);
}

void OpenGLProgram::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
	GLint location = glGetUniformLocation(programID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLProgram::UploadUniformInt(const std::string& name, const int& value)
{
	GLint location = glGetUniformLocation(programID, name.c_str());
	glUniform1i(location, value);
	
}

void OpenGLProgram::Delete()
{
	glDeleteProgram(programID);
}


NAMESPACE_END

