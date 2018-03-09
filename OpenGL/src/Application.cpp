#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

// MACROS
/* Break the program if the assertion is false */
#define ASSERT(x) if (!(x)) __debugbreak();
/* Macro to call GLClearError and GLLogCall before and after the function */
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/* Clear all the errors */
static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

/* Print the errors */
static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error]: " << std::hex << error << std::dec <<": "<< function << " " << file << ": " << line << std::endl;
		return false;
	}
	return true; 
}
/* To return multiple return type we need to create a struct that we can return*/
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource; 
};

/* Open Baisc.shader file and read in the two different shaders */
static ShaderProgramSource ParseShader(const std::string& filepath)
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


static unsigned int CompileShader(unsigned int type, const std::string& source)
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
			(type == GL_VERTEX_SHADER ? "Vertex Shader: " : "Fragment Shader: ") << message <<  std::endl;
		GLCall(glDeleteShader(id));
		return -1;
	}
	return id; 
}

/* Compile the source code for both shaders, link them together in a program and return an index to that program */
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	// Limit the framerate to the refresh rate of the monitor
	glfwSwapInterval(1);

	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[] = {
		-0.5f, -0.5f, // 0
		 0.5f, -0.5f, // 1
		 0.5f,  0.5f, // 2
		-0.5f,  0.5f // 3
	};

	/* To prevent vertex duplication, using indices to refer to the vertex we need to draw a square. */
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	//Create a buffer on the gpu, i.e. vertex buffer
	unsigned int buffer;
	// The ID of the buffer is written into the unsigned int
	GLCall(glGenBuffers(1, &buffer));
	//Select the buffer
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	//Provide the buffer with data, 
	GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), &positions, GL_STATIC_DRAW)); // Pass array and not the array pointer

	//Tell openGL the layout of the memory, the buffer should be bound before doing this 
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
	//Enable the vertex attrib pointer
	GLCall(glEnableVertexAttribArray(0));

	/* Create an index buffer object on the GPU */
	unsigned int ibo;
	// The ID of the buffer is written into the unsigned int
	GLCall(glGenBuffers(1, &ibo));
	//Select the buffer
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	//Provide the buffer with data, 
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &indices, GL_STATIC_DRAW)); // Pass array and not the array pointer

	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));

	/* After the shader is bound, data can be sent to it */
	// Get the location of the uniform variable from the shader
	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	// If open GL deletes the uniform as it is not being used return an error
	ASSERT(location != -1);
	GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

	/* Variables to change the color of the rectangle */
	float increment = 0.05f;
	float r = 0.8f; 

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		// set the colors on the fly
		GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
		// Issue draw call for the buffer
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		/* glDrawElements is used to draw from the index buffers */
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // Nullpointer because the buffer has already been bound

		/* Change the r values as the program loops*/
		if (r > 1.0f)
			increment = -0.05f;
		else if (r < 0.0f)
			increment = 0.05f;
		r += increment;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	GLCall(glDeleteProgram(shader));

	glfwTerminate();
	return 0;
}