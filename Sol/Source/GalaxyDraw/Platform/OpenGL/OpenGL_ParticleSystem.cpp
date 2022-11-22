#include "solpch.h"
#include "OpenGL_ParticleSystem.h"
#include "GalaxyDraw/Interfaces/GalaxyMacros.h"
#include "Sol/Core/Random.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

namespace GalaxyDraw 
{
	OpenGL_ParticleSystem::OpenGL_ParticleSystem(const std::string& vertexFile, const std::string& fragmentFile, uint32_t maxParticles) :
		m_PoolIndex(maxParticles)
	{
		m_ParticlePool.resize(maxParticles);
		m_VertexCode = GalaxyMacros::get_file_contents(vertexFile.c_str());
		m_FragmentCode = GalaxyMacros::get_file_contents(fragmentFile.c_str());
	}
	OpenGL_ParticleSystem::~OpenGL_ParticleSystem()
	{

	}
	void OpenGL_ParticleSystem::OnUpdate(Sol::TimeStep deltaTime)
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.LifeRemaining -= deltaTime;
			particle.Position += particle.Velocity * (float)deltaTime;
			//particle.Rotation += 0.01f * deltaTime;
		}
	}
	void OpenGL_ParticleSystem::OnRender(OrthoCamera& camera)
	{
		if (!m_QuadVA)
		{
			float vertices[] = {
				 -0.5f, -0.5f, 0.0f,
				  0.5f, -0.5f, 0.0f,
				  0.5f,  0.5f, 0.0f,
				 -0.5f,  0.5f, 0.0f
			};

			glCreateVertexArrays(1, &m_QuadVA);
			glBindVertexArray(m_QuadVA);

			GLuint quadVB, quadIB;
			glCreateBuffers(1, &quadVB);
			glBindBuffer(GL_ARRAY_BUFFER, quadVB);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glEnableVertexArrayAttrib(quadVB, 0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

			uint32_t indices[] = {
				0, 1, 2, 2, 3, 0
			};

			glCreateBuffers(1, &quadIB);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIB);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		
			m_ParticleShader = std::shared_ptr<Shader>(Shader::Create(m_VertexCode,m_FragmentCode));
			m_ParticleShaderViewProj = glGetUniformLocation(m_ParticleShader->GetID(), "u_ViewProj");
			m_ParticleShaderTransform = glGetUniformLocation(m_ParticleShader->GetID(), "u_Transform");
			m_ParticleShaderColor = glGetUniformLocation(m_ParticleShader->GetID(), "u_Color");
		}

		glUseProgram(m_ParticleShader->GetID());
		glUniformMatrix4fv(m_ParticleShaderViewProj, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix()));

		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			// Fade away particles
			float life = particle.LifeRemaining / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
			//color.a = color.a * life;

			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

			// Render
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.Position.x, particle.Position.y, 0.0f })
				//* glm::rotate(glm::mat4(1.0f), particle.Rotation, { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });
			glUniformMatrix4fv(m_ParticleShaderTransform, 1, GL_FALSE, glm::value_ptr(transform));
			glUniform4fv(m_ParticleShaderColor, 1, glm::value_ptr(color));
			glBindVertexArray(m_QuadVA);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
	}
	void OpenGL_ParticleSystem::Emit(const ParticleProperties& properties)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];
		particle.Active = true;
		particle.Position = properties.Position;
		//particle.Rotation = Sol::Random::Float() * 2.0f * glm::pi<float>();

		// Velocity
		particle.Velocity = properties.Velocity;
		particle.Velocity.x += properties.VelocityVariation.x * (Sol::Random::Float() - 0.5f);
		particle.Velocity.y += properties.VelocityVariation.y * (Sol::Random::Float() - 0.5f);

		// Color
		particle.ColorBegin = properties.ColorBegin;
		particle.ColorEnd = properties.ColorEnd;

		particle.LifeTime = properties.LifeTime;
		particle.LifeRemaining = properties.LifeTime;
		particle.SizeBegin = properties.SizeBegin + properties.SizeVariation * (Sol::Random::Float() - 0.5f);
		particle.SizeEnd = properties.SizeEnd;

		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}
}

