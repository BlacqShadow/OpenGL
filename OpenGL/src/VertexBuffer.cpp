#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void * data, unsigned int size)
{
	// The ID of the buffer is written into the unsigned int
	GLCall(glGenBuffers(1, &m_RendererID));
	//Select the buffer
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	//Provide the buffer with data, 
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); // Pass array and not the array pointer
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	//Select the buffer
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
