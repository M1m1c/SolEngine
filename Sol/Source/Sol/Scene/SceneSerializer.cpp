#include "solpch.h"
#include "SceneSerializer.h"
#include "Entity.h"

#include <yaml-cpp/yaml.h>

namespace Sol
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity.GetID();

		if (entity.HasComponent<NameComp>())
		{
			out << YAML::Key << "NameComp";
			out << YAML::BeginMap;
			auto& name = entity.GetComponent<NameComp>().GetName();
			out << YAML::Key << "Name" << YAML::Value << name;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComp>())
		{
			out << YAML::Key << "TransformComp";
			out << YAML::BeginMap;
			auto& transform = entity.GetComponent<TransformComp>();
			out << YAML::Key << "Position" << YAML::Value << transform.Position;
			out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transform.Scale;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<ModelComp>())
		{
			out << YAML::Key << "ModelComp";
			out << YAML::BeginMap;
			auto& modelpath = entity.GetComponent<ModelComp>().ModelPath;
			out << YAML::Key << "ModelPath" << YAML::Value << modelpath;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<MaterialComp>())
		{
			out << YAML::Key << "MaterialComp";
			out << YAML::BeginMap;
			auto& color = entity.GetComponent<MaterialComp>().Color;
			out << YAML::Key << "Color" << YAML::Value << color;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	SceneSerializer::SceneSerializer(const s_ptr<Scene>& scene) : m_Scene(scene)
	{
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