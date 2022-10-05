#pragma once
#include "ECS_Core.h"
#include "IComponent.h"
#include "Sol/Core/CrammedVecMap.h"
#include <memory>
#include <unordered_map>

namespace Sol {
	class ComponentAdmin
	{
	public:
		ComponentAdmin();
		~ComponentAdmin();

		//Registers component T to the component type map and assigns it an int ID so it can be recognised as a usable type
		template<typename T>
		inline void RegisterComponent();	

		template<typename T>
		inline CompType GetComponentType();	

		template<typename T>
		inline void AddComponent(EntityID entity, T component);
		
		template<typename T>
		inline void RemoveComponent(EntityID entity);

		template<typename T>
		inline T& GetComponent(EntityID entity);

		// Notify each component collection that an entity has been destroyed.
		// If the collection has a component for that entity, it will remove it.
		inline void EntityDestroyed(EntityID entity);

	private:
		// Map from type string pointer to a component type
		std::unordered_map<const char*, CompType> m_ComponentTypes;

		// Map from type string pointer to a component array
		std::unordered_map<const char*, s_ptr<ICrammedVecMap>> m_ComponentCollections;

		// The component type to be assigned to the next registered component - starting at 0
		CompType m_NextComponentType;

		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template<typename T>
		s_ptr<CrammedVecMap<T>> GetComponentCollection();
	};
}