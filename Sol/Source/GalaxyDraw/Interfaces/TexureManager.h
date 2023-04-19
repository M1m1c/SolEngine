#pragma once
#include "GalaxyDraw/Interfaces/Texture.h"
#include "Sol/Core/KeyedVector.h"
#include <string>
#include <memory>

namespace GalaxyDraw {

	//TODO this class will essentially replace the material manager.
	// it is supposed to be able to load and hold texture, giving us access to tehm via indexes/ keys
	class TextureManager
	{
	public:
		TextureManager();
		~TextureManager();

	private:

		KeyedVector <std::string, std::shared_ptr<Texture>> m_LoadedTextures;
	};
}