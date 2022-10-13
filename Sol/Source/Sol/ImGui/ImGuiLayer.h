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

		virtual void OnUpdate(TimeStep deltaTime) override;
		virtual void OnFixedUpdate(TimeStep fixedStep, const float fixedTime) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void SetBlockEvents(bool b) { m_BlockEvents = b; }
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}

