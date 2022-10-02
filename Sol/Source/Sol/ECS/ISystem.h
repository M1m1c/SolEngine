#pragma once
#include "ECS_Core.h"
#include "IComponent.h"
#include <set>

namespace Sol
{
	class ISystem
	{
	public:
		virtual ~ISystem() = default;
		virtual void OnStartSystem() = 0;
		virtual void OnUpdateSystem() = 0;

		virtual const std::set<Entity>& GetEntitiesSet() const = 0;
		virtual void AddEntitity(const Entity entity) const = 0;
		virtual void RemoveEntitity(const Entity entity) const = 0;

	};

	
}