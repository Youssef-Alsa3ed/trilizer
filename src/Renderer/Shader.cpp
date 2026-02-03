#include "Shader.h"
#include <tuple>
#include <glad/glad.h>
#include <Renderer/GLDEBUGGING.hpp>
#include <sstream>
#include "Logger/Logger.h"

std::tuple<std::string, std::string> Shader::ParseShader(const std::string &shaderLocation)
{
    std::ifstream shaderFileReader(shaderLocation);
    std::string line;
    std::stringstream vSource, fSource;

    while (getline(shaderFileReader, line))
    {
        if (line.find("#shader vertex") != std::string::npos)
        {
            while (getline(shaderFileReader, line))
            {
                if (line.find("#shader fragment") != std::string::npos)
                {
                    break;
                }
                vSource << line << '\n';
            }
        }
        if (line.find("#shader fragment") != std::string::npos)
        {
            while (getline(shaderFileReader, line))
            {
                fSource << line << '\n';
            }
        }
    }

    return std::make_tuple(vSource.str(), fSource.str());
}

void Shader::CreateProgram(const std::string &shaderLocation)
{
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    std::string vertexSourceStr, fragSourceStr;
    std::tie(vertexSourceStr, fragSourceStr) = ParseShader(shaderLocation);
    const char* vertexSource = vertexSourceStr.c_str();
    const char* fragSource = fragSourceStr.c_str();
    //TRACELOG("Vertex Shader Source:\n" + vertexSourceStr);
    //TRACELOG("Fragment Shader Source:\n" + fragSourceStr);
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragSource, NULL);
    glCompileShader(fragment);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        ERRLOG("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
        ERRLOG(infoLog);
    };

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        ERRLOG("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED");
        ERRLOG(infoLog);
    };

    m_ProgramID = glCreateProgram();
    glAttachShader(m_ProgramID, vertex);
    glAttachShader(m_ProgramID, fragment);
    glLinkProgram(m_ProgramID);

    glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(vertex, 512, NULL, infoLog);
        ERRLOG("PROGRAM::LINKING_FAILED");
        ERRLOG(infoLog);
    };

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
Shader::Shader(const std::string & shaderLocation)
{
    if (shaderLocation.empty())
    {
        std::cout << "Invalid Shader Path." << std::endl;
        return;
    }
    CreateProgram(shaderLocation);
}
Shader::~Shader()
{
    GLCALL(glDeleteProgram(m_ProgramID));
}

void Shader::Use() const
{
    GLCALL(glUseProgram(m_ProgramID));
}

int Shader::GetUniformLocation(const std::string &name) const
{
    GLCALL(int location = glGetUniformLocation(m_ProgramID, name.c_str()));

    if (location == -1){
        std::stringstream ss;
        ss << "Warning: uniform '" << name << "' not found or optimized out\n";
        WARNLOG(ss.str());
    }
    return location;
}


void Shader::SetBool(const std::string &name, bool value) const
{
    GLCALL(glUniform1i(GetUniformLocation(name), (int)value));
}

void Shader::SetInt(const std::string &name, int value) const
{
    GLCALL(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetFloat(const std::string &name, float value) const
{
    GLCALL(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetVec2(const std::string &name, const glm::vec2 &value) const
{
    GLCALL(glUniform2fv(GetUniformLocation(name), 1, &value[0]));
}

void Shader::SetVec2(const std::string &name, float x, float y) const
{
    GLCALL(glUniform2f(GetUniformLocation(name), x, y));
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const
{
    GLCALL(glUniform3fv(GetUniformLocation(name), 1, &value[0]));
}

void Shader::SetVec3(const std::string &name, float x, float y, float z) const
{
    GLCALL(glUniform3f(GetUniformLocation(name), x, y, z));
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &value) const
{
    GLCALL(glUniform4fv(GetUniformLocation(name), 1, &value[0]));
}

void Shader::SetVec4(const std::string &name, float x, float y, float z, float w) const
{
    GLCALL(glUniform4f(GetUniformLocation(name), x, y, z, w));
}

void Shader::SetMat2(const std::string &name, const glm::mat2 &mat) const
{
    GLCALL(glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

void Shader::SetMat3(const std::string &name, const glm::mat3 &mat) const
{
    GLCALL(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const
{
    GLCALL(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}
