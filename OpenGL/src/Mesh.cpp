#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices,const std::vector<unsigned int>& indices,const std::vector<Texture* >& textures)
	:m_VB(&vertices[0], vertices.size() * sizeof(Vertex)), m_IB(&indices[0], indices.size()), m_Textures(textures)
{
	
	// Set the layout for the model here: 
	// 3 Position floats
	// 3 normal floats
	// 2 tex co-ordinate floats
	m_VBLayout.Push<float>(3);
	m_VBLayout.Push<float>(3);
	m_VBLayout.Push<float>(2);

	m_VA.AddBuffer(m_VB, m_VBLayout);

}

const std::vector<Texture* >& Mesh::Textures() const
{
	return m_Textures;
}
