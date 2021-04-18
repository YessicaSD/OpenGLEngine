#include "swpch.h"
#include "ShaderData.h"

NAMESPACE_BEGAN

uint32_t ShaderData::ShaderDataTypeSize(ShaderDataType type)
{
	switch (type)
	{
		case Shadow::ShaderDataType::FLOAT:			return 4;			break;
		case Shadow::ShaderDataType::FLOAT2:		return 4 * 2;		break;
		case Shadow::ShaderDataType::FLOAT3:		return 4 * 3;		break;
		case Shadow::ShaderDataType::FLOAT4:		return 4 * 4;		break;
		case Shadow::ShaderDataType::MAT3:			return 4 * 3 * 3; 	break;
		case Shadow::ShaderDataType::MAT4:			return 4 * 4 * 4;	break;
		case Shadow::ShaderDataType::INT:			return 4;			break;
		case Shadow::ShaderDataType::INT2:			return 4 * 2;		break;
		case Shadow::ShaderDataType::INT3:			return 4 * 3;		break;
		case Shadow::ShaderDataType::INT4:			return 4 * 4;		break;
		case Shadow::ShaderDataType::BOOL:			return 1;			break;
		case Shadow::ShaderDataType::NONE: defaul:	return 0;			break;
	}
}

uint32_t ShaderData::GetElementCount(ShaderDataType type)
{
	switch (type)
	{
		case Shadow::ShaderDataType::FLOAT:			return 1;		break;
		case Shadow::ShaderDataType::FLOAT2:			return 2;		break;
		case Shadow::ShaderDataType::FLOAT3:			return 3;		break;
		case Shadow::ShaderDataType::FLOAT4:			return 4;		break;
		case Shadow::ShaderDataType::MAT3:			return 3 * 3; 	break;
		case Shadow::ShaderDataType::MAT4:			return 4 * 4;	break;
		case Shadow::ShaderDataType::INT:				return 1;		break;
		case Shadow::ShaderDataType::INT2:			return 2;		break;
		case Shadow::ShaderDataType::INT3:			return 3;		break;
		case Shadow::ShaderDataType::INT4:			return 4;		break;
		case Shadow::ShaderDataType::BOOL:			return 1;		break;
		case Shadow::ShaderDataType::NONE: defaul:	return 0;		break;
	}
}


NAMESPACE_END