#pragma once
#include <memory>

namespace GalaxyDraw 
{
	struct FramebufferProperties
	{
		uint32_t Width, Height;
		uint32_t samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		static std::unique_ptr<Framebuffer> Create(const FramebufferProperties& properties);
		virtual ~Framebuffer() = default;


		virtual void Bind() = 0;
		virtual void UnBind() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual const FramebufferProperties& GetProperties() const = 0;
		virtual uint32_t GetColorAttachmentsRendererID() const = 0;

	private:

	};
}