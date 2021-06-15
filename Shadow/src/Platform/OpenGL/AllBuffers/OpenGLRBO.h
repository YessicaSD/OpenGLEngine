#pragma once
#include "Shadow/Renderer/RenderBufferObjects.h"

NAMESPACE_BEGAN

class OpenGLRBO : public RBO
{
public:
	OpenGLRBO();
	~OpenGLRBO();
	/// <summary>
	/// To use this function the frame buffer needs to be bind before
	/// </summary>
	virtual void Bind() override;
	virtual void DefineDepthStorageSize(int size) override;
	virtual void BindDepthToFrameBuffer() override;
private:
	unsigned int handle;
};
NAMESPACE_END