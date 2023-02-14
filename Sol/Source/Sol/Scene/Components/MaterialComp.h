#pragma once

#include <glm/glm.hpp>
#include <GalaxyDraw/Interfaces/Texture.h>


namespace Sol
{
	//TODO make this use and load textures, also every entity with a ModelComp should probably have a material comp
	struct MaterialComp
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		MaterialComp() = default;
		MaterialComp(const MaterialComp&) = default;

	};
}