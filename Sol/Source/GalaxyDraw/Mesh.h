#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "shaderClass.h"

#include "GLMacros.h"


struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct MeshTexture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public:
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	std::vector<MeshTexture >      textures;
	unsigned int VAO;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture > textures);

	void Draw(Shader& shader);

private:
	// Mesh requires a VAO, VBO and EBO, but only exposes the VAO publicly ...?
	unsigned int VBO, EBO;

	void setupMesh();
};

#endif
