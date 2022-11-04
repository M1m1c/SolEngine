#pragma once
#include "Scene.h"

#include "entt.hpp"

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

		bool operator==(const Entity& rhs) { return m_EntityID == rhs.m_EntityID; }

		operator bool() const { return m_EntityID != entt::null; }

	private:
		entt::entity m_EntityID{ entt::null };
		Scene* m_Scene = nullptr;
	};


}