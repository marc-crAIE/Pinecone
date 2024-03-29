#pragma once

#include "Pinecone/Core/Base.h"
#include "Pinecone/Core/Window.h"
#include "Pinecone/Core/LayerStack.h"
#include "Pinecone/Core/Timestep.h"

#include "Pinecone/Events/Event.h"
#include "Pinecone/Events/ApplicationEvent.h"

#include "Pinecone/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Pinecone
{
	class Application
	{
	public:
		/// <summary>
		/// The Application constructor
		/// </summary>
		Application();
		/// <summary>
		/// The Application destructor
		/// </summary>
		virtual ~Application();

		/// <summary>
		/// Called when an event occurs
		/// </summary>
		/// <param name="e">The event</param>
		void OnEvent(Event& e);

		/// <summary>
		/// Push a layer onto the layer stack
		/// </summary>
		/// <param name="layer">The layer</param>
		void PushLayer(Layer* layer);
		/// <summary>
		/// Push an overlay onto the layer stack. Overlays sit ontop of all other normal layers
		/// </summary>
		/// <param name="overlay">The overlay layer</param>
		void PushOverlay(Layer* overlay);

		/// <summary>
		/// Get the application window;
		/// </summary>
		/// <returns>The window</returns>
		Window& GetWindow() { return *m_Window; }

		/// <summary>
		/// Close the application
		/// </summary>
		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		/// <summary>
		/// Get the instance of the application
		/// </summary>
		/// <returns></returns>
		static Application& Get() { return *s_Instance; }
	private:
		/// <summary>
		/// Start the application run loop
		/// </summary>
		void Run();
		/// <summary>
		/// Handle the WindowClose event
		/// </summary>
		/// <param name="e">The WindowClose event</param>
		/// <returns>If the event was handled</returns>
		bool OnWindowClose(WindowCloseEvent& e);
		/// <summary>
		/// Handle the WindowResized event
		/// </summary>
		/// <param name="e">The WindowResized event</param>
		/// <returns>If the event was handled</returns>
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in the client program
	Application* CreateApplication();
}