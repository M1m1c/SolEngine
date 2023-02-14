#pragma once

#include "GalaxyDraw/SceneCamera.h"

namespace Sol 
{
	struct CameraComp
	{
		GalaxyDraw::SceneCamera OrthoCamera;
		bool IsPirmary = true;
		bool IsFixedAspectRatio = false;

		CameraComp() = default;
		CameraComp(const CameraComp&) = default;
		//CameraComp(const glm::mat4 & projection) : Camera(projection) { }
		~CameraComp() = default;
	};
}