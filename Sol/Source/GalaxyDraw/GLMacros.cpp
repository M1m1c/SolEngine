#include "solpch.h"
#include "GLMacros.h"

#include <glad/glad.h>

void GLMacros::GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLMacros::GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ") when calling '" << function <<
			"' from " << file << ":" << line << std::endl;
		return false;
	}

	return true;
}
