#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray{
private:
    unsigned int m_RenderID;
public:
    VertexArray();
    ~VertexArray();
    VertexArray(const VertexArray &other) = delete;
    void AddBuffer(const VertexBuffer& vb, const VBL& Layout);

    void Bind() const;
    void UnBind() const;
};