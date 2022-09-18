#pragma once

#include "Sol/Core/Layer.h"

#include "Sol/Core/Core.h"
#include "Sol/Events/ApplicationEvent.h"
#include "Sol/Events/KeyEvent.h"
#include "Sol/Events/MouseEvent.h"

namespace Sol
{
	class SOL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnImGuiRender() override;


		void Begin();
		void End();

	private:

		float m_Time = 0.0f;
	};
}

