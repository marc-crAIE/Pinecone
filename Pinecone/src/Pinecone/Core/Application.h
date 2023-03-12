#pragma once

#include "Pinecone/Core/Base.h"
#include "Pinecone/Core/Window.h"

#include "Pinecone/Events/Event.h"
#include "Pinecone/Events/ApplicationEvent.h"

// TEMPORARY

#include "Pinecone/Renderer/Shader.h"
#include "Pinecone/Renderer/VertexArray.h"
#include "Pinecone/Renderer/Camera.h"

int main(int argc, char** argv);

namespace Pinecone
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);

		Window& GetWindow() { return *m_Window; }

		void Close();

		static Application& Get() { return *s_Instance; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);


		// Temporary stuff
	private:
		Ref<Shader> m_Shader;
		Ref<VertexArray> m_VertexArray;

		Camera m_Camera;
	};

	// To be defined in the client program
	Application* CreateApplication();
}