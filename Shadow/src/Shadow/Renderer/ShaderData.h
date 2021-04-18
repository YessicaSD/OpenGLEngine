#pragma once

NAMESPACE_BEGAN

enum class ShaderDataType
{
	NONE = 0, FLOAT, FLOAT2, FLOAT3, FLOAT4, MAT3, MAT4, INT, INT2, INT3, INT4, BOOL
};

namespace ShaderData
{
	uint32_t ShaderDataTypeSize(ShaderDataType type);

	uint32_t GetElementCount(ShaderDataType type);
}

NAMESPACE_END