#pragma once
#include "Sol/ECS/ECS_Core.h"
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

		virtual const std::set<EntityID>& GetEntitiesSet() const = 0;
		virtual void AddEntitity(const EntityID entity) const = 0;
		virtual void RemoveEntitity(const EntityID entity) const = 0;

	};

	
}