#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE:	 return 1;
		}
		ASSERT(false);
		return 0; 
	}
};

class VertexBufferLayout
{
private: 
	// Create a vector that contains all the elements
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride; 
public:
	VertexBufferLayout()
		:m_Stride(0) {

	}

	// If you try to push something that is not defined. 
	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	// Trying to push in floats into vertex buffer elements 
	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	// Pushing in unsigned int
	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE});
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}
	
	// Pushing in normalized chars 
	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE});
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	// Get the elements
	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }

	// Keep a track of the stride
	inline unsigned int GetStride() const { return m_Stride; }
};

