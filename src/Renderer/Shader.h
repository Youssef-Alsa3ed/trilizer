#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
private:
	int m_ProgramID;

	std::tuple<std::string, std::string> ParseShader(const std::string &shaderLocation);

	int GetUniformLocation(const std::string &name) const;

	void CreateProgram(const std::string &shaderLocation);

public:
	Shader(const std::string &shaderLocation);

	Shader(const Shader &other) = delete;

	~Shader();

	void Use() const;
	void SetBool(const std::string &name, bool value) const;
	// ------------------------------------------------------------------------
	void SetInt(const std::string &name, int value) const;
	// ------------------------------------------------------------------------
	void SetFloat(const std::string &name, float value) const;
	// ------------------------------------------------------------------------
	void SetVec2(const std::string &name, const glm::vec2 &value) const;
	void SetVec2(const std::string &name, float x, float y) const;
	// ------------------------------------------------------------------------
	void SetVec3(const std::string &name, const glm::vec3 &value) const;
	void SetVec3(const std::string &name, float x, float y, float z) const;
	// ------------------------------------------------------------------------
	void SetVec4(const std::string &name, const glm::vec4 &value) const;
	void SetVec4(const std::string &name, float x, float y, float z, float w) const;
	// ------------------------------------------------------------------------
	void SetMat2(const std::string &name, const glm::mat2 &mat) const;
	// ------------------------------------------------------------------------
	void SetMat3(const std::string &name, const glm::mat3 &mat) const;
	// ------------------------------------------------------------------------
	void SetMat4(const std::string &name, const glm::mat4 &mat) const;
};