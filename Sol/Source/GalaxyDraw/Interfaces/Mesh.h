#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Texture.h"

namespace GalaxyDraw {

	class Shader;

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	struct Mesh {
	public:
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
		std::vector<std::shared_ptr<Texture>>  Textures;//TODO replace with material struct
	
		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<std::shared_ptr<Texture>> textures):
			Vertices(vertices), Indices(indices), Textures(textures)
		{}

	};
}
