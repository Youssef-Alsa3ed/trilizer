#include "Light.h"

void LightManager::Submit(const Light &light)
{
    lights.push_back(light);
}

void LightManager::Flush()
{
    lights.clear();
}
using namespace glm;
Light CreateSpotLight()
{
    Light light;
    light.position = vec3(0.0f);
    light.direction = vec3(0.0f, 0.0f, 1.0f);
    light.ambient = vec3(0.1f, 0.1f, 0.1f);
    light.diffuse = vec3(1.0f, 1.0f, 1.0f);
    light.specular = vec3(1.0f, 1.0f, 1.0f);
    light.constant = 1.0f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;
    light.cutOff = cos(radians(12.5f));
    light.outerCutOff = cos(radians(17.5f));
    light.type = 1;
    return light;
}

Light CreateDirectionLight()
{
    Light light;
    light.direction = glm::vec3(0.2f, -1.0f, 0.3f);
    light.ambient   = glm::vec3(0.05f);
    light.diffuse   = glm::vec3(1.0f);
    light.specular  = glm::vec3(0.5f);
    light.type      = 0;
    return light;
}

Light CreatePointLight()
{
    Light light;
    light.position = vec3(0.0f);
    light.diffuse = vec3(1.0f);
    light.specular = vec3(1.0f);
    light.ambient = vec3(0.05f, 0.05f, 0.05f);
    light.constant = 1.0f;
    light.linear = 0.15f;
    light.quadratic = 0.08f;
    light.type = 2;
    return light;
}

void LightManager::Upload(const Shader &shader)
{
    for (int i = 0; i < lights.size(); i++)
    {
        std::string number = std::to_string(i);
        shader.SetVec3("lights[" + number + "].position", lights[i].position);
        shader.SetVec3("lights[" + number + "].direction", lights[i].direction);
        shader.SetVec3("lights[" + number + "].ambient", lights[i].ambient);
        shader.SetVec3("lights[" + number + "].diffuse", lights[i].diffuse);
        shader.SetVec3("lights[" + number + "].specular", lights[i].specular);
        shader.SetFloat("lights[" + number + "].constant", lights[i].constant);
        shader.SetFloat("lights[" + number + "].linear", lights[i].linear);
        shader.SetFloat("lights[" + number + "].quadratic", lights[i].quadratic);
        shader.SetFloat("lights[" + number + "].cutOff", lights[i].cutOff);
        shader.SetFloat("lights[" + number + "].outerCutOff", lights[i].outerCutOff);
        shader.SetInt("lights[" + number + "].type", lights[i].type);
    }
    shader.SetInt("lightCount", lights.size());
}
