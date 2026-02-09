#include "Application.h"
#include "enginepch.h"
#include "Logger/Logger.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexBufferLayout.h"
#include "Renderer/VertexArray.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Input.h"
#include "Core/Camera.h"
#include "Renderer/TriMesh.h"
#include "Renderer/Material.h"
#include "Renderer/Model.h"

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application::Application()
{
    isRunning = true;
    window = Window::Create(800, 600, "Trilizer Engine");
    window->SetCallBack(BIND_EVENT_FN(OnEvent));

    Input::Get().Init(window->GetNativeWindow());

    InitGL();

    ENGINELOG("Application Initialized");
}

Application::~Application()
{
}

using namespace glm;
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void ProcessDeltaTime()
{
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void MoveCamera();

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;

void Application::Run()
{
std::vector<Vertex> vertices = {
    // Back face
    {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}},
    {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}},
    {{-0.5f, -0.5f, -0.5f}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}},

    // Front face
    {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}},
    {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}},
    {{-0.5f,  0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}},
    {{-0.5f, -0.5f,  0.5f}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}},

    // Left face
    {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
    {{-0.5f,  0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
    {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
    {{-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
    {{-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
    {{-0.5f,  0.5f,  0.5f}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},

    // Right face
    {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}},
    {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
    {{ 0.5f, -0.5f, -0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}},
    {{ 0.5f, -0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}},

    // Bottom face
    {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}},
    {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}},
    {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}},
    {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}},
    {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}},
    {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}},

    // Top face
    {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}},
    {{ 0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}},
    {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}},
    {{ 0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}},
    {{-0.5f,  0.5f,  0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}},
    {{-0.5f,  0.5f, -0.5f}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}},
};


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
    glm::vec3( 0.7f,  0.2f,   2.0f),   // key light (front-right)
    glm::vec3( 2.3f, -3.3f,  -4.0f),   // low side accent
    glm::vec3(-4.0f,  2.0f, -12.0f),   // far background glow
    glm::vec3( 0.0f,  0.0f,  -3.0f),   // central fill
    glm::vec3(-2.5f,  1.5f,   1.0f),   // rim light (left)
    glm::vec3( 1.5f,  2.5f,  -2.0f)    // top soft light
};


    glm::vec3 pointLightColors[] = {
    glm::vec3(1.0f, 0.82f, 0.65f),  // warm key light
    glm::vec3(0.65f, 0.80f, 1.0f),  // cool contrast light
    glm::vec3(1.0f, 0.95f, 0.80f),  // neutral fill
    glm::vec3(0.7f, 0.9f, 1.0f),    // blue atmospheric
    glm::vec3(1.0f, 0.6f, 0.4f),    // warm rim highlight
    glm::vec3(0.85f, 0.9f, 1.0f)    // soft overhead light
};


Shader lightingshader("../../Assets/Shaders/lightshader.glsl");
lightingshader.Use();

glm::mat4 projection;

Shader lightSourceShader("../../Assets/Shaders/lightsource.glsl");
lightSourceShader.Use();

    Texture containerTex("../../Assets/Textures/container2.png");
    Texture specularTex("../../Assets/Textures/container2_specular.png");
    Texture anotherTex("../../Assets/Textures/awesomeface.png", true);
    std::vector<Texture> textures;
    textures.reserve(3);
    textures.emplace_back(std::move(containerTex));
    textures.emplace_back(std::move(specularTex));
    textures.emplace_back(std::move(anotherTex));

    TriMat material(vec3(1.0f), 1.0f, vec3(1.0f), 32.0f, std::move(textures));
    TriMat lightsourceMat(vec3(1.0f), 1.0f, vec3(1.0f), 32.0f);
    TriMesh cube(vertices, {});

    Model backpack("../../Assets/Models/backpack/backpack.obj", true);
    

    //Model companionCube("../../Assets/Models/companion_cube/scene.gltf");

    while (isRunning)
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float frequency = 0.5f;
        for(int i = 0; i < 6; i++){
            vec3 initalPos = pointLightPositions[i];
             // update light position over time
            vec3 newPos = vec3(
                initalPos.x + sin(glfwGetTime() * frequency + i) * 2.0f,
                initalPos.y + cos(glfwGetTime() * frequency + i) * 1.0f,
                initalPos.z + sin(glfwGetTime() * frequency + i) * 2.0f
            );
            vec3 lightPos = newPos;
            mat4 lightmodel = mat4(1.0f);
            lightmodel = translate(lightmodel, lightPos);
            lightmodel = scale(lightmodel, vec3(0.2f));
            projection = glm::perspective(glm::radians(45.0f), window->GetAspectRatio(), 0.1f, 100.0f);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            // draw the lamp object
            lightSourceShader.Use();
            lightSourceShader.SetMat4("model", lightmodel);
            lightSourceShader.SetMat4("view", camera.GetViewMatrix());
            lightSourceShader.SetMat4("projection", projection);
            lightSourceShader.SetVec3("lightColor", pointLightColors[i]);
            lightsourceMat.SetShader(&lightSourceShader);
            lightsourceMat.Apply();
            cube.Draw();
        }


