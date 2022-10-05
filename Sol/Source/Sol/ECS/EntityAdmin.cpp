#include "solpch.h"
#include "EntityAdmin.h"

namespace Sol 
{
	EntityAdmin::EntityAdmin() : 
		m_ActiveEntityCount(0),
		m_EntityCollection(MAX_ENTITIES)
	{
		// Initialize the queue with all possible entity IDs
		for (EntityID entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			m_AvailableEntityIDs.push(entity);
		}
	}

	EntityAdmin::~EntityAdmin()
	{
		//TODO make sure this frees up the memory m_CompSignatures uses
		//m_CompSignatures.clear();
	}

	const Entity& EntityAdmin::CreateEntity()
	{
		assert(m_ActiveEntityCount < MAX_ENTITIES && "Could not create new entity, too many entities in existence.");
		EntityID id = m_AvailableEntityIDs.front();
		m_AvailableEntityIDs.pop();

		m_EntityCollection.Add(id, Entity(id));

		m_ActiveEntityCount++;

		return id;
	}

	void EntityAdmin::DestroyEntity(const EntityID& entity)
	{
		assert(entity < MAX_ENTITIES && "Invalid Entity! Entity out of range.");
		m_CompSignatures[entity].reset();
		m_AvailableEntityIDs.push(entity);
		m_ActiveEntityCount--;
	}

	void EntityAdmin::SetCompSignature(const EntityID& entity, const CompSignature& signature)
	{
		assert(entity < MAX_ENTITIES && "Invalid Entity! Entity out of range.");
		m_CompSignatures[entity] = signature;
	}

	const CompSignature& EntityAdmin::GetCompSignature(const EntityID& entity)
	{
		assert(entity < MAX_ENTITIES && "Invalid Entity! Entity out of range.");
		return m_CompSignatures[entity];
	}

}