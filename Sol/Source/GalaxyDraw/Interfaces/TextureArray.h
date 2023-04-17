#pragma once
#include "Sol/Core/Core.h"
#include "Texture.h"
#include <String>

namespace GalaxyDraw {

	class TextureArray
	{
	public:
		static Sol::s_ptr<TextureArray> Create(uint32_t maxTextures);
		virtual ~TextureArray() = default;
		virtual void AddTexture(const std::string& path) = 0;
		virtual bool IsTextureLoaded(const std::string& path) = 0;
		virtual uint32_t GetTextureIndex(const std::string& path) = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual uint32_t Size() const = 0;
		//virtual void SetData(void* data, uint32_t size) = 0;
		virtual void Bind(uint32_t textureUnit = 0) const = 0;

		virtual bool operator==(const TextureArray& other) const = 0;

	};

}