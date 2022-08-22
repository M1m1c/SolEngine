#pragma once

#include "solpch.h"

#include "Sol/Core.h"
#include "Sol/Events/Event.h"

namespace Sol 
{
	struct WindowProps
	{
		std::string Title;

		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string title = "Sol Engine",
			unsigned int width = 1200,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height){}
	};

	class SOL_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}
		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}