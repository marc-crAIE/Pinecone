#pragma once

#include "Pinecone/Core/Base.h"
#include "Pinecone/Core/Window.h"

int main(int argc, char** argv);

namespace Pinecone
{
	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		Window& GetWindow() { return *m_Window; }

		void Close();

		static Application& Get() { return *s_Instance; }
	private:
		void Run();
	private:
		Scope<Window> m_Window;
		bool m_Running = true;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in the client program
	Application* CreateApplication();
}