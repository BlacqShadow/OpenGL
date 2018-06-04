#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Renderer.h"



Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}


/* Compile the source code for both shaders, link them together in a program and return an index to that program */
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);                                                                                                                                                                                                      

	// Attach both the shaders to the program object
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	// Link them together in the program
	GLCall(glLinkProgram(program));
	// Checks if the program can be executed in the current state of the machine. 
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}



/* Open Baisc.shader file and read in the two different shaders */
ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	/* Shader type that we are reading in. */
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	/* Find out where the data different shaders start in out shader file */
	while (getline(stream, line))
	{
		/* if it finds shader on the line */
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		/* If there is any other line, just add it to the source code of the vertex/fragment shaders*/
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	/* Multiple return types using a struct. */
	return { ss[0].str(), ss[1].str() };
}


unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	// Create a shader based on the type passed in
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	// Pass in the source code of the shader
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	// Error Checking
	// Query glCompileShader for any errors
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << " Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "Vertex Shader: " : "Fragment Shader: ") << message << std::endl;
		GLCall(glDeleteShader(id));
		return -1;
	}
	return id;
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform3f(const std::string& name, const glm::vec3& value)
{
	GLCall(glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value)));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}


void Shader::SetUniformMat4f(const std::string& name, const glm::mat4 matrix)
{
	// Takes in name, number of matrices, does the matrix need to be transposed? i.e. changed to
	//	column major, and pointer to the float array (in our case it points to the first element. 
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name)
{
	// If it already contains the location, then return the location 
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	// Cache the location 
	m_UniformLocationCache[name] = location;
	return location;
}
