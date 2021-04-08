#pragma once
#include "Shadow/Renderer/Buffer.h"

NAMESPACE_BEGAN
class VertexArray
{
public:
	VertexArray() {};
	~VertexArray() {};

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
	virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

	virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
	virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;
	virtual uint32_t GetIndexCount()  = 0;

	static VertexArray* Create();
private:

};
NAMESPACE_END