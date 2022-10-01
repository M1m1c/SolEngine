#pragma once
#include "ECS_Core.h"
namespace Sol
{
	class IComponent
	{
	public:
		virtual void Create() = 0;// this should take in a parameter that is dependent on what component it is
		virtual ~IComponent() = default;
		virtual const CompType& GetType() const = 0;
		//virtual void EntityDestroyed(Entity entity) = 0;
	};
}
