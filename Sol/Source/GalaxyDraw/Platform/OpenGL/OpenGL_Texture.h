#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include "GalaxyDraw/Interfaces/Texture.h"

namespace GalaxyDraw 
{
	class Shader;

	class OpenGL_Texture2D : public Texture2D
	{
	public:
		unsigned char* m_LocalBuffer;	
		uint32_t type;
		uint32_t unit;

		OpenGL_Texture2D(const std::string& path);
		OpenGL_Texture2D(const char* image, uint32_t texType, uint32_t slot, uint32_t format, uint32_t pixelType);

		virtual ~OpenGL_Texture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		// Assigns a texture unit to a texture
		void texUnit(Shader& shader, const char* uniform, uint32_t _unit);
		// Binds a texture
		virtual void Bind(uint32_t slot = 0) const override;
		// Unbinds a texture
		void Unbind() const;
		// Deletes a texture
		void Delete();
	private:
		std::string m_Path;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_RendererID;
	};
}
#endif