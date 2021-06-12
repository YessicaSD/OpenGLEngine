#include "swpch.h"
#include "OpenGLRBO.h"
#include "glad/glad.h"

NAMESPACE_BEGAN

OpenGLRBO::OpenGLRBO()
{
	glGenRenderbuffers(1, &handle);
}


void OpenGLRBO::BindDepthToFrameBuffer()
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, handle);
}

void OpenGLRBO::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, handle);
}

void OpenGLRBO::DefineDepthStorageSize(int size)
{
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, size, size);
}

NAMESPACE_END

