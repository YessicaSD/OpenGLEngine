#pragma once

class RBO
{
public:
	virtual void Bind() = 0;
	virtual void DefineDepthStorageSize(int size) = 0;
	virtual void BindDepthToFrameBuffer()  = 0;
};