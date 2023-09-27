#pragma once

#include <Pinecone.h>
#include <Pinecone/Renderer/EditorCamera.h>

#include "Drawing/Panels/SceneHeirarchyPanel.h"
#include "Drawing/Panels/PropertiesPanel.h"

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

		void OnScenePlay();
		void OnSceneStop();

		void OnOverlayRender();

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

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		PropertiesPanel m_PropertiesPanel;

		bool m_ViewportOpen = true;
		bool m_StatsOpen = true;

		GameObject m_HoveredGameObject;

		// Editor resources
		Ref<Texture2D> m_IconPlay, m_IconStop;
	};
}