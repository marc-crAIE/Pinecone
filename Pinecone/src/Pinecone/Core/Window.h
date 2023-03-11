#pragma once

#include <sstream>

#include "Pinecone/Core/Base.h"
#include "Pinecone/Renderer/OpenGLContext.h"

#include <GLFW/glfw3.h>

namespace Pinecone
{
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Pinecone Engine",
			uint32_t width = 1280,
			uint32_t height = 720)
			: Title(title), Width(width), Height(height)
		{ }
	};

	class Window
	{
	public:
		Window(const WindowProps& props);
		~Window();

		void OnUpdate();

		unsigned int GetWidth() const { return m_Data.Width; }
		unsigned int GetHeight() const { return m_Data.Height; }

		void SetVSync(bool enabled);
		bool IsVSync() const { return m_Data.VSync; }

		GLFWwindow* GetNativeWindow() const { return m_Window; }

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	private:
		void Init(const WindowProps& props);
		void Shutdown();
	private:
		GLFWwindow* m_Window;
		Scope<OpenGLContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
		};

		WindowData m_Data;
	};
}