#pragma once
#include "GalaxyDraw/Interfaces/ParticleSystem.h"

namespace GalaxyDraw
{

	class OpenGL_ParticleSystem : public ParticleSystem
	{
	public:
		OpenGL_ParticleSystem(const std::string& vertexFile, const std::string& fragmentFile);


		virtual ~OpenGL_ParticleSystem() override;

		virtual void OnUpdate(Sol::TimeStep deltaTime)override;
		virtual void OnRender(Camera& camera) override;
		virtual void Emit(const ParticleProperties& properties) override;
	private:
		std::vector<Particle> m_ParticlePool;
		uint32_t m_PoolIndex = 999;

		std::string m_VertexCode;
		std::string m_FragmentCode;

		uint32_t m_QuadVA = 0;
		
		std::shared_ptr<Shader> m_ParticleShader;
		int m_ParticleShaderViewProj;
		int m_ParticleShaderTransform;
		int m_ParticleShaderColor;

	};


}