#include "solpch.h"
#include "Entity.h"
#include "Components.h"

namespace Sol {
	Entity::Entity(entt::entity id, Scene* scene) : 
		m_EntityID(id),
		m_Scene(scene)
	{
	}
	const std::string& Entity::GetName()
	{
		SOL_CORE_ASSERT(HasComponent<NameComp>(), "Entity does not have a name component!");
		return GetComponent<NameComp>();
	}
}