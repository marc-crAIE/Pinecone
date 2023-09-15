#include "SandboxLayer.h"

#include "Scripts/CameraController.h"

namespace Sandbox
{
	SandboxLayer::SandboxLayer()
		: Layer("SandboxLayer")
	{
		m_Font = Font::GetDefault();
	}

	void SandboxLayer::OnAttach()
	{
		m_PineconeTexture = Texture2D::Create("assets/textures/pinecone.png");
		m_TreeTexture = Texture2D::Create(TextureSpecification(TextureFilter::NEAREST), "assets/textures/tree.png");

		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });

		m_ActiveScene = CreateRef<Scene>();

		m_Square = m_ActiveScene->CreateGameObject("Pinecone");
		auto& pSprite = m_Square.AddComponent<SpriteComponent>();
		pSprite.Texture = m_PineconeTexture;

		m_Tree = m_ActiveScene->CreateGameObject("Tree");
		auto& tSprite = m_Tree.AddComponent<SpriteComponent>();
		tSprite.Texture = m_TreeTexture;
		tSprite.FlipY(true);

		m_Camera = m_ActiveScene->CreateGameObject("Camera");
		auto& cc = m_Camera.AddComponent<CameraComponent>();
		cc.Camera.SetViewportSize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		cc.Camera.SetOrthographic(5.0f, -1.0f, 1.0f);
		m_Camera.AddComponent<NativeScriptComponent>().Bind<CameraController>(std::string("test"));

		auto test = m_ActiveScene->GetGameObjectByTag("Pinecone");
		test.GetComponent<SpriteComponent>().Color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

		for (int i = 0; i < 5; i++)
			m_ActiveScene->CreateGameObject("TestObject");
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

		Renderer2D::BeginScene(m_Camera.GetComponent<CameraComponent>().Camera);

		//Renderer2D::DrawQuad(glm::mat4(1.0f), m_Font->GetAtlasTexture());

		Renderer2D::DrawString("Hello World!", Font::GetDefault(), glm::mat4(1.0f), glm::vec4(1.0f));

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
		m_ActiveScene->OnViewportResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}