#pragma once
#include "ECS_Core.h"
namespace Sol
{
	template<typename T>
	class IComponent
	{
	public:
		virtual IComponent* Create(T startValue) = 0;// this should take in a parameter that is dependent on what component it is
		virtual ~IComponent() = default;
		virtual const CompType& GetType() const = 0;
		//virtual void EntityDestroyed(Entity entity) = 0;
	};
}
