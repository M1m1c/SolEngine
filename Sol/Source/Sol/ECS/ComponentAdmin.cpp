#include "solpch.h"
#include "ComponentAdmin.h"

namespace Sol 
{
	template<typename T>
	void ComponentAdmin::RegisterComponent()
	{
		const char* typeName = typeid(T).name();

		assert(m_ComponentTypes.find(typeName) == m_ComponentTypes.end() && "Registering component type more than once.");

		// Add this component type to the component type map
		m_ComponentTypes.insert({ typeName, m_NextComponentType });

		// Create a ComponentArray pointer and add it to the component arrays map
		m_ComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

		// Increment the value so that the next component registered will be different
		++m_NextComponentType;
	}

	template<typename T>
	CompType ComponentAdmin::GetComponentType()
	{
		const char* typeName = typeid(T).name();

		assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered before use.");

		// Return this component's type - used for creating signatures
		return m_ComponentTypes[typeName];
	}

	template<typename T>
	void ComponentAdmin::AddComponent(Entity entity, T component)
	{
		// Add a component to the array for an entity
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<typename T>
	void ComponentAdmin::RemoveComponent(Entity entity)
	{
		// Remove a component from the array for an entity
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<typename T>
	T& ComponentAdmin::GetComponent(Entity entity)
	{
		// Get a reference to a component from the array for an entity
		return GetComponentArray<T>()->GetData(entity);
	}

	void ComponentAdmin::EntityDestroyed(Entity entity)
	{
		// Notify each component array that an entity has been destroyed
		// If it has a component for that entity, it will remove it
		for (auto const& pair : m_ComponentArrays)
		{
			auto const& component = pair.second;

			component->DeleteKey(entity);
		}
	}

	template<typename T>
	std::shared_ptr<CrammedVecMap<T>> ComponentAdmin::GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
	}
}