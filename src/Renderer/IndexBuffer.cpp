#include "IndexBuffer.h"
#include "glad/glad.h"
#include <Renderer/GLDEBUGGING.hpp>

IndexBuffer::IndexBuffer()
{
	GLCALL(glGenBuffers(1, &m_RenderID));
}

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count) : m_Count(count)
{
	GLCALL(glGenBuffers(1, &m_RenderID));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCALL(glDeleteBuffers(1, &m_RenderID));
}

/*IndexBuffer::IndexBuffer(const IndexBuffer &other)
{
	m_RenderID = other.m_RenderID;
}*/

IndexBuffer &IndexBuffer::operator=(IndexBuffer &&other) noexcept
{
	if (this != &other)
	{
		glDeleteBuffers(1, &m_RenderID); // Delete our own current buffer

		m_RenderID = other.m_RenderID; // Steal the ID from other
		m_Count = other.m_Count;

		other.m_RenderID = 0; // Make sure other won't delete it
		other.m_Count = 0;
	}
	return *this;
}

void IndexBuffer::SetData(const unsigned int *data, unsigned int count)
{
	m_Count = count;
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

void IndexBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
}

void IndexBuffer::Unbind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}