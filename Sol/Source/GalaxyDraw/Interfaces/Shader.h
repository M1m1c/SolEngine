#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glm/glm.hpp>
#include <unordered_map>


//#include "GLMacros.h"
namespace GalaxyDraw
{
	//TODO ikmplement so we only have to read one file instead of a vertex and fragmen file if we want
	class Shader
	{
	public:

		static std::shared_ptr<Shader> Create(
			const std::string& vertexFile,
			const std::string& fragmentFile,
			const std::string& nameToSet = "");

		virtual ~Shader() = default;

		virtual const std::string& GetName() const = 0;
		virtual const uint32_t GetID() const = 0;

		virtual void Bind() const = 0;
		virtual void Delete() const = 0;

		virtual void setBool(const std::string& name, bool value) const = 0;
		virtual void setInt(const std::string& name, int value) const = 0;
		virtual void setFloat(const std::string& name, float value) const = 0;

		virtual void setVec2(const std::string& name, const glm::vec2& value) const = 0;
		virtual void setVec2(const std::string& name, float x, float y) const = 0;

		virtual void setVec3(const std::string& name, const glm::vec3& value) const = 0;
		virtual void setVec3(const std::string& name, float x, float y, float z) const = 0;

		virtual void setVec4(const std::string& name, const glm::vec4& value) const = 0;
		virtual void setVec4(const std::string& name, float x, float y, float z, float w) const = 0;

		virtual void setMat2(const std::string& name, const glm::mat2& mat) const = 0;
		virtual void setMat3(const std::string& name, const glm::mat3& mat) const = 0;
		virtual void setMat4(const std::string& name, const glm::mat4& mat) const = 0;
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& nameToSet, const std::shared_ptr<Shader>& shader);
		void Add(const std::shared_ptr<Shader>& shader);

		std::shared_ptr<Shader> Load(
			const std::string& vertexFilePath,
			const std::string& fragmentFilePath);

		std::shared_ptr<Shader> Load(
			const std::string& nameToSet,
			const std::string& vertexFilePath,
			const std::string& fragmentFilePath);

		std::shared_ptr<Shader> Get(const std::string& name);

		bool Exists(const std::string& name);

	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
	};


}
#endif