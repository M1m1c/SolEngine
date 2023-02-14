#pragma once

#include <glm/glm.hpp>
#include "GalaxyDraw/GalaxyMacros.h"
#include <GalaxyDraw/Interfaces/Texture.h>

namespace Sol 
{
	struct SpriteRendererComp
	{
	public:

		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		s_ptr<GalaxyDraw::Texture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRendererComp() = default;
		SpriteRendererComp(const SpriteRendererComp&) = default;
		SpriteRendererComp(const glm::vec4& color)
			: Color(color) {}

	private:

	};
}