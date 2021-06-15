#pragma once


#include "swpch.h"

NAMESPACE_BEGAN

class Resource
{
public:
	std::string name;
	~Resource() = default;

private:
	unsigned int uid = 0;
};

NAMESPACE_END