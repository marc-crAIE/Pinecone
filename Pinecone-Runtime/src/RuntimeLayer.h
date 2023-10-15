#pragma once

#include <Pinecone.h>
#include <Pinecone/Renderer/EditorCamera.h>

namespace Pinecone
{
	class RuntimeLayer : public Layer
	{
	public:
		RuntimeLayer();
		virtual ~RuntimeLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnWindowDrop(WindowDropEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		void OnScenePlay();
		void OnSceneStop();
		void OnScenePause();

		void OnDuplicateGameObject();

		void OnOverlayRender();

		bool OpenProject();
		void OpenProject(const std::filesystem::path& path);

		void OpenScene();
		void OpenScene(AssetHandle handle);
	private:
		EditorCamera m_EditorCamera;

		int m_GizmoType = -1;

		enum class SceneState
		{
			Edit = 0, Play = 1
		};
		SceneState m_SceneState = SceneState::Edit;

		Ref<Scene> m_RuntimeScene;

		GameObject m_HoveredGameObject;

		// Editor resources
		Ref<Texture2D> m_IconPlay, m_IconStop, m_IconPause, m_IconStep;
	};
}