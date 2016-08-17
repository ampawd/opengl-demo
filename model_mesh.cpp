#include "model_mesh.h"

ModelMesh::ModelMesh(std::vector<vertex> _vertices, std::vector<texture> _textures, std::vector<GLuint> _indices) :
	vertices(_vertices), textures(_textures), indices(_indices)
{
	loadMesh();
}

void ModelMesh::loadMesh()
{
	loadVertexAttribs();
}

void ModelMesh::loadVertexAttribs()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(POSITION_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)0);
		glEnableVertexAttribArray(POSITION_LOC);

		glVertexAttribPointer(NORMAL_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, normal));	//	offsetof(vertex, normal)
		glEnableVertexAttribArray(NORMAL_LOC);

		glVertexAttribPointer(TEXTCOORD_LOC, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)offsetof(vertex, texCoord));
		glEnableVertexAttribArray(TEXTCOORD_LOC);
	glBindVertexArray(0);
}

void ModelMesh::render(GLuint programm)
{
	std::string textureTypeStr, number;
	int diffTexCount = 0, specTexCount = 0;
	for (size_t i = 0, texturesSize = textures.size(); i < texturesSize; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		textureTypeStr = textures[i].type;

		if (textureTypeStr == "diffuseTexture") {
			diffTexCount++;
			number = std::to_string(diffTexCount);
		}

		if (textureTypeStr == "specularTexture") {
			specTexCount++;
			number = std::to_string(specTexCount);
		}

		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
		glUniform1i(glGetUniformLocation(programm, ("material." + textureTypeStr + number).c_str()), i);
		glUniform1f(glGetUniformLocation(programm, "material.shininess"), 32.0);
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (size_t i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);	//	unbind
	}
}

const std::vector<vertex>& ModelMesh::getVertices() const
{
	return vertices;
}

const std::vector<texture>& ModelMesh::getTextures() const 
{
	return textures;
}
	
ModelMesh::~ModelMesh()
{
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}