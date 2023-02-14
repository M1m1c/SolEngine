#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "GalaxyDraw/GalaxyMacros.h"
#include "GalaxyDraw/SceneCamera.h"
#include <GalaxyDraw/Interfaces/Texture.h>
#include <GalaxyDraw/Interfaces/Model.h>
#include "Sol/SolDefines.h"
#include "Sol/Scene/Components/TransformComp.h"

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
		s_ptr<GalaxyDraw::Model> Model;
		std::string ModelPath;
		std::string TexturePath;

		ModelComp() = default;
		ModelComp(const ModelComp&) = default;
		//Sets up model based on providde file path
		ModelComp(const std::string& modelPath, EntityID entityID) : ModelPath(modelPath)
		{
			Model = GalaxyDraw::Model::Create(modelPath,entityID);
		}

		//TODO when we create a model, it should be using the path provided and setup all values in m_Model,
		// later in the scene when we are updating we should get all modelComps that are set to be visible
		// and draw them using a static function in Renderer DrawModel().
		// look at ModelOld and MeshOld for reference of how it used to be done.
	};

	//TODO make this use and load textures, also every entity with a ModelComp should probably have a material comp
	struct MaterialComp
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		MaterialComp() = default;
		MaterialComp(const MaterialComp&) = default;
		
	};
}