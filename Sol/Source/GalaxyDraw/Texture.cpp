#include "Texture.h"

Texture::Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType)
{
	// Assigns the type of the texture ot the texture object
	type = texType;

	// Stores the width, height, and the number of color channels of the image
	int widthImg, heightImg, numColCh;
	// Flips the image so it appears right side up
	stbi_set_flip_vertically_on_load(true);
	// Reads the image from a file and stores it in bytes
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	// Generates an OpenGL texture object
	GLCall(glGenTextures(1, &ID));
	// Assigns the texture to a Texture Unit
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	unit = slot;
	GLCall(glBindTexture(texType, ID));

	// Configures the type of algorithm that is used to make the image smaller or bigger
	GLCall(glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	
	// Configures the way the texture repeats (if it does at all)
	GLCall(glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCall(glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT));

	// Extra lines in case you choose to use GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	// Assigns the image to the OpenGL Texture object
	GLCall(glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes));
	// Generates MipMaps
	GLCall(glGenerateMipmap(texType));

	// Deletes the image data as it is already in the OpenGL Texture object
	stbi_image_free(bytes);

	// Unbinds the OpenGL Texture object so that it can't accidentally be modified
	GLCall(glBindTexture(texType, 0));
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint _unit)
{
	// Gets the location of the uniform
	GLCall(GLuint texUni = glGetUniformLocation(shader.ID, uniform));
	// Shader needs to be activated before changing the value of a uniform
	shader.Bind();
	// Sets the value of the uniform
	GLCall(glUniform1i(texUni, _unit));
}

void Texture::Bind() const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + unit));
	GLCall(glBindTexture(type, ID));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(type, 0));
}

void Texture::Delete()
{
	GLCall(glDeleteTextures(1, &ID));
}