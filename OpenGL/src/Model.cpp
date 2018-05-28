#include "Model.h"

Model::Model(const std::string& path)
{
	load(path);
}

Model::~Model()
{
	// Free the memory that was allocated for model meshes
	for (unsigned int i = 0; i < m_Meshes.size(); i++)
	{
		delete m_Meshes[i];
	}
}

void Model::load(const std::string& path)
{
	Assimp::Importer importer;
	// Read the obj file in using assimp
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);
	if (!scene)
	{
		std::cout << "Error: " << importer.GetErrorString() << std::endl;
		return;
	}
	// Store all the meshes in mesh vector
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[i];
		m_Meshes.push_back(processMesh(mesh, scene));
	}
}

Mesh* Model::processMesh(const aiMesh* mesh,const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	// TODO: Implement Textures
	std::vector<Texture> textures;

	// Reserve memory for vertices for optimization
	vertices.reserve(mesh->mNumVertices);
	// Retrive Vertex Data from each vertex
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// Vertex co-ordinate
		glm::vec3 tempvector;
		tempvector.x = mesh->mVertices[i].x;
		tempvector.y = mesh->mVertices[i].y;
		tempvector.z = mesh->mVertices[i].z;
		vertex.Position = tempvector;

		// Normals
		tempvector.x = mesh->mNormals[i].x;
		tempvector.y = mesh->mNormals[i].y;
		tempvector.z = mesh->mNormals[i].z;
		vertex.Normal = tempvector;

		// Assipmt allows upto 8 tex co-ordinates so the array is 
		// indexed by the tex co-ordinate
		// Check if the mesh contains tex co-oridnates ? 
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 tempvec;
			tempvec.x = mesh->mTextureCoords[0][i].x;
			tempvec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoord = tempvec;
		}
		vertices.push_back(vertex);
	}
	// Get the indicies now
	// Face contains which vertecies need to be drawn in what order, for optimzation
	// etc
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// TODO: Get the material data
	// We want our mesh to 
	//Mesh meshtesting(vertices, indices, textures);
	return new Mesh(vertices, indices, textures);

}

