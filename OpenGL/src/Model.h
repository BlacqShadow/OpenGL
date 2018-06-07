#pragma once
#include "Mesh.h"
#include <vector>

// 3D object loading
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"



class Model
{
private:
	// Using vectors to store all the arrays so it is a contiguous block of memory
	std::vector<Mesh*> m_Meshes;
	std::vector<Texture* > m_Textures;

public:
	Model(const std::string& path);
	~Model();
	inline std::vector<Mesh*> GetMeshes() const
	{
		return m_Meshes;
	}

private:
	void load(const std::string& path);
	// Process the raw ai mesh retrieved using assimp
	Mesh* processMesh(const aiMesh* mesh, const aiScene* scene);
	std::vector<Texture* > LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);

};