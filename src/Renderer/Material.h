#pragma once
#include "glm/glm.hpp"
#include "texture.h"
#include <vector>
#include "Shader.h"

using namespace std;
using namespace glm;
class TriMat{
    private:
    vec3 diffuse;
    float specular;
    vec3 ambient;
    float shininess;
    vector<std::shared_ptr<Texture>> textures;
    Shader* shader;
    public:
    TriMat(vec3 diffuse, float specular, vec3 ambient, float shininess, vector<std::shared_ptr<Texture>> textures);
    TriMat(vec3 diffuse, float specular, vec3 ambient, float shininess) : TriMat(diffuse, specular, ambient, shininess, {}) {}
    TriMat(vec3 diffuse, float specular, vec3 ambient) : TriMat(diffuse, specular, ambient, 32.0f, {}) {}
    TriMat(vec3 diffuse, float specular) : TriMat(diffuse, specular, vec3(1.0f), 32.0f, {}) {}
    TriMat(vec3 diffuse) : TriMat(diffuse, 1.0f, vec3(1.0f), 32.0f, {}) {}
    TriMat() : TriMat(vec3(1.0f), 1.0f, vec3(1.0f), 32.0f, {}) {}

    ~TriMat();
    void SetShader(Shader* shader);
    void Apply();
};