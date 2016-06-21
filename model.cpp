#include "model.h"

Model::Model(const std::string& _absPath) : absPath(_absPath)
{
	import();
}

void Model::render(GLuint programm)
{
	for (size_t i = 0; i < modelParts.size(); i++)
	{
		modelParts[i].render(programm);
	}
}

void Model::import()
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(absPath, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
        return;
    }

    directory = absPath.substr(0, absPath.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		processMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}
	
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<vertex> vertices;
	std::vector<texture> textures;
	std::vector<GLuint> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertex v;
		v.position.x = mesh->mVertices[i].x;
		v.position.y = mesh->mVertices[i].y;
		v.position.z = mesh->mVertices[i].z;

		v.normal.x = mesh->mNormals[i].x;
		v.normal.y = mesh->mNormals[i].y;
		v.normal.z = mesh->mNormals[i].z;

		vertices.push_back(v);
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	ModelMesh m(vertices, textures, indices);
	this->modelParts.push_back(m);
}

std::vector<texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	return std::vector<texture>();
}