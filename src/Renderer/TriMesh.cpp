#include "TriMesh.h"

TriMesh::TriMesh(vector<Vertex> vertices, vector<unsigned int> indices)
: vertices(std::move(vertices)), indices(std::move(indices))
{

    setupMesh();
}

void TriMesh::Draw()
{
    VAO.Bind();
    if (!indices.empty())
    {
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<unsigned int>(vertices.size()));
    }
}

void TriMesh::setupMesh()
{
    VAO.Bind();
    VBO.Bind();
    VBO.SetData(vertices.data(), vertices.size() * sizeof(Vertex));
    if (!indices.empty())
    {
        EBO.Bind();
        EBO.SetData(indices.data(), indices.size());
    }

    layout.Push<float>(3, "aPos");
    layout.Push<float>(3, "aNormal");
    layout.Push<float>(2, "aTexCoord");
    layout.Push<float>(3, "aTangents");
    layout.Push<float>(3, "aBiTangents");

    VAO.AddBuffer(VBO, layout);
}
