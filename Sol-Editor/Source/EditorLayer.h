#pragma once

#include "Sol.h"

namespace Sol 
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		
		virtual void OnUpdate(TimeStep deltaTime) {};

		virtual void OnFixedUpdate(TimeStep fixedStep, const float fixedTime) {};
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) {};
	private:
		Sol::u_ptr<GD_Framebuffer> m_Framebuffer;
	};
}