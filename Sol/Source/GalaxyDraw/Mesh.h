#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>

namespace GalaxyDraw {

	class Shader;

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct MeshTexture {
		uint32_t id;
		std::string type;
		std::string path;
	};

	class Mesh {
	public:
		std::vector<Vertex>       vertices;
		std::vector<uint32_t> indices;
		std::vector<MeshTexture>  textures;
		unsigned int VAO;

		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<MeshTexture > textures);

		void Draw(Shader& shader);

	private:
		// Mesh requires a VAO, VBO and EBO, but only exposes the VAO publicly ...?
		unsigned int VBO, EBO;

		void setupMesh();
	};
}
#endif
