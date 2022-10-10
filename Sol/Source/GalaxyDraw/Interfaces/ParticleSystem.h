#pragma once
#include "Sol/Core/Core.h"
#include "glm/glm.hpp"
#include <Sol/Core/TimeStep.h>
#include "Camera.h"
#include "Shader.h"

namespace GalaxyDraw
{
	struct ParticleProperties
	{
		glm::vec3 Position;
		glm::vec3 Velocity;
		glm::vec3 VelocityVariation;
		glm::vec4 ColorBegin;
		glm::vec4 ColorEnd;
		float SizeBegin;
		float SizeEnd;
		float SizeVariation;
		float LifeTime = 1.0f;
	};

	struct Particle
	{
		glm::vec3 Position;
		glm::vec3 Velocity;
		glm::vec3 Rotation;
		glm::vec4 ColorBegin;
		glm::vec4 ColorEnd;
		float SizeBegin;
		float SizeEnd;
		float LifeTime = 1.f;
		float LifeRemaining = 0.f;
		bool Active = false;
	};

	class ParticleSystem
	{
	public:
		//ParticleSystem();
		static std::shared_ptr<ParticleSystem> Create(const std::string& vertexFile, const std::string& fragmentFile);

		virtual ~ParticleSystem() = default;

		virtual void OnUpdate(Sol::TimeStep deltaTime) = 0;
		virtual void OnRender(Camera& camera) = 0;
		virtual void Emit(const ParticleProperties& properties) = 0;
	private:



		//std::vector<Particle> m_ParticlePool;
		//uint32_t m_ParticleIndex = 999;

		//uint32_t m_QuadVA = 0;
		//std::unique_ptr<Shader> m_ParticleShader;
		//int m_ParticleShaderViewProj;
		//int m_ParticleShaderTransform;
		//int m_ParticleShaderColor;

	};


}