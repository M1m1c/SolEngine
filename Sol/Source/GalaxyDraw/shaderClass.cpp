#include "solpch.h"

#include "shaderClass.h"
#include <glad/glad.h>


//TODO I seem unable to send afile path and have this find it
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);

	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return (contents);
	}
	throw (errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	(glShaderSource(vertexShader, 1, &vertexSource, NULL));
	(glCompileShader(vertexShader));
	compileErrors(vertexShader, "VERTEX");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	(glShaderSource(fragmentShader, 1, &fragmentSource, NULL));
	(glCompileShader(fragmentShader));
	compileErrors(fragmentShader, "FRAGMENT");

	(ID = glCreateProgram());
	(glAttachShader(ID, vertexShader));
	(glAttachShader(ID, fragmentShader));
	(glLinkProgram(ID));
	compileErrors(ID, "PROGRAM");

	(glDeleteShader(vertexShader));
	(glDeleteShader(fragmentShader));
}

void Shader::Bind() const
{
	(glUseProgram(ID));
}

void Shader::Delete()
{
	(glDeleteProgram(ID));
}

// Checks if the different Shaders have compiled properly
void Shader::compileErrors(unsigned int shader, const char* type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		(glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled));
		if (hasCompiled == GL_FALSE)
		{
			(glGetShaderInfoLog(shader, 1024, NULL, infoLog));
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		(glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled));
		if (hasCompiled == GL_FALSE)
		{
			(glGetProgramInfoLog(shader, 1024, NULL, infoLog));
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}

void Shader::setBool(const std::string& name, bool value) const
{
    (glUniform1i(GetUniformLocation(name.c_str()), (int)value));
}

void Shader::setInt(const std::string& name, int value) const
{
    (glUniform1i(GetUniformLocation(name.c_str()), value));
}

void Shader::setFloat(const std::string& name, float value) const
{
    (glUniform1f(GetUniformLocation(name.c_str()), value));
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    (glUniform2fv(GetUniformLocation(name.c_str()), 1, &value[0]));
}

void Shader::setVec2(const std::string& name, float x, float y) const
{
    (glUniform2f(GetUniformLocation(name.c_str()), x, y));
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    (glUniform3fv(GetUniformLocation(name.c_str()), 1, &value[0]));
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    (glUniform3f(GetUniformLocation(name.c_str()), x, y, z));
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    (glUniform4fv(GetUniformLocation(name.c_str()), 1, &value[0]));
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
    (glUniform4f(GetUniformLocation(name.c_str()), x, y, z, w));
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    (glUniformMatrix2fv(GetUniformLocation(name.c_str()), 1, GL_FALSE, &mat[0][0]));
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    (glUniformMatrix3fv(GetUniformLocation(name.c_str()), 1, GL_FALSE, &mat[0][0]));
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	(glUniformMatrix4fv(GetUniformLocation(name.c_str()), 1, GL_FALSE, &mat[0][0]));
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
	GLint location = glGetUniformLocation(ID, name.c_str());
	if (location == -1)
	{
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}

	return location;
}