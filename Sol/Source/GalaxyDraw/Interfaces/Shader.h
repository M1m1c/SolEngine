#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glm/glm.hpp>


//#include "GLMacros.h"
namespace GalaxyDraw
{

	class Shader
	{
	public:

		//TODO turn into shared_ptr
		static Shader* Create(const char* vertexFile, const char* fragmentFile);

		virtual ~Shader() = default;

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
}
#endif