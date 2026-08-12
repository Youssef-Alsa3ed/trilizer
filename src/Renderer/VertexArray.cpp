#include "glad/glad.h"
#include "VertexArray.h"
#include "GLDEBUGGING.hpp"
#include <vector>
VertexArray::VertexArray()
{
    GLCALL(glGenVertexArrays(1, &m_RenderID));
    GLCALL(glBindVertexArray(m_RenderID));
}

VertexArray::~VertexArray()
{
    GLCALL(glDeleteVertexArrays(1, &m_RenderID));
}

/*VertexArray::VertexArray(const VertexArray &other)
{
    m_RenderID = other.m_RenderID;
}*/

void VertexArray::AddBuffer(const VertexBuffer &vb, const VBL &layout)
{
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for(int i = 0; i < elements.size(); i++){
        const auto element = elements[i];
        GLCALL(glEnableVertexAttribArray(i));

        GLCALL(glVertexAttribPointer(i, element.count, 
        element.type, element.normalize, 
        layout.GetStride(), 
        (const void*) offset));

        offset += VertexBufferElement::GetSizeOfType(element.type) * element.count;
    }
}

void VertexArray::Bind() const
{
    GLCALL(glBindVertexArray(m_RenderID));
}

void VertexArray::UnBind() const
{
    GLCALL(glBindVertexArray(0));
}