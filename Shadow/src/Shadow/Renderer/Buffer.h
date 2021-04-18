#pragma once
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "ShaderData.h"

NAMESPACE_BEGAN

struct Vertex
{
	glm::vec3 position = glm::vec3(0.0, 0.0,0.0);
	glm::vec3 normal = glm::vec3(0.0, 0.0,0.0);
	glm::vec2 texCoords = glm::vec2(0.0,0.0);
};



struct BufferElement
{
	std::string name;
	ShaderDataType type;
	uint32_t size;
	uint32_t offset;
	bool normalize;

	BufferElement(ShaderDataType type, const std::string& name, bool normalize = false):
				 name(name), type(type),size(ShaderData::ShaderDataTypeSize(type)),offset(0), normalize(normalize)
	{}

	uint32_t GetElementCount() const
	{
		return	ShaderData::GetElementCount(type);
	}
};
class BufferLayout
{
public:
	BufferLayout() {};
	BufferLayout(const std::initializer_list<BufferElement>& elements);
	inline const std::vector<BufferElement>& GetElements() const { return elements;  };
	inline const uint32_t GetStride() const { return stride; };

	// To iterate BufferLayout
	std::vector<BufferElement>::iterator begin() { return elements.begin(); }
	std::vector<BufferElement>::iterator end() { return elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return elements.end(); }

private:
	void CalculateOffsetAndStrinde();
private:
	std::vector<BufferElement> elements;
	uint32_t stride;
};

class VertexBuffer
{
public:
	VertexBuffer() {};

	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;
	virtual uint32_t GetBufferID() const = 0;

	static VertexBuffer* Create(float* vertices, uint32_t size);
	static VertexBuffer* Create(Vertex* vertices, uint32_t count);

	virtual const BufferLayout& GetLayout() const = 0;
	virtual void SetLayout(const BufferLayout& layout) = 0;

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