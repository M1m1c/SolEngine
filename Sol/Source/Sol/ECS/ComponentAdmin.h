#pragma once
#include "ECS_Core.h"
#include "IComponent.h"
#include "ComponentPool.h"
#include <memory>
#include <unordered_map>

namespace Sol {
	class ComponentAdmin
	{
	public:
		ComponentAdmin();
		~ComponentAdmin();

		template<typename T>
		inline void RegisterComponent();	

		template<typename T>
		inline CompType GetComponentType();	

		template<typename T>
		inline void AddComponent(Entity entity, T component);
		
		template<typename T>
		inline void RemoveComponent(Entity entity);

		template<typename T>
		inline T& GetComponent(Entity entity);

		inline void EntityDestroyed(Entity entity);

		// TODO should create a component of type and place it in map with id as key
		//static  std::shared_ptr<IComponent> CreateComponent(const std::uint32_t& entityID, const std::uint32_t& compType);
	private:
		// Map from type string pointer to a component type
		std::unordered_map<const char*, CompType> m_ComponentTypes;

		// Map from type string pointer to a component array
		std::unordered_map<const char*, s_ptr<IComponentPool>> m_ComponentArrays;

		// The component type to be assigned to the next registered component - starting at 0
		CompType m_NextComponentType;

		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template<typename T>
		std::shared_ptr<ComponentPool<T>> GetComponentArray();	
	};
}