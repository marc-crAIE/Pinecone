#include "SandboxLayer.h"

#include "Scripts/CameraController.h"

#include <Pinecone/Asset/TextureImporter.h>

#include <imgui/imgui.h>

namespace Sandbox
{
	SandboxLayer::SandboxLayer()
		: Layer("SandboxLayer")
	{
		PC_PROFILE_FUNCTION();

		m_Font = Font::GetDefault();
	}

	void SandboxLayer::OnAttach()
	{
		PC_PROFILE_FUNCTION();

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec); 

		m_PineconeTexture = TextureImporter::LoadTexture2D("assets/textures/pinecone.png");
		m_TreeTexture = TextureImporter::LoadTexture2D("assets/textures/tree.png"); // TODO: Asset configuration

		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });

		m_ActiveScene = CreateRef<Scene>();

		m_Square = m_ActiveScene->CreateGameObject("Pinecone");
		auto& pSprite = m_Square.AddComponent<SpriteComponent>();
		pSprite.Texture = m_PineconeTexture->Handle;

		m_Tree = m_ActiveScene->CreateGameObject("Tree");
		auto& tSprite = m_Tree.AddComponent<SpriteComponent>();
		tSprite.Texture = m_TreeTexture->Handle;
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
		PC_PROFILE_FUNCTION();

		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

		// Resize
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

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

		m_Framebuffer->Bind();
		RenderCommand::Clear();

		m_ActiveScene->OnUpdateRuntime(ts);

		Renderer2D::BeginScene(m_Camera.GetComponent<CameraComponent>().Camera, m_Camera.GetComponent<TransformComponent>().GetTransform());

		//Renderer2D::DrawQuad(glm::mat4(1.0f), m_Font->GetAtlasTexture());

		//Renderer2D::DrawString("Hello World!", Font::GetDefault(), glm::mat4(1.0f), glm::vec4(1.0f));

		Renderer2D::EndScene();

		m_Framebuffer->Unbind();
	}

	void SandboxLayer::OnImGuiRender()
	{
		PC_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Editor Dockspace", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 200.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("EditorDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::PopStyleVar();
		ImGui::End();

		ImGui::Begin("Stats");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::End();
	}

	void SandboxLayer::OnEvent(Event& e)
	{
		PC_PROFILE_FUNCTION();

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
		PC_PROFILE_FUNCTION();

		m_ActiveScene->OnViewportResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}