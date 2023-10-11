#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
{
    IndexBuffer::count = count;

    glGenBuffers(1, &rendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (count * sizeof(unsigned int)), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &rendererID);
}

const void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
}

const void IndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}