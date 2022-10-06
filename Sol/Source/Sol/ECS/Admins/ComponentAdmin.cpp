#include "solpch.h"
#include "ComponentAdmin.h"

namespace Sol 
{
	template<typename T>
	void ComponentAdmin::RegisterComponent()
	{
		const char* typeName = typeid(T).name();
		assert(m_ComponentTypes.find(typeName) == m_ComponentTypes.end() && "Attempted to register component type more than once.");
		m_ComponentTypes.insert({ typeName, m_NextComponentType });
		m_ComponentMapss.insert({ typeName, std::make_shared<ComponentArray<T>>() });
		++m_NextComponentType;
	}

	template<typename T>
	CompType ComponentAdmin::GetComponentType()
	{
		const char* typeName = typeid(T).name();
		assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered before use.");
		return m_ComponentTypes[typeName];
	}

	template<typename T>
	void ComponentAdmin::AddComponent(EntityID entity, T component)
	{
		GetComponentCollection<T>()->Add(entity, component);
	}

	template<typename T>
	void ComponentAdmin::RemoveComponent(EntityID entity)
	{
		GetComponentCollection<T>()->Remove(entity);
	}

	template<typename T>
	T& ComponentAdmin::GetComponent(EntityID entity)
	{
		return GetComponentCollection<T>()->Get(entity);
	}

	void ComponentAdmin::EntityDestroyed(EntityID entity)
	{
		for (auto const& pair : m_ComponentCollections)
		{
			auto const& component = pair.second;

			component->DeleteKey(entity);
		}
	}

	template<typename T>
	s_ptr<CrammedVecMap<T>> ComponentAdmin::GetComponentCollection()
	{
		const char* typeName = typeid(T).name();
		assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered before use.");
		return std::static_pointer_cast<CrammedVecMap<T>>(m_ComponentCollections[typeName]);
	}
}