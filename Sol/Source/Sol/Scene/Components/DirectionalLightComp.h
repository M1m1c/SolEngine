#pragma once

#include "GalaxyDraw/Interfaces/LightStructures.h"
#include <glm/glm.hpp>

namespace Sol
{
	struct DirectionalLightComp
	{
		GalaxyDraw::DirectionalLight Dir_Light;

		//TODO we need to use the transform of the entity thsi belongs to,
		// get the forward direciton from that and calculate a local direction for each mesh
		// and apply that local direction to a instanced attribute,
		// which we will then use in the shader to determine if a part of teh mesh is lit or not.
	};
}