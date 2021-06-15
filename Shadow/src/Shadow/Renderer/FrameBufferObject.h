#pragma once
#include "Shadow/Core.h"
NAMESPACE_BEGAN

class FBO
{
public:
	virtual ~FBO() = default;
	virtual void Bind() = 0;
};

NAMESPACE_END