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
		//Updates once each frame and renders to screen.
		//Called after OnFixedUpdate
		virtual void OnUpdate(TimeStep deltaTime){};
		//Updates at a fixed rate each frame, happens before OnUpdate. 
		// fixedStep is the size of each step.
		// fixedTime is the update frequency.
		virtual void OnFixedUpdate(TimeStep fixedStep, const float fixedTime){};
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Event& e){};

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}

