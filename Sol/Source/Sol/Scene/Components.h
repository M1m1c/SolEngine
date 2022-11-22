#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "GalaxyDraw/GalaxyMacros.h"
#include "GalaxyDraw/SceneCamera.h"
#include <GalaxyDraw/Interfaces/Texture.h>

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

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Position)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		operator glm::mat4& () {
			return GetTransform();
		}

		operator const glm::mat4& () const {
			return GetTransform();
		}


		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

	};

	struct SpriteRendererComp
	{
	public:

		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		s_ptr<GD_::Texture2D> Texture;
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
}