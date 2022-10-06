#include "solpch.h"
#include "ECS_Admin.h"

namespace Sol
{
	void ECS_Admin::Init()
	{
		m_ComponentAdmin = std::make_unique<ComponentAdmin>();
		m_EntityAdmin = std::make_unique<EntityAdmin>();
		m_SystemAdmin = std::make_unique<SystemAdmin>();
	}

	Entity ECS_Admin::CreateEntity()
	{
		return m_EntityAdmin->CreateEntity();
	}

	void ECS_Admin::DestroyEntity(EntityID entity)
	{
		m_EntityAdmin->DestroyEntity(entity);
		m_DestroyedEntities.push(entity);
	}

	void ECS_Admin::RemoveDestroyedEntities()
	{
		while (!m_DestroyedEntities.empty())
		{
			auto id = m_DestroyedEntities.front();
			EntityDestroyed(id);
			m_DestroyedEntities.pop();
		}
	}

	void ECS_Admin::EntityDestroyed(EntityID entity)
	{

		m_EntityAdmin->EntityDestroyed(entity);

		m_ComponentAdmin->EntityDestroyed(entity);

		m_SystemAdmin->EntityDestroyed(entity);

	}

	template<typename T>
	void ECS_Admin::RegisterComponent()
	{
		m_ComponentAdmin->RegisterComponent<T>();
	}

	template<typename T>
	void ECS_Admin::AddComponent(Entity entity, T component)
	{
		m_ComponentAdmin->AddComponent<T>(entity, component);
		auto signature = m_EntityAdmin->GetCompSignature(entity);
		signature.set(m_ComponentAdmin->GetComponentType<T>(), true);
		m_EntityAdmin->SetCompSignature(entity, signature);
		m_SystemAdmin->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void ECS_Admin::RemoveComponent(Entity entity)
	{
		m_ComponentAdmin->RemoveComponent<T>(entity);
		auto signature = m_EntityAdmin->GetCompSignature(entity);
		signature.set(m_ComponentAdmin->GetComponentType<T>(), false);
		m_EntityAdmin->SetCompSignature(entity, signature);
		m_SystemAdmin->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& ECS_Admin::GetComponent(Entity entity)
	{
		return m_ComponentAdmin->GetComponent<T>(entity);
	}

	template<typename T>
	CompType ECS_Admin::GetComponentType()
	{
		return m_ComponentAdmin->GetComponentType<T>();
	}

	template<typename T>
	s_ptr<T> ECS_Admin::RegisterSystem()
	{
		return m_SystemAdmin->RegisterSystem<T>();
	}

	template<typename T>
	void ECS_Admin::SetSystemSignature(CompSignature signature)
	{
		m_SystemAdmin->SetSignature<T>(signature);
	}

}