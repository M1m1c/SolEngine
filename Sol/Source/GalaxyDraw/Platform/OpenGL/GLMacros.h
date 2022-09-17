#ifndef GL_MACROS_H
#define GL_MACROS_H

#define ASSERT(x) if (!(x)) __debugbreak();

#define GLCall(x) GLMacros::GLClearError();\
	x;\
	ASSERT(GLMacros::GLLogCall(#x,__FILE__,__LINE__));
// By using \ you can ignore the \n character, such that the define statement has a more-than-one-line scope

class GLMacros
{
public:

	static void GLClearError();

	static bool GLLogCall(const char* function, const char* file, int line);
};

#endif