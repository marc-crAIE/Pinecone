#pragma once

#include <Pinecone.h>
#include <Pinecone/Renderer/EditorCamera.h>

#include "Drawing/Panels/SceneHeirarchyPanel.h"
#include "Drawing/Panels/PropertiesPanel.h"
#include "Drawing/Panels/ContentBrowserPanel.h"

namespace Pinecone
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnWindowDrop(WindowDropEvent& e);

		void OnScenePlay();
		void OnSceneStop();
		void OnScenePause();

		void OnDuplicateGameObject();

		void OnOverlayRender();

		void NewProject();
		bool OpenProject();
		void OpenProject(const std::filesystem::path& path);
		void SaveProject();

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();
		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);

		// UI Panels
		void UIToolbar();
	private:
		EditorCamera m_EditorCamera;

		Ref<Framebuffer> m_Framebuffer;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		int m_GizmoType = -1;

		enum class SceneState
		{
			Edit = 0, Play = 1
		};
		SceneState m_SceneState = SceneState::Edit;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;
		std::filesystem::path m_EditorScenePath;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		PropertiesPanel m_PropertiesPanel;
		Scope<ContentBrowserPanel> m_ContentBrowserPanel;

		bool m_ViewportOpen = true;
		bool m_StatsOpen = true;

		GameObject m_HoveredGameObject;

		// Editor resources
		Ref<Texture2D> m_IconPlay, m_IconStop, m_IconPause, m_IconStep;
	};
}