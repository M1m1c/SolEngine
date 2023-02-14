#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Texture.h"
#include <Sol/Scene/Components/TransformComp.h>

namespace GalaxyDraw
{

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec4 Color;
		// Editor-only
		int EntityID;
	};

	struct Mesh {
	public:
		std::string Name;
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
		std::vector<std::shared_ptr<Texture>>  Textures;//TODO replace with material struct
		Sol::TransformComp MeshTransform;

		Mesh(std::string name,
			std::vector<Vertex> vertices,
			std::vector<uint32_t> indices,
			std::vector<std::shared_ptr<Texture>> textures,
			const Sol::TransformComp& meshTransform) :
			Name(name), Vertices(vertices), Indices(indices), Textures(textures), MeshTransform(meshTransform)
		{}

	};
}
