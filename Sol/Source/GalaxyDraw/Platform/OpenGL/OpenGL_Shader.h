#pragma once

#include "GalaxyDraw/Interfaces/Shader.h"
#include <glm/glm.hpp>


//#include "GLMacros.h"
namespace GalaxyDraw
{
	class OpenGL_Shader : public Shader
	{
	public:
		
		OpenGL_Shader(const std::string& vertexFile, const std::string& fragmentFile);
		OpenGL_Shader(const std::string& nameToSet, const std::string& vertexFile, const std::string& fragmentFile);
		virtual ~OpenGL_Shader() override;

		virtual const std::string& GetName() const override;
		virtual const uint32_t GetID() const override;

		virtual void Bind() const override;
		virtual void Delete() const override;

		virtual void SetBool(const std::string& name, bool value) const override;
		void SetInt(const std::string& name, int value) const override;
		void SetFloat(const std::string& name, float value) const override;

		void SetVec2(const std::string& name, const glm::vec2& value) const override;
		void SetVec2(const std::string& name, float x, float y) const override;

		void SetVec3(const std::string& name, const glm::vec3& value) const override;
		void SetVec3(const std::string& name, float x, float y, float z) const override;

		void SetVec4(const std::string& name, const glm::vec4& value) const override;
		void SetVec4(const std::string& name, float x, float y, float z, float w)const override;

		void SetMat2(const std::string& name, const glm::mat2& mat) const override;
		void SetMat3(const std::string& name, const glm::mat3& mat) const override;
		void SetMat4(const std::string& name, const glm::mat4& mat) const override;

	private:
		void Compile(const std::string& vertexFile, const std::string& fragmentFile);
		// Checks if the different Shaders have compiled properly
		void compileErrors(unsigned int shader, const char* type);
		int GetUniformLocation(const std::string& name) const;

		uint32_t m_RendererID;
		std::string m_Name;
	};
}