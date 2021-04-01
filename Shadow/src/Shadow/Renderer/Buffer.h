#pragma once
#include "Shadow/Core.h"

NAMESPACE_BEGAN

class VertexBuffer
{
public:
	VertexBuffer() {};

	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
	virtual uint32_t GetBufferID() const = 0;

	static VertexBuffer* Create(float* vertices, uint32_t size);

private:

};

class IndexBuffer
{
public:
	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
	virtual unsigned int GetCount() const = 0;


	static IndexBuffer* Create(uint32_t* indices, uint32_t size);
};

NAMESPACE_END