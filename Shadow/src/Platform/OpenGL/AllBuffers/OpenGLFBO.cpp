#include "swpch.h"
#include "OpenGLFBO.h"
#include "glad/glad.h"

NAMESPACE_BEGAN

OpenGLFBO::OpenGLFBO()
{
	glGenFramebuffers(1, &handle);
}
OpenGLFBO::~OpenGLFBO()
{
	glDeleteFramebuffers(1, &handle);
}

void OpenGLFBO::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

NAMESPACE_END


