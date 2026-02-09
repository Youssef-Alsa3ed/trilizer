#include "VertexBuffer.h"
#include "glad/glad.h"
#include <Renderer/GLDEBUGGING.hpp>
VertexBuffer::VertexBuffer()
{
    GLCALL(glGenBuffers(1, &m_RenderID));
}
VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
    GLCALL(glGenBuffers(1, &m_RenderID));
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

/*VertexBuffer::VertexBuffer(const VertexBuffer &other)
{
    m_RenderID = other.m_RenderID;
}*/

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) noexcept
{
    if (this != &other)
    {
        glDeleteBuffers(1, &m_RenderID); // Clean up existing
        m_RenderID = other.m_RenderID;   // Steal the ID
        other.m_RenderID = 0;
    }
    return *this;
}

void VertexBuffer::SetData(const void *data, const unsigned int size)
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCALL(glDeleteBuffers(1, &m_RenderID));
}

void VertexBuffer::Bind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
}
void VertexBuffer::UnBind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}