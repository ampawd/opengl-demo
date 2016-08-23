#ifndef _MODEL_H
#define _MODEL_H

#include "model_mesh.h"
#include <SOIL.h>
#include <unordered_map>

class Model 
{
	public:
		std::string absPath, directory;
		Model(const std::string&);
		void render(GLuint);
	
	private:
		std::unordered_map<std::string, texture> loadedTextures;
		const aiScene* scene;
		std::vector<ModelMesh> modelParts;
		void import();
		void processNode(aiNode*);
		void processMesh(aiMesh*);
		std::vector<texture> loadMaterialTextures(aiMaterial*, aiTextureType, const std::string&); 
		GLint getTextureImageID(const std::string&);
};

#endif