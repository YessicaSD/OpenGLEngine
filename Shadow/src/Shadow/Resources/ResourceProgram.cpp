#include "swpch.h"
#include "ResourceProgram.h"
#include "glad/glad.h"
#include "Shadow/Log.h"
#include <glm/gtc/type_ptr.hpp>
#include "Shadow/Layers/LayerRenderer.h"
#include "Platform/OpenGL/OpenGLProgram.h"
#include <fstream>
NAMESPACE_BEGAN

Program::~Program()
{
	Delete();
}



Program* LoadProgram(const char* vsPath, const char* fsPath, const char* gsPath)
{
	switch (Renderer::GetRendererAPI())
	{
		case RendererAPI::RenderAPIType::NONE:
			SW_CORE_ASSERT(false, "not render api using");
			return nullptr;
			break;

		case RendererAPI::RenderAPIType::OPENGL:
		{
			std::string vs;
			std::string fs;

			std::ifstream vShaderFile;
			std::ifstream fShaderFile;
			vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

			try
			{
				// open files
				vShaderFile.open(vsPath);
				fShaderFile.open(fsPath);
				std::stringstream vShaderStream, fShaderStream;
				// read file's buffer contents into streams
				vShaderStream << vShaderFile.rdbuf();
				fShaderStream << fShaderFile.rdbuf();
				// close file handlers
				vShaderFile.close();
				fShaderFile.close();
				// convert stream into string
				vs = vShaderStream.str();
				fs = fShaderStream.str();
			}
			catch (std::ifstream::failure& e)
			{
				SW_LOG_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
			}
			return new OpenGLProgram(vs, fs);
		}
		break;

		default:
			return nullptr;
			break;
	}

	return nullptr;
}

Program* LoadProgram(const char* path)
{
	switch (Renderer::GetRendererAPI())
	{
	case RendererAPI::RenderAPIType::NONE:
		SW_CORE_ASSERT(false, "not render api using");
		return nullptr;
		break;

	case RendererAPI::RenderAPIType::OPENGL:
	{
		std::string vs = "#version 430\n";
		std::string fs = "#version 430\n";

		std::ifstream programFile;
		programFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			programFile.open(path);
			std::stringstream programStream;
			// read file's buffer contents into streams
			programStream << programFile.rdbuf();
			// close file handlers
			programFile.close();
			// convert stream into string
			vs +=  "#define VERTEX_SHADER \n" + programStream.str();
			fs +=  "#define FRAGMENT_SHADER \n" + programStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			SW_LOG_ERROR("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: ensure the path is correct. The given path is " + std::string(path));
		}

		SW_LOG_INFO("Loading program with path" + std::string(path));
		return new OpenGLProgram(vs, fs);
	}
		
		break;

	default:
		return nullptr;
		break;
	}
}

void Program::LogAttributes()
{
	SW_LOG_TRACE("{0}", GetProgramID());
	for each (auto attribute in attributes)
	{
		SW_LOG_TRACE("{0}", attribute.name);
	}
}

NAMESPACE_END