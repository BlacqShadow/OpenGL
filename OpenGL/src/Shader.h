#pragma once

#include <string>

// Including a hash table for caching so that we don't retrive the location of uniforms over and over again. 
#include <unordered_map>

// Include GLM
#include <glm\glm.hpp>

/* To return multiple return type we need to create a struct that we can return*/
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


class Shader
{
private:
	// Filepath for debug purposes
	std::string m_FilePath;
	unsigned int m_RendererID;
	// caching system
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	// Using bind instead of gl_useProgram to stay consistent throughout the series. 
	void Bind() const;
	void Unbind() const;

	// Set Uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);

private:
	int GetUniformLocation(const std::string& name);
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
};