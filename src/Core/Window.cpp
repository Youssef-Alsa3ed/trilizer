#include "Window.h"
#include "Events/WindowEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"
std::unique_ptr<Window> Window::Create(unsigned int width, unsigned int height, std::string title)
{
    return std::make_unique<Window>(width, height, title);
}

void Window::SetCallBack(const EventCallBackFn &callback)
{
    m_Data.Callback = callback;
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

Window::Window(unsigned int width, unsigned int height, std::string title)
{
    m_Data.Width = width;
    m_Data.Height = height;
    m_Data.Title = title;
    m_Data.VSync = true;
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSwapInterval(m_Data.VSync ? 1 : 0);
    // Create a GLFW window
    m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
    if (!m_Window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, &m_Data);
    //Callbacks
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;
        WindowResizeEvent event(width, height);
        data.Callback(event);

    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.Callback(event);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        switch (action)
        {
            case GLFW_PRESS:
            {
                KeyPressEvent event(key, 0);
                data.Callback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleaseEvent event(key);
                data.Callback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressEvent event(key, 1);
                data.Callback(event);
                break;
            }
        }
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseMoveEvent event((float)xpos, (float)ypos);
        data.Callback(event);
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseButtonEvent event(button);

        data.Callback(event);
    });
}

void Window::OnUpdate()
{
    glfwPollEvents();
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(m_Window);
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_Window);
}

unsigned int Window::GetWidth() const
{
    return m_Data.Width;
}

unsigned int Window::GetHeight() const
{
    return m_Data.Height;
}

float Window::GetAspectRatio() const
{
    return (float)m_Data.Width / (float)m_Data.Height; 
}
