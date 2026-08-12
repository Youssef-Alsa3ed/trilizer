#include "Input.h"

static Input instance;

Input &Input::Get()
{
    return instance;
}

void Input::Init(GLFWwindow *window)
{
    m_Windowptr = window;
}

void Input::DisableCursor()
{
    glfwSetInputMode(m_Windowptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::EnableCursor()
{
    glfwSetInputMode(m_Windowptr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool Input::GetKey(int keycode) const
{
    return glfwGetKey(m_Windowptr, keycode) == GLFW_PRESS;
}

bool Input::GetKeyHold(int keycode) const
{
    return glfwGetKey(m_Windowptr, keycode) == GLFW_REPEAT;
}
