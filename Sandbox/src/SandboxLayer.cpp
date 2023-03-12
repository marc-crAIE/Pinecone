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

		m_PineconeTexture = Texture2D::Create("assets/textures/pinecone.png");

		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });

		m_ActiveScene = CreateRef<Scene>();

		m_Square = m_ActiveScene->CreateGameObject();
		m_ActiveScene->GetRegistry().emplace<TransformComponent>(m_Square);
		m_ActiveScene->GetRegistry().emplace<SpriteRendererComponent>(m_Square);

		m_ActiveScene->GetRegistry().get<SpriteRendererComponent>(m_Square).Color = glm::vec4(0.2f, 0.4f, 0.8f, 1.0f);
	}

	void SandboxLayer::OnDetach()
	{
	}

	void SandboxLayer::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(Key::Q))
		{
			m_PineconeRotation += 45.0f * ts;
		}
		else if (Input::IsKeyPressed(Key::E))
		{
			m_PineconeRotation -= 45.0f * ts;
		}

		Renderer2D::ResetStats();
		RenderCommand::Clear();

		Renderer2D::BeginScene(m_Camera);

		m_ActiveScene->OnUpdate(ts);

		Renderer2D::DrawRotatedQuad(glm::vec2(0.0f), glm::vec2(1.0f), glm::radians(45.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
		Renderer2D::DrawRotatedQuad(glm::vec2(1.0f), glm::vec2(1.0f), glm::radians(m_PineconeRotation), m_PineconeTexture);

		Renderer2D::EndScene();
	}

	void SandboxLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(PC_BIND_EVENT_FN(SandboxLayer::OnWindowResized));
		dispatcher.Dispatch<KeyPressedEvent>(PC_BIND_EVENT_FN(SandboxLayer::OnKeyPressed));
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