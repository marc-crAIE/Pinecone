#pragma once

#include <sstream>

#include "Pinecone/Core/Base.h"
#include "Pinecone/Events/Event.h"
#include "Pinecone/Renderer/OpenGLContext.h"

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
		using EventCallbackFn = std::function<void(Event&)>;

		/// <summary>
		/// The Window constructor
		/// </summary>
		/// <param name="props">The properties that will be used to create the window</param>
		Window(const WindowProps& props);
		/// <summary>
		/// The Window destructor
		/// </summary>
		~Window();

		/// <summary>
		/// Update the window
		/// </summary>
		void OnUpdate();

		/// <summary>
		/// Get the width of the window (in pixels)
		/// </summary>
		/// <returns>The window width</returns>
		unsigned int GetWidth() const { return m_Data.Width; }
		/// <summary>
		/// Get the height of the window (in pixels)
		/// </summary>
		/// <returns>The window height</returns>
		unsigned int GetHeight() const { return m_Data.Height; }

		/// <summary>
		/// Set the callback function for events
		/// </summary>
		/// <param name="callback">The event callback function</param>
		void SetEventCallback(const EventCallbackFn& callback) { m_Data.EventCallback = callback; }
		/// <summary>
		/// Enable or disable v-sync
		/// </summary>
		/// <param name="enabled">Whether to enable v-sync or not</param>
		void SetVSync(bool enabled);
		/// <summary>
		/// Returns true if the window has v-sync enabled
		/// </summary>
		/// <returns>True if v-sync is enabled</returns>
		bool IsVSync() const { return m_Data.VSync; }

		/// <summary>
		/// Get the native GLFW window handle
		/// </summary>
		/// <returns>The GLFW window handle</returns>
		GLFWwindow* GetNativeWindow() const { return m_Window; }

		/// <summary>
		/// Creates a smart unique pointer to a new Window
		/// </summary>
		/// <param name="props">The properties that will be used to create the window</param>
		/// <returns>A unique pointer to a new Window</returns>
		static Scope<Window> Create(const WindowProps& props = WindowProps());
	private:
		/// <summary>
		/// Initializes the GLFW window, sets up our OpenGL context and the window event 
		/// callback functions
		/// </summary>
		/// <param name="props">The properties that will be used to create the window</param>
		void Init(const WindowProps& props);
		/// <summary>
		/// Destroy the GLFW window handle. Terminates GLFW is there are no other windows
		/// </summary>
		void Shutdown();
	private:
		GLFWwindow* m_Window;
		Scope<OpenGLContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}