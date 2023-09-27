#pragma once

#include <Pinecone/Core/Base.h>
#include <Pinecone/Scene/GameObject.h>
#include <Pinecone/Renderer/Texture2D.h>

namespace Pinecone
{
	class PropertiesPanel
	{
	public:
		PropertiesPanel();
		PropertiesPanel(const Ref<Scene>& sceneContext);

		void SetSceneContext(const Ref<Scene>& sceneContext);

		void OnImGuiRender(const GameObject& context);

		void Open() { m_Open = true; }
	private:
		template<typename T>
		void DisplayAddComponentEntry(GameObject gameObject, const std::string& entryName);

		void DrawComponents(GameObject gameObject);
	private:
		Ref<Scene> m_SceneContext;

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