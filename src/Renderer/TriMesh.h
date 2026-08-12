#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexBufferLayout.h"
#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"
#include <vector>
#include "Renderer/Texture.h"
#include "Renderer/Material.h"
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 BiTangent;
};
using namespace std;
class TriMesh {
    public:
        // mesh data
        vector<Vertex>       vertices;
        vector<unsigned int> indices;

        TriMesh(vector<Vertex> vertices, vector<unsigned int> indices);
        TriMesh(vector<Vertex> vertices) : TriMesh(vertices, {}) {}
        void Draw();
    private:
        //  render data

        VertexArray VAO;
        VertexBuffer VBO;
        IndexBuffer EBO;
        VBL layout;
        void setupMesh();
}; 