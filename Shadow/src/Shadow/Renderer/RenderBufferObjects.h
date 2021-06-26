#pragma once

class RBO
{
public:
	~RBO() = default;
	virtual void Bind() = 0;
	virtual void DefineDepthStorageSize(int size) = 0;
	virtual void DefineDepthStorageSize(int w, int h) = 0;
	virtual void BindDepthToFrameBuffer()  = 0;
};