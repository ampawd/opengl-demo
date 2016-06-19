#ifndef _MODEL_H
#define _MODEL_H

#include "model_mesh.h"

class Model 
{
	public:
		std::string absPath, directory;
		Model(std::string&);
		void render(GLuint);
	
	private:
		std::vector<ModelMesh> modelParts;
		void importModel();
		void processNode(aiNode*, const aiScene*);
		void processMesh(aiMesh*, const aiScene*);
		std::vector<texture> loadMaterialTextures(aiMaterial*, aiTextureType, std::string); 
};



#endif