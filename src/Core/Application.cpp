#include "Application.h"
#include "enginepch.h"
#include "Logger/Logger.h"
#include "Renderer/Renderer.h"
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application::Application()
{
    isRunning = true;
    window = Window::Create(800, 600, "Trilizer Engine");
    window->SetCallBack(BIND_EVENT_FN(OnEvent));

    InitGL();

    ENGINELOG("Application Initialized");
}

Application::~Application()
{
}

void Application::Run()
{
    while (isRunning)
    {



        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // should be last to update window events
        window->OnUpdate();
    }
}

void Application::Close()
{
    ENGINELOG("ENGINE CLOSE.");
    isRunning = false;
}

void Application::Initialize()
{
}

void Application::OnEvent(Event &e)
{
    CORELOG(e.ToString());
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
}

bool Application::OnWindowClose(WindowCloseEvent &e)
{
    Close();
    return true;
}
