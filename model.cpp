#include "model.h"

Model::Model(std::string& _absPath) : absPath(_absPath)
{
	importModel();
}

void Model::render(GLuint programm)
{
	for (size_t i = 0; i < modelParts.size(); i++)
	{
		modelParts[i].render(programm);
	}
}

void Model::importModel()
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

}

void Model::processMesh(aiMesh* mesh, const aiScene* scene)
{

}

std::vector<texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{

}