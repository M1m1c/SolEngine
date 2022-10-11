#include "Sol.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Sol::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_CameraController(10, 10, glm::vec2(1.6f, 0.9f), glm::vec3(0.f))
	{
		m_VertexArray = GD_VAO::Create();

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.f, 0.f,
			0.5f, -0.5f, 0.0f, 1.f, 0.f,
			0.5f, 0.5f, 0.0f, 1.f, 1.f,
			-0.5f, 0.5f, 0.0f, 0.f, 1.f,
		};

		auto vertexBuffer = GD_VBO::Create(squareVertices, sizeof(squareVertices));

		GD_BufferLayout layout =
		{
			{GD_ShaderDataType::Float3, "a_Position"},
			{GD_ShaderDataType::Float2, "a_TexCoord"},
			/*	{GD_ShaderDataType::Float3, "a_Normal"},
				{GD_ShaderDataType::Float4, "a_Color"},*/
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0,1,2,2,3,0 };
		auto indexBuffer = GD_EBO::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		auto shader = m_ShaderLib.Load("Square", "Square.vert", "Square.frag");

		m_Texture = GD_Texture2D::Create("assets/textures/think.png");

		m_WhiteTexture = GD_Texture2D::Create(1, 1);
		uint32_t whiteColor = 0Xffffffff;
		m_WhiteTexture->SetData(&whiteColor, sizeof(uint32_t));

		shader->Bind();
		shader->setInt("u_Texture", 0);

	}

	void OnUpdate(Sol::TimeStep deltaTime) override
	{
		SOL_PROFILE_FUNCTION();
		m_Framebuffer->Bind();
		//UPDATE STEP
		m_CameraController.OnUpdate(deltaTime);

		//Sol::Timer timer("ExampleLayer::SandboxApp::OnUpdate");

		//RENDER STEP
		GD_RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		GD_RenderCommand::Clear();

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		GD_Renderer::BeginScene(m_CameraController.GetCamera());

		//m_Shader->setVec3("u_Color", m_TriangleColor);

	/*	for (int x = -5; x < 5; x++)
		{
			for (int y = -5; y < 5; y++)
			{
				glm::vec3 pos(x * 0.12f, y * 0.12f, 0.f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				GD_Renderer::Submit(m_Shader, m_VertexArray, transform);
			}
		}*/

		{
			SOL_PROFILE_SCOPE("RenderDraw");
			glm::vec3 pos(0.f, 0.f, 0.f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos);

			auto shader = m_ShaderLib.Get("Square");

			GD_Renderer::Submit(shader, m_VertexArray, transform);

			m_Texture->Bind();
		}

		GD_Renderer::EndScene();
		m_Framebuffer->UnBind();
	}

	void OnFixedUpdate(Sol::TimeStep fixedStep, const float fixedTime) override
	{
	}
	//TODO move into editor project
	virtual void OnImGuiRender() override
	{
		
		static bool dockSpaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) { Sol::Application::Get().Close(); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		//All windows and tabs need to be here___________________________________________
		{
			ImGui::Begin("ViewPort");
			//ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_TriangleColor));
			uint32_t textureID = m_Framebuffer->GetColorAttachmentsRendererID();
			ImGui::Image((void*)textureID, ImVec2{ 1280,720 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
			ImGui::End();

		}
		
		//All windows and tabs need to be here___________________________________________
		ImGui::End();
	}

	virtual void OnEvent(Sol::Event& e) override
	{
		//SOL_TRACE("{0}", event);
		m_CameraController.OnEvent(e);
	}

	virtual void OnAttach() override 
	{
		GD_FramebufferProps properties;
		properties.Width = 1280;
		properties.Height = 720;
		m_Framebuffer = GD_::Framebuffer::Create(properties);
	}

private:

	GD_ShaderLibrary m_ShaderLib;
	Sol::s_ptr<GD_Shader> m_Shader;
	Sol::s_ptr<GD_VAO> m_VertexArray;
	Sol::s_ptr<GD_Texture2D> m_Texture;
	Sol::s_ptr<GD_Texture2D> m_WhiteTexture;

	Sol::u_ptr<GD_Framebuffer> m_Framebuffer;

	glm::vec3 m_TrianglePos = glm::vec3(0.f);

	Sol::CameraController m_CameraController;
	//glm::vec3 m_CameraPosition = glm::vec3(0.f);
	//float m_CameraSpeed = 1.f;

	glm::vec3 m_TriangleColor = { 0.f, 0.8f, 0.8f };
};

class Sandbox : public Sol::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {}
};

Sol::Application* Sol::CreateApplication()
{
	return new Sandbox();
}

//int main() 
//{
//	auto sandbox = new Sandbox();
//	sandbox->Run();
//	delete sandbox;
//	return 0;
//}