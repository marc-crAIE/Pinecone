#include "pcpch.h"
#include "Application.h"

#include "Pinecone/Core/Log.h"

namespace Pinecone
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
	}

	void Application::Run()
	{
		PC_CORE_INFO("Hello World!");
	}
}