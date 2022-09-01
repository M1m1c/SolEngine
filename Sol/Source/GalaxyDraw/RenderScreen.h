#pragma once

#include <glad/glad.h>
#include "VAO.h"
#include "EBO.h"
#include "shaderClass.h"
#include "Model.h"

namespace GalaxyDraw 
{
	class RenderScreen
	{
	public:
		void Draw(const GalaxyDraw::VAO& va, const GalaxyDraw::EBO& ib, const Shader& shader) const;
		void Draw(const Model& model, const Shader& shader) const;
		void Clear() const;
	};
}