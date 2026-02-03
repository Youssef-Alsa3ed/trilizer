#pragma once

#include "GLFW/glfw3.h"


class Input
{
private:
    GLFWwindow* m_Windowptr;
public:
    static Input& Get();
    void Init(GLFWwindow* window);
    void DisableCursor();
    void EnableCursor();
    
    bool GetKey(int keycode) const;
    bool GetKeyHold(int keycode) const;
    Input() = default;
    ~Input() = default;

    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

};

