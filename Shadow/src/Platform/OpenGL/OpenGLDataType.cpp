#include "swpch.h"
#include "OpenGLDataType.h"
#include <glad/glad.h>

NAMESPACE_BEGAN

GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch (type)
	{
	case Shadow::ShaderDataType::FLOAT:
	case Shadow::ShaderDataType::FLOAT2:
	case Shadow::ShaderDataType::FLOAT3:
	case Shadow::ShaderDataType::FLOAT4:
	case Shadow::ShaderDataType::MAT3:
	case Shadow::ShaderDataType::MAT4:
		return GL_FLOAT;
		break;

	case Shadow::ShaderDataType::INT:
	case Shadow::ShaderDataType::INT2:
	case Shadow::ShaderDataType::INT3:
	case Shadow::ShaderDataType::INT4:
		return GL_INT;
		break;
	case Shadow::ShaderDataType::BOOL:
		return GL_BOOL;
		break;

	case Shadow::ShaderDataType::NONE: default: return GL_NONE; 	break;
	}
}

ShaderDataType OpenGLBaseTypeToShaderDataType(GLenum type)
{
	int nfloat = GL_FLOAT;
	switch (type)
	{
		case GL_FLOAT:		return Shadow::ShaderDataType::FLOAT;	break;
		case GL_FLOAT_VEC2: return Shadow::ShaderDataType::FLOAT2;	break;
		case GL_FLOAT_VEC3: return Shadow::ShaderDataType::FLOAT3;	break;
		case GL_FLOAT_VEC4: return Shadow::ShaderDataType::FLOAT4;	break;
		case GL_FLOAT_MAT3: return Shadow::ShaderDataType::MAT3;	break;
		case GL_FLOAT_MAT4: return Shadow::ShaderDataType::MAT4;	break;
		case GL_INT:		return Shadow::ShaderDataType::INT;		break;
		case GL_INT_VEC2:	return Shadow::ShaderDataType::INT2;	break;
		case GL_INT_VEC3:	return Shadow::ShaderDataType::INT3;	break;
		case GL_INT_VEC4:	return Shadow::ShaderDataType::INT4;	break;
		case GL_BOOL:		return Shadow::ShaderDataType::BOOL;	break;
		default:			return Shadow::ShaderDataType::NONE;	break;
	}
}

NAMESPACE_END