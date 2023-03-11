#pragma once

int main(int argc, char** argv);

namespace Pinecone
{
	class Application
	{
	public:
		Application();
	private:
		void Run();
	private:
		bool m_Running = true;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in the client program
	Application* CreateApplication();
}