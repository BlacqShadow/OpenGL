#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

//Delete iostream
#include <iostream>

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}


VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
	std::cout << "Destructor Called" << std::endl;
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0; 
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		// Go through all the elements present in the vector class. 
		const auto& element = elements[i];
		//Tell openGL the layout of the memory, the buffer should be bound before doing this 
		// Also binds vertex array object to our vertex buffer
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		//Enable the vertex attrib pointer 
		GLCall(glEnableVertexAttribArray(i));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);

	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
