#include "pcpch.h"
#include "Application.h"

#include "Pinecone/Core/Log.h"
#include "Pinecone/Renderer/Renderer.h"
#include "Pinecone/Scripting/ScriptEngine.h"
#include "Pinecone/Utils/PlatformUtils.h"

namespace Pinecone
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification)
		: m_Specification(specification)
	{
		PC_PROFILE_FUNCTION();

		// Make sure there is only one instance of the application
		PC_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Set working directory here
		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

		// Create the window
		m_Window = Window::Create(WindowProps(m_Specification.Name));
		// Set the window event callback function to be the application OnEvent function
		m_Window->SetEventCallback(PC_BIND_EVENT_FN(Application::OnEvent));

		// Initilize the renderer
		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		PC_PROFILE_FUNCTION();

		// Shutdown the renderer
		Renderer::Shutdown();

		ScriptEngine::Shutdown();
	}

	void Application::Close()
	{
		// Set running to false which will close the application
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		PC_PROFILE_FUNCTION();

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
		PC_PROFILE_FUNCTION();

		// Push the layer to the layer stack and call its OnAttach function
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		PC_PROFILE_FUNCTION();

		// Push the overlay to the layer stack and call its OnAttach function
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::SubmitToMainThread(const std::function<void()>& function)
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

		m_MainThreadQueue.emplace_back(function);
	}

	void Application::Run()
	{
		PC_PROFILE_FUNCTION();

		while (m_Running)
		{
			PC_PROFILE_SCOPE("RunLoop");

			// Get the time in seconds since we had the previous frame
			float time = Time::GetTime();
			Timestep ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			ExecuteMainThreadQueue();

			// Don't update layers when minimized
			if (!m_Minimized)
			{
				{
					PC_PROFILE_SCOPE("LayerStack OnUpdate");

					// Update all of the layers
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(ts);
				}

				m_ImGuiLayer->Begin();
				{
					PC_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
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
		PC_PROFILE_FUNCTION();

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

	void Application::ExecuteMainThreadQueue()
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

		for (auto& func : m_MainThreadQueue)
			func();

		m_MainThreadQueue.clear();
	}
}