#pragma once
#include "ECS_Core.h"
#include <queue>
namespace Sol 
{
	class EntityAdmin
	{
	public:
		EntityAdmin();
		~EntityAdmin();

		inline const Entity& CreateEntity();
		inline void DestroyEntity(const Entity& entity);

		//Sets what components an active entity has.
		inline void SetCompSignature(const Entity& entity, const CompSignature& signature);
		inline const CompSignature& GetCompSignature(const Entity& entity);

	private:
		
		//Reperesents the enteties that are active in the scene.
		std::queue<Entity> m_AvailableEntityIDs;

		//Keeps track of what components the active entities have.
		std::array<CompSignature, MAX_ENTITIES> m_CompSignatures;
		//std::unordered_map<Entity, CompSignature> m_CompSignatures;

		uint32_t m_ActiveEntityCount;
	};
}