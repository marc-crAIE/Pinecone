#include "pcpch.h"
#include "Application.h"

#include "Pinecone/Core/Log.h"
#include "Pinecone/Renderer/Renderer.h"
#include "Pinecone/Utils/Utils.h"

namespace Pinecone
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		// Make sure there is only one instance of the application
		PC_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Create the window
		m_Window = Window::Create();
		// Set the window event callback function to be the application OnEvent function
		m_Window->SetEventCallback(PC_BIND_EVENT_FN(Application::OnEvent));

		// Initilize the renderer
		Renderer::Init();
	}

	Application::~Application()
	{
		// Shutdown the renderer
		Renderer::Shutdown();
	}

	void Application::Close()
	{
		// Set running to false which will close the application
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		// Create the event dispatcher
		EventDispatcher dispatcher(e);
		// Dispatch the specified event types to their appropriate functions
		dispatcher.Dispatch<WindowCloseEvent>(PC_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(PC_BIND_EVENT_FN(Application::OnWindowResized));

		// Pass the event first to last through the layers
		// Note: If the event was handled in one layer, it will be ignored in all layers that come after
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			// Break out of the for loop if the event has been handled
			if (e.Handled)
				break;
			// Pass the event to the layers OnEvent function
			(*it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		// Push the layer to the layer stack and call its OnAttach function
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		// Push the overlay to the layer stack and call its OnAttach function
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			// Get the time in seconds since we had the previous frame
			float time = Time::GetTime();
			Timestep ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			// Don't update layers when minimized
			if (!m_Minimized)
			{
				// Update all of the layers
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(ts);
			}

			// Update the window
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		// Set running to false which will close the application
		m_Running = false;
		// Return true as we handled the event
		return true;
	}

	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		// If the width and height is 0 then the window is most likely minimized
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			// Return false as other layers might want to handle this event aswell
			return false;
		}

		m_Minimized = false;
		// Set the viewport size for the renderer
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		// Return false as other layers might want to handle this event aswell
		return false;
	}
}