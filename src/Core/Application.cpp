#include "Application.h"
#include "enginepch.h"
#include "Logger/Logger.h"
#include "Renderer/Renderer.h"
#include "Input.h"

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

static Application *appInstance = nullptr;

Application::Application()
{
    Initialize();
}

Application::~Application()
{
}

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void ProcessDeltaTime()
{
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void Application::Run()
{

    while (isRunning)
    {
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        AppUpdate(deltaTime);
        // // should be last to update window events
        window->OnUpdate();
        window->SwapBuffers();
        ProcessDeltaTime();
    }
}

void Application::Close()
{
    AppClose();
    ENGINELOG("ENGINE CLOSE.");
    isRunning = false;
}

void Application::Initialize()
{
    isRunning = true;
    window = Window::Create(800, 600, "Trilizer Engine");
    window->SetCallBack(BIND_EVENT_FN(OnEvent));

    Input::Get().Init(window->GetNativeWindow());

    InitGL();

    appInstance = this;
    ENGINELOG("Engine Initialized");
    AppInit();
    ENGINELOG("Application Initialized");
}

Application *Application::GetInstance()
{
    return appInstance;
}

Window &Application::GetWindow()
{
    return *window;
}

void Application::OnEvent(Event &e)
{
    // CORELOG(e.ToString());
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>([](WindowResizeEvent &e)
                                           {
        GLCALL(glViewport(0, 0, e.GetWidth(), e.GetHeight()));
        return true; });

    dispatcher.Dispatch<KeyPressEvent>(BIND_EVENT_FN(ProcessKeyPress));
    dispatcher.Dispatch<MouseMoveEvent>([](MouseMoveEvent &e)
                                        {

        ClientMouseMoveCallBack(e);
        
        return true; });

    dispatcher.Dispatch<MouseButtonEvent>([](MouseButtonEvent &e)
                                          {

        ClientMouseButtonCallBack(e);

        return true; });
}

bool Application::ProcessKeyPress(KeyPressEvent &e)
{
    if (e.GetKeyCode() == GLFW_KEY_ESCAPE)
    {
        Close();
    }

    ClientKeyCallBack(e);

    return true;
}

bool Application::OnWindowClose(WindowCloseEvent &e)
{
    Close();
    return true;
}
