#include "pcpch.h"
#include "Application.h"

#include "Pinecone/Core/Log.h"
#include "Pinecone/Renderer/Renderer.h"

// TEMPORARY
#include "Pinecone/Renderer/RenderCommand.h"

namespace Pinecone
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		PC_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallback(PC_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
	}

	Application::~Application()
	{
		Renderer::Shutdown();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(PC_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(PC_BIND_EVENT_FN(Application::OnWindowResized));

		PC_CORE_INFO("Event: {0}", e.ToString());
	}

	void Application::Run()
	{
		RenderCommand::SetClearColor({ 1.0f, 0.0f, 1.0f, 1.0f });
		while (m_Running)
		{
			RenderCommand::Clear();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}