#pragma once
#include "Shadow/Renderer/ShaderData.h"
typedef unsigned int GLenum;

NAMESPACE_BEGAN

GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type);
ShaderDataType OpenGLBaseTypeToShaderDataType(GLenum type);

NAMESPACE_END