#include "Renderer.h"
#include <iostream>

/* Clear all the errors */
void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

/* Print the errors */
bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error]: " << std::hex << error << std::dec << ": " << function << " " << file << ": " << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Clear() const
{
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray & va, const IndexBuffer & ib, const Shader & shader) const
{
	/* Bind all the buffers again before issuing a draw call */
	shader.Bind();
	va.Bind();
	ib.Bind();
	/* glDrawElements is used to draw from the index buffers */
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr)); // Nullpointer because the buffer has already been bound
}
