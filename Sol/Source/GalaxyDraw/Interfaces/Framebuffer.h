#pragma once
#include <memory>

namespace GalaxyDraw
{

	enum class FramebufferTextureFormat
	{
		None = 0,
		RGBA8,
		RED_INTERGER,
		DEPTH24STENCIL8,
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format) : TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		//TODO filterin and wrap
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments) : Attachments(attachments) {}
		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferProperties
	{
		uint32_t Width, Height;
		uint32_t samples = 1;

		FramebufferAttachmentSpecification Attachments;

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
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		virtual const FramebufferProperties& GetProperties() const = 0;
		virtual uint32_t GetColorAttachmentsRendererID(uint32_t index = 0) const = 0;

	private:

	};
}