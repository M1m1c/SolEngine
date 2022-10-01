#pragma once
#include "ECS_Core.h"
namespace Sol 
{

	class IComponentContainer
	{
	public:
		virtual ~IComponentContainer() = default;
		virtual void EntityDestroyed(Entity entity) = 0;
	};

	template<typename T>
	class ComponentContainer : IComponentContainer
	{
	public:
		void InsertData(Entity entity, T component);
		void RemoveData(Entity entity);
		T& GetData(Entity entity);
		virtual void EntityDestroyed(Entity entity) override;

	private:
		// The packed array of components (of generic type T),
		// set to a specified maximum amount, matching the maximum number
		// of entities allowed to exist simultaneously, so that each entity
		// has a unique spot.
		std::array<T, MAX_ENTITIES> m_ComponentArray;

		// Map from an entity ID to an array index.
		std::unordered_map<Entity, size_t> m_EntityToIndexMap;

		// Map from an array index to an entity ID.
		std::unordered_map<size_t, Entity> m_IndexToEntityMap;

		// Total size of valid entries in the array.
		size_t m_Size;
	};



}