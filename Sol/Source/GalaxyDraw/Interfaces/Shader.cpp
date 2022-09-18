#include "solpch.h"

#include "Shader.h"
#include "Renderer.h"
#include "GalaxyDraw/Platform/OpenGL/OpenGL_Shader.h"

namespace GalaxyDraw 
{
	std::shared_ptr <Shader> Shader::Create(const std::string& vertexFile, const std::string& fragmentFile, const std::string& nameToSet)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SOL_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL: 
			if (nameToSet!="")
			{
				return std::make_shared<OpenGL_Shader>(nameToSet,vertexFile, fragmentFile);
			}
			else
			{
				return std::make_shared<OpenGL_Shader>(vertexFile, fragmentFile);
			}

			break;
		}
		SOL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


	void ShaderLibrary::Add(const std::string& nameToSet, const std::shared_ptr<Shader>& shader)
	{
		SOL_CORE_ASSERT(!Exists(nameToSet), "Shader already exists!");
		m_Shaders[nameToSet] = shader;
	}

	void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& vertexFilePath, const std::string& fragmentFilePath)
	{
		auto shader = Shader::Create(vertexFilePath, fragmentFilePath);
		Add(shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& nameToSet, const std::string& vertexFilePath, const std::string& fragmentFilePath)
	{
		auto shader = Shader::Create(vertexFilePath, fragmentFilePath, nameToSet);
		Add(nameToSet,shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name)
	{
		SOL_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

}