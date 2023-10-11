#pragma once

class IndexBuffer
{
private:
	unsigned int rendererID;
	unsigned int count;

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	const void Bind();
	const void Unbind();

	inline unsigned int GetCount() const { return count; }
};