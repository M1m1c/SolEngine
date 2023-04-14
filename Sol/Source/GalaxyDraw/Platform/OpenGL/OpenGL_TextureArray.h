#pragma once
#include "GalaxyDraw/Interfaces/TextureArray.h"

namespace GalaxyDraw {

	class OpenGL_TextureArray : public TextureArray
	{
	public:
		OpenGL_TextureArray();
		~OpenGL_TextureArray();

		// Inherited via TextureArray
		virtual void addTexture(Sol::s_ptr<Texture> texture) override;

		virtual uint32_t GetRendererID() const override;

		virtual void Bind(uint32_t textureUnit = 0) const override;

		virtual bool operator==(const TextureArray& other) const override;

	private:
		uint32_t m_RendererID; // The ID of the texture array in GPU memory
	};
	
}