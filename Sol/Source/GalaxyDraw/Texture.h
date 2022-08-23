#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>

#include "shaderClass.h"
#include "GLMacros.h"

class Texture
{
public:
	unsigned char* m_LocalBuffer;
	GLuint ID;
	GLenum type;
	GLuint unit;

	Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint _unit);
	// Binds a texture
	void Bind() const;
	// Unbinds a texture
	void Unbind() const;
	// Deletes a texture
	void Delete();
};
#endif