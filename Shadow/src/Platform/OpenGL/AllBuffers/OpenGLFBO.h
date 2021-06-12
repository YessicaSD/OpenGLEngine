#pragma once
#include "Shadow/Renderer/FrameBufferObject.h"

NAMESPACE_BEGAN

class OpenGLFBO : public FBO
{
public:
	OpenGLFBO();
	~OpenGLFBO();
	virtual void Bind() override;
private:
	unsigned int handle;
};

NAMESPACE_END