#pragma once

#include <Pinecone/Core/Base.h>
#include <Pinecone/Scene/Scene.h>
#include <Pinecone/Scene/GameObject.h>

#include <Pinecone/Renderer/Texture.h>

namespace Pinecone
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel();
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

		GameObject GetSelectedGameObject() const { return m_SelectionContext; }
		void SetSelectedGameObject(GameObject gameObject);

		void Open() { m_Open = true; }
	private:
		void DrawEntityNode(GameObject gameObject);
	private:
		Ref<Scene> m_Context;
		GameObject m_SelectionContext;

		Ref<Texture2D> m_SceneIcon;
		Ref<Texture2D> m_GameObjectIcon;

		bool m_Open = true;
	};
}