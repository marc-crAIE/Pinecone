#pragma once

#include <Pinecone.h>
#include <Pinecone/Scene/SceneCamera.h>
#include <Pinecone/Events/KeyEvent.h>

#include <Pinecone/Renderer/Font.h>

using namespace Pinecone;

namespace Sandbox
{
	class SandboxLayer : public Layer
	{
	public:
		SandboxLayer();
		~SandboxLayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		Ref<Texture2D> m_PineconeTexture;
		Ref<Texture2D> m_TreeTexture;

		Ref<Scene> m_ActiveScene;
		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		GameObject m_Square;
		GameObject m_Tree;
		GameObject m_Camera;

		Ref<Font> m_Font;
	};
}