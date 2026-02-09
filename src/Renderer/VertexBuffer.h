#pragma once

class VertexBuffer{
private:
    unsigned int m_RenderID;
public:
    VertexBuffer();
    VertexBuffer(const void* data, const unsigned int size); 
    VertexBuffer(const VertexBuffer &other) = delete;

    // ADD MOVING
    VertexBuffer(VertexBuffer&& other) noexcept : m_RenderID(other.m_RenderID){
        other.m_RenderID = 0;
    }

    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    void SetData(const void* data, const unsigned int size);

    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;
};