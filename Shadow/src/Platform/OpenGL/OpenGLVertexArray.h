#pragma once
#include "Shadow/Renderer/VertexArray.h"

NAMESPACE_BEGAN

class OpenGLVertexArray : public VertexArray
{
public:
	OpenGLVertexArray();
	~OpenGLVertexArray();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) override;
	virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) override;

	virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override;
	virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override;
	virtual uint32_t GetIndexCount() override;

private:
	std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
	std::shared_ptr<IndexBuffer> indexBuffer;
	uint32_t vertexArrayID = 0;
};
NAMESPACE_END