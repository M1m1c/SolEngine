#pragma once
#include <memory>
#include <Sol/ECS/IComponent.h>

namespace Sol {
	class ComponentManager
	{
	public:
		ComponentManager();
		~ComponentManager();

		// TODO should create a component of type and place it in map with id as key
		static  std::shared_ptr<IComponent> CreateComponent(const std::uint32_t& entityID, const std::uint32_t& compType);
	private:

	};
}