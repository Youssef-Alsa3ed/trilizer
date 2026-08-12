#pragma once
#include "glm/glm.hpp"
#include "Texture.h"
#include <vector>
#include "Shader.h"

using namespace std;
using namespace glm;
class TriMat{
    private:
    float shininess = 64.0f;
    vector<std::shared_ptr<Texture>> textures;
    Shader* shader;
    public:
    TriMat(float shininess, vector<std::shared_ptr<Texture>> textures);
    TriMat(float shininess) : TriMat(shininess, {}) {}
    TriMat() : TriMat(64.0f, {}) {}

    ~TriMat();
    void SetShader(Shader* shader);
    void Apply();
};