#pragma once

#include "EditorPanel.h"

#include <Pinecone/Core/Base.h>
#include <Pinecone/Editor/SelectionManager.h>
#include <Pinecone/Scene/GameObject.h>
#include <Pinecone/Renderer/Texture2D.h>

namespace Pinecone
{
	class PropertiesPanel : public EditorPanel
	{
	public:
		PropertiesPanel();
		PropertiesPanel(const Ref<Scene>& sceneContext);

		void SetSceneContext(const Ref<Scene>& sceneContext);
		void SetSelection(const SelectionContext& context, const UUID& selectionID);

		void OnImGuiRender() override;

		void Open() { m_Open = true; }
	private:
		template<typename T>
		void DisplayAddComponentEntry(GameObject gameObject, const std::string& entryName);

		void DrawComponents(GameObject gameObject);
		void DrawAssetEditor(AssetHandle handle);
	private:
		Ref<Scene> m_SceneContext;

		struct Selection
		{
			SelectionContext Context;
			UUID ID;
		};

		Selection m_Selection;

		Ref<Texture2D> m_TransformIcon;
		Ref<Texture2D> m_SpriteRendererIcon;
		Ref<Texture2D> m_CircleRendererIcon;
		Ref<Texture2D> m_CameraIcon;
		Ref<Texture2D> m_Rigidbody2DIcon;
		Ref<Texture2D> m_BoxCollider2DIcon;
		Ref<Texture2D> m_CircleCollider2DIcon;

		bool m_Open = true;
	};
}