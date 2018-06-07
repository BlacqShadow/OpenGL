#pragma once 
#include <iostream>
#include <vector>
#include "Texture.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"


// GLM Math Library
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"

// This struck represents one vertex of the mesh
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

// This is how we will represent a single mesh
class Mesh
{
private:
	// Store them for debugging purposes ? 
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Texture*> m_Textures;

	// Each mesh needs to be able to spit out its VAO and IB for the renderer
	VertexBuffer m_VB;
	VertexBufferLayout m_VBLayout;
	VertexArray m_VA;
	IndexBuffer m_IB;

public: 
	// Take in constant 
	Mesh(const std::vector<Vertex>& vertices,const std::vector<unsigned int>& indices,const std::vector<Texture* >& textures);

	// Return a reference to the IndexBuffer and Vertex Buffer
	inline const VertexArray& VA() const
	{ 
		return m_VA; 
	}
	inline const IndexBuffer& IB() const
	{ 
		return m_IB; 
	}
	const std::vector<Texture*>& Textures() const;


};