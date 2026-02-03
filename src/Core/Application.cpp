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
float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

    VertexArray cubeVAO;

    VertexBuffer VBO(vertices, sizeof(vertices));
    VBL layout;
    layout.Push<float>(3, "aPos");
    layout.Push<float>(3, "aNormal");
    layout.Push<float>(2, "aTexCoord");
    cubeVAO.AddBuffer(VBO, layout);

    Shader lightingshader("../../Assets/Shaders/lightshader.glsl");
    lightingshader.Use();

    glm::mat4 projection;

    VertexArray lightVAO;
    lightVAO.AddBuffer(VBO, layout);
    vec3 lightPos(1.2f, 1.0f, 2.0f);
    mat4 lightmodel = mat4(1.0f);
    lightmodel = translate(lightmodel, lightPos);
    lightmodel = scale(lightmodel, vec3(0.2f)); // a smaller cube
    
    Shader lightSourceShader("../../Assets/Shaders/lightsource.glsl");
    lightSourceShader.Use();

    Texture containerTex("../../Assets/Textures/container2.png");
    Texture specularTex("../../Assets/Textures/container2_specular.png");
    while (isRunning)
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        projection = glm::perspective(glm::radians(45.0f), window->GetAspectRatio(), 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        
        // draw the lamp object
        lightSourceShader.Use();
        lightSourceShader.SetMat4("model", lightmodel);
        lightSourceShader.SetMat4("view", camera.GetViewMatrix());
        lightSourceShader.SetMat4("projection", projection);
        lightVAO.Bind();
        GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));

        // draw our first cube
        lightingshader.Use();
        lightingshader.SetVec3("viewPos", camera.Position);
        lightingshader.SetVec3("light.position", lightPos);
        
        //light properties
        lightingshader.SetVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        lightingshader.SetVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darkened
        lightingshader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);
        
        // material properties
        lightingshader.SetFloat("material.shininess", 64.0f);
        containerTex.Bind(0);
        specularTex.Bind(1);
        lightingshader.SetInt("material.diffuse", 0);
        lightingshader.SetInt("material.specular", 1);

        //lightingshader.SetMat4("model", model);
        lightingshader.SetMat4("view", camera.GetViewMatrix());
        lightingshader.SetMat4("projection", projection);
        cubeVAO.Bind();
        for(unsigned int i = 0; i < 10; i++)
        {
            mat4 model = mat4(1.0f);
            model = translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle),
            glm::vec3(1.0f, 0.3f, 0.5f));
            lightingshader.SetMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // should be last to update window events
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
