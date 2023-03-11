#pragma once

#include "Pinecone/Core/Base.h"
#include "Pinecone/Core/Window.h"

#include "Pinecone/Events/Event.h"
#include "Pinecone/Events/ApplicationEvent.h"

int main(int argc, char** argv);

namespace Pinecone
{
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

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
	};

	// To be defined in the client program
	Application* CreateApplication();
}