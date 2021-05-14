#pragma once
#include "PaperbackPCH.h"

#include "Core.h"
#include "Math/Vector.h"

namespace Paperback
{
	struct WindowProperties
	{
		WindowProperties(std::string _title = "Paperback", u32 _width = 1280, u32 _height = 720) :
			title{ _title }, width{ _width }, height{ _height } { }

		std::string title;
		u32 width, height;
	};

	class PAPERBACK_API IWindow
	{
	public:
		using EventCallback = Func<void, IEvent&>;

		virtual ~IWindow() = default;

		virtual u32 GetWidth() const = 0;
		virtual u32 GetHeight() const = 0;	

		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void Terminate() = 0;

		virtual void* GetWindow() const = 0;
		virtual void SetEventCallback(const EventCallback& callback) = 0;

		static Scope<IWindow> Create(const WindowProperties& props = WindowProperties());
	};
}