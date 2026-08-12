#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
    unsigned int m_RenderID;

public:
    VertexArray();
    ~VertexArray();
    // Delete Copying
    VertexArray(const VertexArray &) = delete;
    VertexArray &operator=(const VertexArray &) = delete;

    // ADD MOVING
    VertexArray(VertexArray &&other) noexcept : m_RenderID(other.m_RenderID)
    {
        other.m_RenderID = 0; // Take ownership, reset original
    }

    VertexArray &operator=(VertexArray &&other) noexcept
    {
        if (this != &other)
        {
            glDeleteVertexArrays(1, &m_RenderID); // Clean up existing
            m_RenderID = other.m_RenderID;      // Steal the ID
            other.m_RenderID = 0;
        }
        return *this;
    }
    void AddBuffer(const VertexBuffer &vb, const VBL &Layout);

    void Bind() const;
    void UnBind() const;
};