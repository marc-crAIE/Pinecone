#include "SandboxLayer.h"

namespace Sandbox
{
	SandboxLayer::SandboxLayer()
		: Layer("SandboxLayer")
	{ }

	void SandboxLayer::OnAttach()
	{
		m_PineconeTexture = Texture2D::Create("assets/textures/pinecone.png");

		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });

		m_ActiveScene = CreateRef<Scene>();

		m_Square = m_ActiveScene->CreateGameObject();
		auto& sprite = m_Square.AddComponent<SpriteComponent>();
		sprite.Color = { 0.2f, 0.5f, 0.9f, 1.0f };

		m_Camera = m_ActiveScene->CreateGameObject("Camera");
		auto& cc = m_Camera.AddComponent<CameraComponent>();
		cc.Camera.SetViewportSize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		cc.Camera.SetOrthographic(5.0f, -1.0f, 1.0f);
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
		if (Input::IsKeyPressed(Key::E))
		{
			m_PineconeRotation -= 45.0f * ts;
		}

		auto& cameraTransform = m_Camera.GetComponent<TransformComponent>();
		if (Input::IsKeyPressed(Key::W))
		{
			cameraTransform.Translation += glm::vec3{ 0.0f, 1.0f, 0.0f } * (float)ts;
		}
		if (Input::IsKeyPressed(Key::S))
		{
			cameraTransform.Translation += glm::vec3{ 0.0f, -1.0f, 0.0f } * (float)ts;
		}
		if (Input::IsKeyPressed(Key::A))
		{
			cameraTransform.Translation += glm::vec3{ -1.0f, 0.0f, 0.0f } * (float)ts;
		}
		if (Input::IsKeyPressed(Key::D))
		{
			cameraTransform.Translation += glm::vec3{ 1.0f, 0.0f, 0.0f } * (float)ts;
		}

		Renderer2D::ResetStats();
		RenderCommand::Clear();

		m_ActiveScene->OnUpdate(ts);
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
		m_ActiveScene->OnViewportResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}