#include "solpch.h"
#include "SceneSerializer.h"
#include "Entity.h"

#include <yaml-cpp/yaml.h>

namespace Sol
{
	SceneSerializer::SceneSerializer(const s_ptr<Scene>& scene) : m_Scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity) 
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "1234"; //EntityID goes in value

		if (entity.HasComponent<NameComp>())
		{
			out << YAML::Key << "NameComp";
			out << YAML::BeginMap;
			auto& name = entity.GetComponent<NameComp>().GetName();
			out << YAML::Key << "Name" << YAML::Value << name;
			out << YAML::EndMap;
		}


		out << YAML::EndMap;
	}

	void SceneSerializer::SerializeToText(const std::string& filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled Scene";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->GetRegistry().each([&](auto entityID)
			{
				
				Entity entity = { entityID,m_Scene.get() };
				if (!entity) { return; }
				SerializeEntity(out, entity);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;
		std::ofstream fOut(filePath);
		fOut << out.c_str();
	}

	void SceneSerializer::SerializeToBinary(const std::string& filePath)
	{
		SOL_CORE_ASSERT(false, "Not Implemented");
	}

	bool SceneSerializer::DeserializeText(const std::string& filePath)
	{
		return false;
	}

	bool SceneSerializer::DeserializeBinary(const std::string& filePath)
	{
		SOL_CORE_ASSERT(false, "Not Implemented");
		return false;
	}

}