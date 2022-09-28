#pragma once
#include "Entity.h"
#include <cstdint>
namespace Sol 
{
	class IComponent
	{
	public:
		virtual ~IComponent() = default;
		virtual const std::uint32_t& GetType() const = 0;
		virtual const std::shared_ptr<Entity> GetOwner() const = 0;
		virtual void OnStartComponent() = 0;
		virtual void OnUpdateComponent() = 0;
		//virtual void EntityDestroyed(Entity entity) = 0;
	};
}