// draw our first cube
        lightingshader.Use();
        lightingshader.SetVec3("viewPos", camera.Position);
        for(int i = 0; i < 5; i++){
            vec3 initalPos = pointLightPositions[i];
             // update light position over time
            vec3 newPos = vec3(
                initalPos.x + sin(glfwGetTime() * frequency + i) * 2.0f,
                initalPos.y + cos(glfwGetTime() * frequency + i) * 1.0f,
                initalPos.z + sin(glfwGetTime() * frequency + i) * 2.0f
            );
            std::string number = std::to_string(i);
            lightingshader.SetVec3("lights[" + number + "].position", newPos);
            lightingshader.SetVec3("lights[" + number + "].ambient", 0.05f, 0.05f, 0.05f);
            lightingshader.SetVec3("lights[" + number + "].diffuse", pointLightColors[i]); // darkened
            lightingshader.SetVec3("lights[" + number + "].specular", 1.0f, 1.0f, 1.0f);

            lightingshader.SetFloat("lights[" + number + "].constant", 1.0f);
            lightingshader.SetFloat("lights[" + number + "].linear", 0.15f);
            lightingshader.SetFloat("lights[" + number + "].quadratic", 0.08f);
        }

        lightingshader.SetVec3("lights[5].position", camera.Position);
        lightingshader.SetVec3("lights[5].direction", camera.Front);
        lightingshader.SetVec3("lights[5].ambient", 0.1f, 0.1f, 0.1f);
        vec3 diff = vec3(1.0f, 1.0f, 1.0f) * 0.9f;
        lightingshader.SetVec3("lights[5].diffuse", diff);
        lightingshader.SetVec3("lights[5].specular", 1.0f, 1.0f, 1.0f);
        lightingshader.SetFloat("lights[5].constant", 1.0f);
        lightingshader.SetFloat("lights[5].linear", 0.09f);
        lightingshader.SetFloat("lights[5].quadratic", 0.032f);
        lightingshader.SetFloat("lights[5].cutOff", glm::cos(glm::radians(12.5f)));
        lightingshader.SetFloat("lights[5].outerCutOff", glm::cos(glm::radians(17.5f)));
        // material properties
        lightingshader.SetFloat("material.shininess", 64.0f);

        // lightingshader.SetMat4("model", model);
        lightingshader.SetMat4("view", camera.GetViewMatrix());
        lightingshader.SetMat4("projection", projection);
        for (unsigned int i = 0; i < 10; i++)
        {
            mat4 model = mat4(1.0f);
            model = translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle),
                                glm::vec3(1.0f, 0.3f, 0.5f));

            model = scale(model, vec3(0.4f));
            lightingshader.SetMat4("model", model);
            //companionCube.Draw(lightingshader);
            backpack.Draw(lightingshader);
        }
        // // should be last to update window events
        window->OnUpdate();
        window->SwapBuffers();
        ProcessDeltaTime();
        MoveCamera();
    }
}

void MoveCamera()
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

void Application::Close()
{
    ENGINELOG("ENGINE CLOSE.");
    isRunning = false;
}

void Application::Initialize()
{
}

bool hideCursor = false;
int lastX = 400, lastY = 300;
void Application::OnEvent(Event &e)
{
    CORELOG(e.ToString());
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>([](WindowResizeEvent &e)
                                           {
        GLCALL(glViewport(0, 0, e.GetWidth(), e.GetHeight()));
        return true; });

    dispatcher.Dispatch<KeyPressEvent>(BIND_EVENT_FN(ProcessKeyPress));
    dispatcher.Dispatch<MouseMoveEvent>([](MouseMoveEvent &e)
                                        {

        if(!hideCursor){
            //man i'm such a genius
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
        
        return true; });

    dispatcher.Dispatch<MouseButtonEvent>([](MouseButtonEvent &e)
                                          {
        if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT)
        {
            hideCursor = !hideCursor;
            if (hideCursor)
                Input::Get().DisableCursor();
            else
                Input::Get().EnableCursor();
        }


        return true; });
}

bool Application::ProcessKeyPress(KeyPressEvent &e)
{
    if (e.GetKeyCode() == GLFW_KEY_ESCAPE)
    {
        Close();
    }

    return true;
}

bool Application::OnWindowClose(WindowCloseEvent &e)
{
    Close();
    return true;
}
