#include "swpch.h"
#include "ResourceProgram.h"
#include "glad/glad.h"
#include "Shadow/Log.h"
#include <glm/gtc/type_ptr.hpp>

NAMESPACE_BEGAN

Program::Program(std::string& vertexSource, std::string& fragmentSource)
{
	programID = CreateShader(vertexSource, fragmentSource);
}

Program::~Program()
{
	glDeleteProgram(programID);
}

unsigned int Program::CreateShader(std::string& vertexSource, std::string& fragmentSource)
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

		SW_ERROR("There was an error with the vertex shader, Error Info: {0}", infoLog.data());
		return 0;
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

		SW_ERROR("There was an error with the fragment shader, Error Info: {0}", infoLog.data());
		
		return 0;
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

		SW_ERROR("There was an error with the program, Error Info: {0}", infoLog.data());
	
		return 0;
	}

	// Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	return program;
}

void Program::Bind()
{
	glUseProgram(programID);
}

void Program::UnBind()
{
	glUseProgram(0);
}

void Program::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
	GLint location = glGetUniformLocation(programID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

NAMESPACE_END