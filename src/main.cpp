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

bool firstMouse = true;
bool hideCursor = false;
int lastX = 400, lastY = 300;
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::mat4 projection;
unique_ptr<Shader> lightingshader;
unique_ptr<Shader> singlecolorshader;
unique_ptr<Shader> lightSourceShader;

unique_ptr<Model> cube;

Texture tex;
Texture tex2;

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
    lightingshader = std::make_unique<Shader>("../../Assets/Shaders/lightshader.glsl");
    lightSourceShader = std::make_unique<Shader>("../../Assets/Shaders/lightsource.glsl");
    singlecolorshader = std::make_unique<Shader>("../../Assets/Shaders/outlineshader.glsl");

    cube = std::make_unique<Model>("../../Assets/Models/cube.obj");
    tex.LoadAsync("../../Assets/Textures/container2.png");
    tex2.LoadAsync("../../Assets/Textures/container2_specular.png");
    Light light = CreateDirectionLight();
    LightManager::Get().Submit(light);
    Light point = CreatePointLight();
    point.position = vec3(point.position + vec3(2.0f, 0.0f, 0.0f));
    point.diffuse = vec3(0.6f, 0.33f, 0.2f) * 3.0f;
    LightManager::Get().Submit(point);
}

void AppUpdate(float deltaTime)
{

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF); // enable writing to the stencil buffer
    mat4 projection = glm::perspective(glm::radians(45.0f), Application::GetInstance()->GetWindow().GetAspectRatio(), 0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);

    tex.TryUploadToGPU();
    tex.Bind(0);
    tex.TryUploadToGPU();
    tex.Bind(1);
    lightingshader->Use();
    lightingshader->SetInt("material.tex1", 0);
    lightingshader->SetInt("material.tex2", 1);
    lightingshader->SetMat4("view", camera.GetViewMatrix());
    lightingshader->SetMat4("projection", projection);

    lightingshader->SetMat4("model", model);
    LightManager::Get().Upload(*lightingshader);
    cube->Draw(*lightingshader);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00); // disable writing to the stencil buffer
    glDisable(GL_DEPTH_TEST);
    singlecolorshader->Use();
    model = scale(model, vec3(1.05f));
    singlecolorshader->SetMat4("view", camera.GetViewMatrix());
    singlecolorshader->SetMat4("projection", projection);
    singlecolorshader->SetMat4("model", model);
    cube->Draw(*singlecolorshader);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_DEPTH_TEST);

    MoveCamera(deltaTime);
}

void AppClose()
{
    lightingshader.reset();
    lightSourceShader.reset();
    cube.reset();
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
