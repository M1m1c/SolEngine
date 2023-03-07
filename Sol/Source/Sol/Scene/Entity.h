#pragma once
#include "Scene.h"

#include "entt.hpp"
#include "Sol/SolDefines.h"

namespace Sol
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity id, Scene* scene);
		Entity(const Entity& other) = default;
		~Entity() = default;

		template<typename T,typename... Args>
		T& AddComponent(Args&&... args) {
			SOL_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityID,std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent() {
			SOL_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityID);
		}

		template<typename T>
		bool HasComponent() {
			return m_Scene->m_Registry.any_of<T>(m_EntityID);
		}

		template<typename T>
		void RemoveComponent() {
			SOL_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityID);
		}

		const std::string& GetName();
		const EntityID& GetID() { return m_EntityID; }

		bool operator==(const Entity& other) { return m_EntityID == other.m_EntityID && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) { return !operator==(other); }

		operator bool() const { return m_EntityID != entt::null; }

		operator uint32_t() const { return (uint32_t)m_EntityID; }

		operator entt::entity() const { return m_EntityID; }

	private:
		EntityID m_EntityID{ entt::null };
		Scene* m_Scene = nullptr;
	};


}