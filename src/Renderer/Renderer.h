#pragma once
#include "glad/glad.h"
#include "Logger/Logger.h"
#include "GLDEBUGGING.hpp"


inline void InitGL(){

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }   

    GLCALL(glViewport(0, 0, 800, 600));
    glEnable(GL_DEPTH_TEST);  
    std::string Vendor(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    std::string Renderer(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    std::string Version(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    std::string ShadingLanguageVersion(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

    INITLOG(std::string("Vendor: " + Vendor));
    INITLOG(std::string("Renderer: " + Renderer));
    INITLOG(std::string("OpenGL Version: " + Version));
    INITLOG(std::string("GLSL Version: " + ShadingLanguageVersion));
}
