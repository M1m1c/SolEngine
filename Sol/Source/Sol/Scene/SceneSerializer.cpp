#include "solpch.h"
#include "SceneSerializer.h"
#include "Entity.h"

#include "GalaxyDraw/Interfaces/MaterialData.h"

#include <yaml-cpp/yaml.h>


namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3) { return false; }

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};


	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4) { return false; }

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

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
		out << YAML::Key << "Entity" << YAML::Value << (uint64_t)entity.GetID();

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
			auto& materialComp = entity.GetComponent<MaterialComp>();
			auto& color = materialComp.Properties.Color;
			out << YAML::Key << "Color" << YAML::Value << color;
			out << YAML::Key << "MaterialIndex" << YAML::Value << materialComp.GetMaterialIndex();
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	static void SerializeMaterial(YAML::Emitter& out, uint32_t matIndex, s_ptr <GD_::MaterialData > matData)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Material" << YAML::Value << matIndex;
		out << YAML::Key << "Name" << YAML::Value << matData->Name;
		out << YAML::Key << "DiffuseTexture" << YAML::Value << matData->BaseTexturePath;
		out << YAML::EndMap;
	}

	SceneSerializer::SceneSerializer(const s_ptr<Scene>& scene) : m_Scene(scene)
	{
	}

	void SceneSerializer::SerializeToText(const std::string& filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "Materials" << YAML::Value << YAML::BeginSeq;
		auto& materials = GD_Renderer3D::GetMaterials();
		for (size_t i = 0; i < materials.size(); i++)
		{
			if (i == 0) { continue; }
			SerializeMaterial(out, i, materials[i]);
		}
		out << YAML::EndSeq;

		out << YAML::Key << "Scene" << YAML::Value << "Untitled Scene";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->GetRegistry().each([&](auto entityID)
			{

				Entity entity = { entityID,m_Scene.get() };
				if (!entity) { return; }
				if (!entity.HasComponent<InternalComp>()) //TODO might want to change this later so that camera transform gets stored
				{
					SerializeEntity(out, entity);
				}
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
		std::ifstream stream(filePath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		//TODO make sure that we load materials present in scene
		//TODO call createMaterial on Renderer3D and make sure it is in the correct order the amterials were created in

		YAML::Node data = YAML::Load(strStream.str());

		if (data["Materials"])
		{
			auto materials = data["Materials"];

			for (size_t i = 0; i < materials.size(); i++)
			{
				auto mat = materials[i];

				uint32_t matIndex = 0;
				std::string name = "";
				std::string diffuseTexture = "";

				auto indexField = mat["Material"];
				if (indexField)
				{
					matIndex = indexField.as<uint32_t>();
				}

				auto nameField = mat["Name"];
				if (nameField)
				{
					name = nameField.as<std::string>();
				}

				auto difTexField = mat["DiffuseTexture"];
				if (true)
				{
					diffuseTexture = difTexField.as<std::string>();
				}
				SOL_CORE_TRACE("Loading Material: Index = {0}, Name = {1}", matIndex, name);
				GD_Renderer3D::CreateNewMaterial(diffuseTexture, name);
			}
		}


		if (!data["Scene"]) { return false; }

		std::string sceneName = data["Scene"].as<std::string>();
		SOL_CORE_TRACE("Loading scene |{0}|", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t id = entity["Entity"].as<uint64_t>();

				std::string name;
				auto nameComp = entity["NameComp"];
				if (nameComp)
				{
					name = nameComp["Name"].as<std::string>();
				}

				SOL_CORE_TRACE("Loading entity: ID = {0}, Name = {1}", id, name);

				Entity loadedEntity = m_Scene->CreateEntity(name);//TODO add so we can set id on creation

				auto transformComp = entity["TransformComp"];
				if (transformComp)
				{
					auto& transform = loadedEntity.GetComponent<TransformComp>();
					transform.Position = transformComp["Position"].as<glm::vec3>();
					transform.Rotation = transformComp["Rotation"].as<glm::vec3>();
					transform.Scale = transformComp["Scale"].as<glm::vec3>();
				}

				auto materialComp = entity["MaterialComp"];
				uint32_t matIndex = 0;
				if (materialComp)
				{
					matIndex = materialComp["MaterialIndex"].as<uint32_t>();
					auto& material = loadedEntity.AddComponent<MaterialComp>(matIndex);
					material.Properties.Color = materialComp["Color"].as<glm::vec4>();
				}

				auto modelComp = entity["ModelComp"];
				if (modelComp)
				{
					auto path = modelComp["ModelPath"].as<std::string>();
					auto& model = loadedEntity.AddComponent<ModelComp>(path, loadedEntity.GetID(), matIndex);
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeBinary(const std::string& filePath)
	{
		SOL_CORE_ASSERT(false, "Not Implemented");
		return false;
	}

}