#pragma once
#include <entt.hpp>
#include <Sol/Core/TimeStep.h>
#include "GalaxyDraw/GalaxyDraw.h"

namespace Sol
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdateEditor(TimeStep deltaTime);
		void OnUpdateRuntime(TimeStep deltaTime);

		void OnViewportResize(uint32_t width, uint32_t height);

		//Creates an entity with a TransformComp and a TagComp in the scene, returns created entity.
		Entity CreateEntity(const std::string& name = std::string());

		void DestroyEntity(EntityID entityID);

		void DestroyAllEntities();

		template<typename T>
		void RemoveComponent(EntityID entityID)
		{
			if constexpr (std::is_same<T, ModelComp>::value || std::is_same<T, MaterialComp>::value)
			{
				if (m_Registry.all_of<ModelComp, MaterialComp>(entityID))
				{
					auto& modelComp = m_Registry.get<ModelComp>(entityID);
					auto& materialComp = m_Registry.get<MaterialComp>(entityID);

					GD_Renderer3D::DiscardEntityRenderData(entityID);

					m_Registry.remove<ModelComp>(entityID);
					m_Registry.remove<MaterialComp>(entityID);
					return;
				}
			}
			m_Registry.remove<T>(entityID);
		}
		//TODO check if it is okay to not have this return const
		entt::registry& GetRegistry() { return m_Registry; }

	private:

		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;

		friend class Entity;

		GD_ShaderLibrary m_ShaderLib;
		s_ptr<GD_Shader> m_Shader;
		s_ptr<GD_VAO> m_VertexArray;
		s_ptr<GD_Texture2D> m_Texture;
	};
}

