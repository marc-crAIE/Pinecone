#include "pcpch.h"
#include "Application.h"

#include "Pinecone/Core/Log.h"
#include "Pinecone/Renderer/Renderer.h"

// TEMPORARY
#include "Pinecone/Renderer/RenderCommand.h"
#include <glad/glad.h>

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

		/// TEMPORARY CODE

		m_VertexArray = VertexArray::Create();

		// Our vertex data (Vec3 (position), Vec4 (Colour))
		// Note: Nothing happens with colour at the moment as we don't yet have shaders!
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		// Create the vertex buffer
		std::shared_ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		// Create the index buffer
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);
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
		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		while (m_Running)
		{
			RenderCommand::Clear();

			Renderer::Submit(m_VertexArray);

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