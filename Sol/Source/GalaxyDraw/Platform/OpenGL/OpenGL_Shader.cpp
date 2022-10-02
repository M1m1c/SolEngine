#include "solpch.h"
#include "OpenGL_Shader.h"
#include "GalaxyDraw/Interfaces/GalaxyMacros.h"
#include "GLMacros.h"
#include <glad/glad.h>

namespace GalaxyDraw 
{
	//Constructs a shader with a automatically generated name based on vertex file name
	OpenGL_Shader::OpenGL_Shader(const std::string& vertexFile, const std::string& fragmentFile)
	{
		auto lastSlash = vertexFile.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = vertexFile.rfind('.');
		auto count = lastDot == std::string::npos ? vertexFile.size() - lastSlash : lastDot - lastSlash;
		m_Name = vertexFile.substr(lastSlash, count);
		Compile(vertexFile, fragmentFile);
	}

	//constructs a shader with a specified name
	OpenGL_Shader::OpenGL_Shader(const std::string& nameToSet, const std::string& vertexFile, const std::string& fragmentFile)
	{
		m_Name = nameToSet;
		Compile(vertexFile, fragmentFile);
	}

	OpenGL_Shader::~OpenGL_Shader()
	{
		GLCall(glDeleteProgram(m_RendererID));
	}

	const std::string& OpenGL_Shader::GetName() const
	{
		return m_Name;
	}

	const uint32_t OpenGL_Shader::GetID() const
	{
		return m_RendererID;
	}


	void OpenGL_Shader::Bind() const
	{
		GLCall(glUseProgram(m_RendererID));
	}

	void OpenGL_Shader::Delete() const
	{
		GLCall(glDeleteProgram(m_RendererID));
	}

	void OpenGL_Shader::Compile(const std::string& vertexFile, const std::string& fragmentFile)
	{
		std::string vertexCode = GalaxyMacros::get_file_contents(vertexFile.c_str());
		std::string fragmentCode = GalaxyMacros::get_file_contents(fragmentFile.c_str());

		const char* vertexSource = vertexCode.c_str();
		const char* fragmentSource = fragmentCode.c_str();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLCall(glShaderSource(vertexShader, 1, &vertexSource, NULL));
		GLCall(glCompileShader(vertexShader));
		compileErrors(vertexShader, "VERTEX");

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		GLCall(glShaderSource(fragmentShader, 1, &fragmentSource, NULL));
		GLCall(glCompileShader(fragmentShader));
		compileErrors(fragmentShader, "FRAGMENT");

		GLCall(m_RendererID = glCreateProgram());
		GLCall(glAttachShader(m_RendererID, vertexShader));
		GLCall(glAttachShader(m_RendererID, fragmentShader));
		GLCall(glLinkProgram(m_RendererID));
		compileErrors(m_RendererID, "PROGRAM");

		GLCall(glDeleteShader(vertexShader));
		GLCall(glDeleteShader(fragmentShader));
	}

	// Checks if the different Shaders have compiled properly
	void OpenGL_Shader::compileErrors(unsigned int shader, const char* type)
	{
		// Stores status of compilation
		GLint hasCompiled;
		// Character array to store error message in
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled));
			if (hasCompiled == GL_FALSE)
			{
				GLCall(glGetShaderInfoLog(shader, 1024, NULL, infoLog));
				std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
			}
		}
		else
		{
			GLCall(glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled));
			if (hasCompiled == GL_FALSE)
			{
				GLCall(glGetProgramInfoLog(shader, 1024, NULL, infoLog));
				std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
			}
		}
	}

	void OpenGL_Shader::setBool(const std::string& name, bool value) const
	{
		GLCall(glUniform1i(GetUniformLocation(name.c_str()), (int)value));
	}

	void OpenGL_Shader::setInt(const std::string& name, int value) const
	{
		GLCall(glUniform1i(GetUniformLocation(name.c_str()), value));
	}

	void OpenGL_Shader::setFloat(const std::string& name, float value) const
	{
		GLCall(glUniform1f(GetUniformLocation(name.c_str()), value));
	}

	void OpenGL_Shader::setVec2(const std::string& name, const glm::vec2& value) const
	{
		GLCall(glUniform2fv(GetUniformLocation(name.c_str()), 1, &value[0]));
	}

	void OpenGL_Shader::setVec2(const std::string& name, float x, float y) const
	{
		GLCall(glUniform2f(GetUniformLocation(name.c_str()), x, y));
	}

	void OpenGL_Shader::setVec3(const std::string& name, const glm::vec3& value) const
	{
		//TODO for some reson this GLCALL triggerd a break point when everything was right, look into why
		//GLCall(glUniform3fv(GetUniformLocation(name.c_str()), 1, &value[0]));
		glUniform3fv(GetUniformLocation(name.c_str()), 1, &value[0]);
	}

	void OpenGL_Shader::setVec3(const std::string& name, float x, float y, float z) const
	{
		GLCall(glUniform3f(GetUniformLocation(name.c_str()), x, y, z));
	}

	void OpenGL_Shader::setVec4(const std::string& name, const glm::vec4& value) const
	{
		GLCall(glUniform4fv(GetUniformLocation(name.c_str()), 1, &value[0]));
	}

	void OpenGL_Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		GLCall(glUniform4f(GetUniformLocation(name.c_str()), x, y, z, w));
	}

	void OpenGL_Shader::setMat2(const std::string& name, const glm::mat2& mat) const
	{
		GLCall(glUniformMatrix2fv(GetUniformLocation(name.c_str()), 1, GL_FALSE, &mat[0][0]));
	}

	void OpenGL_Shader::setMat3(const std::string& name, const glm::mat3& mat) const
	{
		GLCall(glUniformMatrix3fv(GetUniformLocation(name.c_str()), 1, GL_FALSE, &mat[0][0]));
	}

	void OpenGL_Shader::setMat4(const std::string& name, const glm::mat4& mat) const
	{
		GLCall(glUniformMatrix4fv(GetUniformLocation(name.c_str()), 1, GL_FALSE, &mat[0][0]));
	}

	GLint OpenGL_Shader::GetUniformLocation(const std::string& name) const
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
		{
			std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
		}

		return location;
	}
}
