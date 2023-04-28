#pragma once

#include "Sol/Scene/Components/TransformComp.h"
#include "Sol/Scene/Components/NameComp.h"
#include "Sol/Scene/Components/SpriteRendererComp.h"
#include "Sol/Scene/Components/CameraComp.h"
#include "Sol/Scene/Components/ModelComp.h"
#include "Sol/Scene/Components/MaterialComp.h"
#include "Sol/Scene/Components/DirectionalLightComp.h"

namespace Sol {
	struct InternalComp
	{
	public:
		InternalComp() = default;
		~InternalComp() = default;
		bool IsVisible() { return m_Visible; }
		void SetIsVisible(bool b) { m_Visible = b; }
	private:
		bool m_Visible = false;
	};
}