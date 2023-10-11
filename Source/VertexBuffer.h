#pragma once

class VertexBuffer
{
private:
	unsigned int rendererID;

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	const void Bind();
	const void Unbind();
};