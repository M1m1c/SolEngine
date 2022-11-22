#include "solpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Sol/Scene/Components.h"
#include "GalaxyDraw/GalaxyDraw.h"

namespace Sol
{
	Scene::Scene()
	{
		//TODO figure out way to solve indecies issue and remove the content here and the related memeber variables
		//m_VertexArray = GD_VAO::Create();

		//float squareVertices[5 * 4] = {
		//	-0.5f, -0.5f, 0.0f, 0.f, 0.f,
		//	0.5f, -0.5f, 0.0f, 1.f, 0.f,
		//	0.5f, 0.5f, 0.0f, 1.f, 1.f,
		//	-0.5f, 0.5f, 0.0f, 0.f, 1.f,
		//};

		//auto vertexBuffer = GD_VBO::Create(squareVertices, sizeof(squareVertices));

		//GD_BufferLayout layout =
		//{
		//	{GD_ShaderDataType::Float3, "a_Position"},
		//	{GD_ShaderDataType::Float2, "a_TexCoord"},
		//	/*	{GD_ShaderDataType::Float3, "a_Normal"},
		//		{GD_ShaderDataType::Float4, "a_Color"},*/
		//};

		//vertexBuffer->SetLayout(layout);
		//m_VertexArray->AddVertexBuffer(vertexBuffer);

		//uint32_t indices[6] = { 0,1,2,2,3,0 };
		//auto indexBuffer = GD_EBO::Create(indices, sizeof(indices) / sizeof(uint32_t));
		//m_VertexArray->SetIndexBuffer(indexBuffer);

	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(TimeStep deltaTime)
	{

		GD_Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = m_Registry.view<TransformComp, CameraComp>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComp, CameraComp>(entity);

				if (camera.IsPirmary)
				{
					mainCamera = &camera.OrthoCamera;
					cameraTransform = &transform.GetTransform();
					break;
				}
			}
		}


		if (mainCamera)
		{
			GD_Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto group = m_Registry.group<TransformComp>(entt::get<SpriteRendererComp>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComp, SpriteRendererComp>(entity);

				GD_Renderer2D::DrawQuad(transform, sprite.Color);
			}

			GD_Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComp>();
		for (auto entity : view)
		{
			auto& camComp = view.get<CameraComp>(entity);

			if (!camComp.IsFixedAspectRatio)
			{
				camComp.OrthoCamera.SetViewportSize(width, height);
			}

		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		auto entity = Entity{ m_Registry.create(), this };
		entity.AddComponent<TransformComp>();
		entity.AddComponent<NameComp>(name == "" ? "Entity" : name);
		return entity;
	}

}