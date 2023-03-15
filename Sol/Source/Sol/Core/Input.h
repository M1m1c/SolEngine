#pragma once

#include "Core.h"

namespace Sol 
{
	class SOL_API Input
	{
	public:
		
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static void SetMousePosition(float posX, float posY);
		static void SetDisplayCursor(bool b);

		static float GetMouseX();
		static float GetMouseY();
	};


}