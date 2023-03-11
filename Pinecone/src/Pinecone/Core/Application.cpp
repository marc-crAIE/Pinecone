#include "pcpch.h"
#include "Application.h"

#include "Pinecone/Core/Log.h"

namespace Pinecone
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		PC_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}
}