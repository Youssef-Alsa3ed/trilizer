#include "Material.h"
#include <stdexcept>
#include "Logger/Logger.h"


TriMat::TriMat(float shininess, vector<std::shared_ptr<Texture>> textures) 
: shininess(shininess), textures(std::move(textures)) {}

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
    shader->SetFloat("material.shininess", shininess);

    for (size_t i = 0; i < textures.size(); i++) {
        textures[i]->Bind(i);
        shader->SetInt("material.tex" + std::to_string(i + 1), static_cast<int>(i));
    }
}

TriMat::~TriMat()
{

}