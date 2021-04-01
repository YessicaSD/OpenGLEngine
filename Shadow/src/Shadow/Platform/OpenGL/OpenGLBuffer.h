#pragma once

#include "Shadow/Renderer/Buffer.h"

#include "Shadow/Core.h"

NAMESPACE_BEGAN
class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(float* vertices, uint32_t size);
	~OpenGLVertexBuffer();

	virtual void Bind() const override;
	virtual void UnBind() const override;
	virtual uint32_t GetBufferID() const override;

private:
	uint32_t bufferID = 0;
};

class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
	~OpenGLIndexBuffer();

	virtual void Bind() const override;
	virtual void UnBind() const override;
	virtual unsigned int GetCount() const override;

private:
	uint32_t bufferID = 0;
	unsigned int count = 0;
};

NAMESPACE_END
