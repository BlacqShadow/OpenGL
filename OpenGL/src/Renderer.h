#pragma once

#include<GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// IMGUI library headers
#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>


// MACROS
/* Break the program if the assertion is false */
#define ASSERT(x) if (!(x)) __debugbreak();
/* Macro to call GLClearError and GLLogCall before and after the function */
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/* Clear all the errors */
void GLClearError();

/* Print the errors */
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	bool* m_show_demo_window;
	bool* m_show_another_window;
	ImVec4* m_clear_color;
public:
	Renderer();
	~Renderer();
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
