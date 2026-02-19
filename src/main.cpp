#include "enginepch.h"
#include "Core/Application.h"
#include "Core/Input.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Core/Camera.h"
#include "Renderer/Model.h"
#include "Renderer/Light.h"

using namespace glm;

vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)};

glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f, 0.2f, 2.0f),    // key light (front-right)
    glm::vec3(2.3f, -3.3f, -4.0f),  // low side accent
    glm::vec3(-4.0f, 2.0f, -12.0f), // far background glow
    glm::vec3(0.0f, 0.0f, -3.0f),   // central fill
    glm::vec3(-2.5f, 1.5f, 1.0f),   // rim light (left)
};

glm::vec3 pointLightColors[] = {
    glm::vec3(1.0f, 0.82f, 0.65f), // warm key light
    glm::vec3(0.65f, 0.80f, 1.0f), // cool contrast light
    glm::vec3(1.0f, 0.95f, 0.80f), // neutral fill
    glm::vec3(0.7f, 0.9f, 1.0f),   // blue atmospheric
    glm::vec3(1.0f, 0.6f, 0.4f),   // warm rim highlight
};

bool firstMouse = true;
bool hideCursor = false;
int lastX = 400, lastY = 300;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::mat4 projection;
unique_ptr<Shader> lightingshader;
unique_ptr<Shader> lightSourceShader;

unique_ptr<Model> cube;

unique_ptr<Model> backpack;

void MoveCamera(float deltaTime)
{
    if (Input::Get().GetKey(GLFW_KEY_W))
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (Input::Get().GetKey(GLFW_KEY_S))
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (Input::Get().GetKey(GLFW_KEY_A))
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (Input::Get().GetKey(GLFW_KEY_D))
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void AppInit()
{
    lightingshader = std::make_unique<Shader>("../../Assets/Shaders/depthshader.glsl");
    lightSourceShader = std::make_unique<Shader>("../../Assets/Shaders/lightsource.glsl");
    cube = std::make_unique<Model>("../../Assets/Models/cube.obj");
    backpack = std::make_unique<Model>("../../Assets/Models/backpack/backpack.obj", true);
    
    Light dir = CreateDirectionLight();
    LightManager::Get().lights.push_back(dir);
    for (int i = 1; i < 5; i++)
    {
        Light light = CreatePointLight();
        light.position = pointLightPositions[i];
        light.diffuse = pointLightColors[i];
        LightManager::Get().lights.push_back(light);
    }
    Light spot = CreateSpotLight();
    LightManager::Get().lights.push_back(spot);
}

void AppUpdate(float deltaTime)
{
    float frequency = 0.5f;
    for (int i = 0; i < 5; i++)
    {
        vec3 initalPos = pointLightPositions[i];
        // update light position over time
        vec3 newPos = vec3(
            initalPos.x + sin(glfwGetTime() * frequency + i) * 2.0f,
            initalPos.y + cos(glfwGetTime() * frequency + i) * 1.0f,
            initalPos.z + sin(glfwGetTime() * frequency + i) * 2.0f);
        vec3 lightPos = newPos;
        mat4 lightmodel = mat4(1.0f);
        lightmodel = translate(lightmodel, lightPos);
        lightmodel = scale(lightmodel, vec3(0.2f));
        float aspectRatio = Application::GetInstance()->GetWindow().GetAspectRatio();
        projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        // draw the lamp object
        lightSourceShader->Use();
        lightSourceShader->SetMat4("model", lightmodel);
        lightSourceShader->SetMat4("view", camera.GetViewMatrix());
        lightSourceShader->SetMat4("projection", projection);
        lightSourceShader->SetVec3("lightColor", pointLightColors[i]);

        cube->Draw(*lightSourceShader);
    }

    for (int i = 0; i < 5; i++)
    {
        vec3 initalPos = pointLightPositions[i];
        // update light position over time
        vec3 newPos = vec3(
            initalPos.x + sin(glfwGetTime() * frequency + i) * 2.0f,
            initalPos.y + cos(glfwGetTime() * frequency + i) * 1.0f,
            initalPos.z + sin(glfwGetTime() * frequency + i) * 2.0f);

        LightManager::Get().lights[i].position = newPos;
    }

    lightingshader->Use();
    lightingshader->SetVec3("viewPos", camera.Position);
    Light& spot = LightManager::Get().lights[5];
    spot.position = camera.Position;
    spot.direction = camera.Front;
    // lightingshader->SetMat4("model", model);
    lightingshader->SetFloat("material.shininess", 128.0f);
    lightingshader->SetMat4("view", camera.GetViewMatrix());
    lightingshader->SetMat4("projection", projection);
    for (unsigned int i = 0; i < 10; i++)
    {
        mat4 model = mat4(1.0f);
        model = translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle),
                            glm::vec3(1.0f, 0.3f, 0.5f));

        model = scale(model, vec3(0.4f));
        lightingshader->SetMat4("model", model);
        LightManager::Get().Upload(*lightingshader);
        backpack->Draw(*lightingshader);
    }
    MoveCamera(deltaTime);
}

void AppClose()
{
    lightingshader.reset();
    lightSourceShader.reset();
    cube.reset();
    backpack.reset();
}
bool ClientKeyCallBack(KeyPressEvent &e)
{
    return true;
}

bool ClientMouseMoveCallBack(MouseMoveEvent &e)
{
    if (!hideCursor)
    {
        // man i'm such a genius
        firstMouse = true;
        return true;
    }

    float xpos = e.GetX();
    float ypos = e.GetY();
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);

    return true;
}

bool ClientMouseButtonCallBack(MouseButtonEvent &e)
{
    if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT)
    {
        hideCursor = !hideCursor;
        if (hideCursor)
            Input::Get().DisableCursor();
        else
            Input::Get().EnableCursor();
    }

    return true;
}

int main()
{
    Application app;
    app.Run();

    return 0;
}
