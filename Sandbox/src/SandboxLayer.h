#pragma once

#include <Pinecone.h>
#include <Pinecone/Scene/SceneCamera.h>
#include <Pinecone/Events/KeyEvent.h>

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
		void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		Ref<Texture2D> m_PineconeTexture;
		float m_PineconeRotation = 0.0f;

		Ref<Scene> m_ActiveScene;

		GameObject m_Square;
		GameObject m_Camera;
	};
}