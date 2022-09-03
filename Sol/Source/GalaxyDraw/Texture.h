#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

namespace GalaxyDraw 
{
	class Shader;

	class Texture
	{
	public:
		unsigned char* m_LocalBuffer;
		uint32_t ID;
		uint32_t type;
		uint32_t unit;

		Texture(const char* image, uint32_t texType, uint32_t slot, uint32_t format, uint32_t pixelType);

		// Assigns a texture unit to a texture
		void texUnit(Shader& shader, const char* uniform, uint32_t _unit);
		// Binds a texture
		void Bind() const;
		// Unbinds a texture
		void Unbind() const;
		// Deletes a texture
		void Delete();
	};
}
#endif