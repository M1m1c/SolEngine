#pragma once
#include <glm/glm.hpp>

namespace GalaxyDraw {

	struct Material
	{
		Material(uint32_t texIndex,glm::vec4 color=glm::vec4(1.f,1.f,1.f,1.f)) 
			: TextureIndex(texIndex),Color(color){}
		uint32_t TextureIndex;//TextureIndices[5];
		glm::vec4 Color;
		// Other material properties go here
	};
}