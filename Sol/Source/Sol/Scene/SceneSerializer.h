#pragma once
#include "Scene.h"

namespace Sol {

	class SceneSerializer
	{
	public:
		SceneSerializer(const s_ptr<Scene>& scene);
		~SceneSerializer() = default;

		void SerializeToText(const std::string& filePath);
		void SerializeToBinary(const std::string& filePath);

		bool DeserializeText(const std::string& filePath);
		bool DeserializeBinary(const std::string& filePath);
	private:
		s_ptr<Scene> m_Scene;
	};
}