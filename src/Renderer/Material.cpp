#include "Material.h"
#include <stdexcept>
#include "Logger/Logger.h"


TriMat::TriMat(vec3 diffuse, float specular, vec3 ambient, float shininess, vector<Texture> textures) 
: diffuse(diffuse), specular(specular), ambient(ambient), shininess(shininess), textures(std::move(textures)) {}

void TriMat::SetShader(Shader *shader)
{
    this->shader = shader; 
}

void TriMat::Apply()
{
    if(shader == nullptr){
        ERRLOG("Shader not set for material.");
        return;
    }

    shader->Use();
    shader->SetVec3("material.diffuse", diffuse);
    shader->SetFloat("material.specular", specular);
    shader->SetVec3("material.ambient", ambient);
    shader->SetFloat("material.shininess", shininess);

    for (size_t i = 0; i < textures.size(); i++) {
        textures[i].Bind(i);
        shader->SetInt("material.tex" + std::to_string(i + 1), static_cast<int>(i));
    }
}