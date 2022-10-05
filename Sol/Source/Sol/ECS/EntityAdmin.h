#pragma once
#include "ECS_Core.h"
#include "Entity.h"
#include "Sol/Core/CrammedVecMap.h"
#include <queue>
namespace Sol 
{
	class EntityAdmin
	{
	public:
		EntityAdmin();
		~EntityAdmin();

		inline const Entity& CreateEntity();
		inline void DestroyEntity(const EntityID& entity);

		//Sets what components an active entity has.
		inline void SetCompSignature(const EntityID& entity, const CompSignature& signature);
		inline const CompSignature& GetCompSignature(const EntityID& entity);

	private:
		
		//Reperesents the enteties that are active in the scene.
		std::queue<EntityID> m_AvailableEntityIDs;

		CrammedVecMap<Entity> m_EntityCollection;

		//Keeps track of what components the active entities have.
		std::array<CompSignature, MAX_ENTITIES> m_CompSignatures;

		uint32_t m_ActiveEntityCount;
	};
}