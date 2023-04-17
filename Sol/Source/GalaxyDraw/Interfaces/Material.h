#pragma once
#include <glm/glm.hpp>

namespace GalaxyDraw {

	struct Material
	{
		Material(uint32_t texIndex, std::string name = "newMaterial", glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f))
			: TextureIndex(texIndex), Name(name) , Color(color) {}
		uint32_t TextureIndex;//TextureIndices[5];
		glm::vec4 Color;
		std::string Name;
		// Other material properties go here

	private:
		//TODO make TexureIndex private so we can only manipualte it through functions
	};
}