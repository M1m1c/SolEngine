#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "GalaxyDraw/GalaxyMacros.h"
#include "GalaxyDraw/SceneCamera.h"
#include <GalaxyDraw/Interfaces/Texture.h>
#include <GalaxyDraw/Interfaces/Model.h>

namespace Sol
{
	struct NameComp
	{
		NameComp() = default;
		NameComp(const NameComp&) = default;
		NameComp(const std::string& name) :m_Name(name) {}
		~NameComp() = default;

		operator std::string& () { return m_Name; }
		operator const std::string& () const { return m_Name; }
	private:
		std::string m_Name;
	};

	struct TransformComp
	{
	public:

		TransformComp() = default;
		TransformComp(const TransformComp&) = default;
		TransformComp(const glm::vec3& position) : Position(position) { }
		~TransformComp() = default;

		glm::mat4 GetTransformMatrix() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Position)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
		
		operator const glm::mat4 () const {
			return GetTransformMatrix();
		}


		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

	};

	struct SpriteRendererComp
	{
	public:

		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		s_ptr<GalaxyDraw::Texture2D> Texture;
		float TilingFactor = 1.0f;

		SpriteRendererComp() = default;
		SpriteRendererComp(const SpriteRendererComp&) = default;
		SpriteRendererComp(const glm::vec4 & color)
			: Color(color) {}

	private:
		
	};

	struct CameraComp
	{
		GalaxyDraw::SceneCamera OrthoCamera;
		bool IsPirmary = true;
		bool IsFixedAspectRatio = false;

		CameraComp() = default;
		CameraComp(const CameraComp&) = default;
		//CameraComp(const glm::mat4 & projection) : Camera(projection) { }
		~CameraComp() = default;
	};

	struct ModelComp 
	{

		std::string ModelPath;
		std::string TexturePath;

		ModelComp() = default;
		ModelComp(const ModelComp&) = default;
		ModelComp(const std::string& modelPath) : ModelPath(modelPath) {}

		//TODO when we create a model, it should it using the path provided and setup all values in m_Model,
		// later in the scene when we are updating we should get all modelComps that are set to be visible
		// and draw them using a static fucniton in Renderer DrawModel().
		// look at ModelOld and MeshOld for reference of how it used to be done.

	private:
		s_ptr<GalaxyDraw::Model> m_Model;
	};
}