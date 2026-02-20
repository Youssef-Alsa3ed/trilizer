#pragma once
#include "glad/glad.h"
#include "Logger/Logger.h"
#include "GLDEBUGGING.hpp"

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            unsigned int id,
                            GLenum severity,
                            GLsizei length,
                            const char *message,
                            const void *userParam)
{
    // Ignore non-significant errors
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    std::cout << "Source: ";
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        std::cout << "API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::cout << "Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::cout << "Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::cout << "Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        std::cout << "Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        std::cout << "Other";
        break;
    }
    std::cout << std::endl;

    std::cout << "Type: ";
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        std::cout << "Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        std::cout << "Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        std::cout << "Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "Other";
        break;
    }
    std::cout << std::endl;

    std::cout << "Severity: ";
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "HIGH";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "LOW";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "NOTIFICATION";
        break;
    }
    std::cout << std::endl;
}

inline void InitGL()
{

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    GLCALL(glViewport(0, 0, 800, 600));
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    
    std::string Vendor(reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
    std::string Renderer(reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
    std::string Version(reinterpret_cast<const char *>(glGetString(GL_VERSION)));
    std::string ShadingLanguageVersion(reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

    INITLOG(std::string("Vendor: " + Vendor));
    INITLOG(std::string("Renderer: " + Renderer));
    INITLOG(std::string("OpenGL Version: " + Version));
    INITLOG(std::string("GLSL Version: " + ShadingLanguageVersion));

    // debugging
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE,
                              0, nullptr, GL_TRUE);
    }
}
