#ifndef _MODEL_MESH
#define _MODEL_MESH

#include <gl/glew.h>
#include <iostream>
#include <string>
#include <vector>
#include <gl/glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define POSITION_LOC  0
#define NORMAL_LOC	  1
#define TEXTCOORD_LOC 2

struct vertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

struct texture
{
	GLuint ID;
	std::string type;
	aiString path;
};

class ModelMesh
{
	private:
		GLuint vbo, vao, ebo;
		void loadMesh();
		void loadVertexAttribs();
		std::vector<vertex>  vertices;
		std::vector<texture> textures;
		std::vector<GLuint>  indices;

	public:
		ModelMesh(std::vector<vertex>&, std::vector<texture>&, std::vector<GLuint>&);
		const std::vector<vertex>& getVertices() const;
		const std::vector<texture>& getTextures() const;
		void render(GLuint);

		~ModelMesh();
};

#endif