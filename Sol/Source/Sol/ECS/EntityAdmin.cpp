#include "solpch.h"
#include "EntityAdmin.h"

namespace Sol 
{
	EntityAdmin::EntityAdmin()
	{
		// Initialize the queue with all possible entity IDs
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			m_AvailableEntityIDs.push(entity);
		}
	}

	EntityAdmin::~EntityAdmin()
	{
	}

	const Entity& EntityAdmin::CreateEntity()
	{
		assert(m_ActiveEntityCount < MAX_ENTITIES && "Could not create new entity, too many entities in existence.");

		// Take an ID from the front of the queue
		Entity id = m_AvailableEntityIDs.front();
		m_AvailableEntityIDs.pop();
		++m_ActiveEntityCount;

		return id;
	}

	void EntityAdmin::DestroyEntity(const Entity& entity)
	{
		assert(entity < MAX_ENTITIES && "Invalid Entity! Entity out of range.");

		// Invalidate the destroyed entity's signature
		m_CompSignatures[entity].reset();

		// Put the destroyed ID at the back of the queue
		m_AvailableEntityIDs.push(entity);
		--m_ActiveEntityCount;
	}

	void EntityAdmin::SetCompSignature(const Entity& entity, const CompSignature& signature)
	{
		assert(entity < MAX_ENTITIES && "Invalid Entity! Entity out of range.");

		// Put this entity's signature into the array
		m_CompSignatures[entity] = signature;
	}

	const CompSignature& EntityAdmin::GetCompSignature(const Entity& entity)
	{
		assert(entity < MAX_ENTITIES && "Invalid Entity! Entity out of range.");

		// Get this entity's signature from the array
		return m_CompSignatures[entity];
	}

}