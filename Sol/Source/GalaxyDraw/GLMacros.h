#ifndef GL_MACROS_H
#define GL_MACROS_H

#include <iostream>
#include <glad/glad.h>

#define ASSERT(x) if (!(x)) __debugbreak();

#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x,__FILE__,__LINE__));
// By using \ you can ignore the \n character, such that the define statement has a more-than-one-line scope


static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ") when calling '" << function <<
			"' from " << file << ":" << line << std::endl;
		return false;
	}

	return true;
}

#endif