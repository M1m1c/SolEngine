#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Sol/Core/TimeStep.h"

namespace Sol 
{

	class SOL_API Layer
	{
	public:
		Layer(const std::string& name = "layer");
		~Layer();
		
		virtual void OnAttach(){};
		virtual void OnDetach(){};
		virtual void OnUpdate(TimeStep deltaTime){};
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Event& event){};

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}

