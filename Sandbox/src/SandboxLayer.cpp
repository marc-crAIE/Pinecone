#include "SandboxLayer.h"

#include "Scripts/CameraController.h"

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
		sprite.Texture = m_PineconeTexture;

		m_Camera = m_ActiveScene->CreateGameObject("Camera");
		auto& cc = m_Camera.AddComponent<CameraComponent>();
		cc.Camera.SetViewportSize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		cc.Camera.SetOrthographic(5.0f, -1.0f, 1.0f);
		m_Camera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
	}

	void SandboxLayer::OnDetach()
	{
	}

	void SandboxLayer::OnUpdate(Timestep ts)
	{
		auto& pineconeTransform = m_Square.GetComponent<TransformComponent>();
		if (Input::IsKeyPressed(Key::Q))
		{
			pineconeTransform.Rotation += glm::radians(glm::vec3{ 0.0f, 0.0f, 45.0f } * (float)ts);
		}
		if (Input::IsKeyPressed(Key::E))
		{
			pineconeTransform.Rotation -= glm::radians(glm::vec3{ 0.0f, 0.0f, 45.0f } * (float)ts);
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