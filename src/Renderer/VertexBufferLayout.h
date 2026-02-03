#pragma once
#include <vector>
#include <string>
#include "glad/glad.h"

struct VertexBufferElement
{
    std::string name;
    unsigned int type;
    unsigned int count;
    unsigned char normalize;

    static unsigned int GetSizeOfType(unsigned int type){
        switch(type){
            case GL_FLOAT:         return sizeof(float);
            case GL_UNSIGNED_INT:  return sizeof(GLuint);
            case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        }

        return 0;
    }
};

class VBL{
private: 
    unsigned int m_stride;
    std::vector<VertexBufferElement> m_Elements;
public:
    VBL() : m_stride(0){

    }

    template<typename T>
    void Push(unsigned int count, std::string name){
        //nothing for now
    }

    inline const std::vector<VertexBufferElement> GetElements() const{ 
        return m_Elements;
    }
    inline unsigned int GetStride() const{ 
        return m_stride; 
    }
};

template<>
inline void VBL::Push<float>(unsigned int count, std::string name){
    m_Elements.push_back({name, GL_FLOAT, count, GL_FALSE});
    m_stride += sizeof(float) * count;
}

template<>
inline void VBL::Push<unsigned int>(unsigned int count, std::string name){
    m_Elements.push_back({name, GL_UNSIGNED_INT, count, GL_FALSE});
    m_stride += sizeof(GLuint) * count;
}

template<> 
inline void VBL::Push<int>(unsigned int count, std::string name){
    m_Elements.push_back({name, GL_INT, count, GL_FALSE});
    m_stride += sizeof(GLint) * count;
}
template<>
inline void VBL::Push<unsigned char>(unsigned int count, std::string name){
    m_Elements.push_back({name, GL_UNSIGNED_BYTE, count, GL_FALSE});
    m_stride += sizeof(GLubyte) * count;
}