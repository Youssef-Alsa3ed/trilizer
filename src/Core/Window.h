#pragma once
#define GLFW_INCLUDE_NONE
#include "enginepch.h"
#include "GLFW/glfw3.h"
#include "Events/Event.h"

class Window
{
public:
    using EventCallBackFn = std::function<void(Event &)>;

    void SetCallBack(const EventCallBackFn &callback);
    ~Window();
    Window(unsigned int width, unsigned int height, std::string title);

    void OnUpdate();
    bool ShouldClose() const;
    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

    inline GLFWwindow* GetNativeWindow() const { return m_Window; }

    static std::unique_ptr<Window> Create(unsigned int width, unsigned int height, std::string title);

private:
    struct WindowData
    {
        unsigned int Width, Height;
        std::string Title;
        bool VSync;
        EventCallBackFn Callback;
    };
    GLFWwindow *m_Window;
    WindowData m_Data;
};
