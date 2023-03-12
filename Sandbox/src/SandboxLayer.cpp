#include "SandboxLayer.h"

namespace Sandbox
{
	SandboxLayer::SandboxLayer()
		: Layer("SandboxLayer")
	{ }

	void SandboxLayer::OnAttach()
	{
		m_Camera.SetViewportSize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		m_Camera.SetOrthographic(5.0f, -1.0f, 1.0f);

		m_VertexArray = VertexArray::Create();

		// Our vertex data (Vec3 (position), Vec4 (Colour))
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

		m_Shader = Shader::Create("assets/shaders/Basic.glsl");

		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	}

	void SandboxLayer::OnDetach()
	{
	}

	void SandboxLayer::OnUpdate(Timestep ts)
	{
		Renderer2D::ResetStats();
		RenderCommand::Clear();

		Renderer2D::BeginScene(m_Camera);

		Renderer2D::DrawRotatedQuad(glm::vec2(0.0f), glm::vec2(1.0f), glm::radians(45.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

		Renderer2D::EndScene();
	}

	void SandboxLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(PC_BIND_EVENT_FN(SandboxLayer::OnWindowResized));
	}

	bool SandboxLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		return false;
	}

	bool SandboxLayer::OnWindowResized(WindowResizeEvent& e)
	{
		m_Camera.SetViewportSize(e.GetWidth(), e.GetHeight());
		return false;
	}
}