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
	scene = importer.ReadFile(absPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
        return;
    }

    directory = absPath.substr(0, absPath.find_last_of('/'));
    processNode(scene->mRootNode);
}

void Model::processNode(aiNode* node)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		processMesh(scene->mMeshes[node->mMeshes[i]]);
	}
	
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i]);
	}
}

void Model::processMesh(aiMesh* mesh)
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
		if (mesh->mTextureCoords[0]) 
		{
			v.texCoord.x = mesh->mTextureCoords[0][i].x;
			v.texCoord.y = mesh->mTextureCoords[0][i].y;
		} else {
			v.texCoord = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(v);
	}

	if (mesh->mMaterialIndex >= 0) 
	{
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<texture> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "diffuseTexture");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<texture> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "specularTexture");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
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

std::vector<texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType textureType, const std::string& textureTypeStr)
{
	std::vector<texture> textures;
	for (size_t i = 0; i < mat->GetTextureCount(textureType); i++)
	{
		aiString filename;
		mat->GetTexture(textureType, i, &filename);
		const char* filenameStr = filename.C_Str();
		if (loadedTextures.find(filenameStr) == loadedTextures.end())
		{
			texture texture;
			texture.ID = getTextureImageID(filenameStr);
			texture.type = textureTypeStr;
			texture.filename = filenameStr;
			loadedTextures[filename.C_Str()] = texture;
			textures.push_back(texture);
		} 
		else 
		{
			textures.push_back(loadedTextures[filenameStr]);
		}	
	}

	return textures;
}

GLint Model::getTextureImageID(const std::string& filename)
{
	std::string path = directory + '/' + filename;
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width,height;
    unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);	

    // params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;
}