#include "SceneHeirarchyPanel.h"

#include <Pinecone/Scene/Components.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "../UIWidgets.h"

namespace Pinecone
{
	SceneHierarchyPanel::SceneHierarchyPanel()
	{
		m_SceneIcon = Texture2D::Create("Resources/Icons/Panels/SceneHierachy/SceneIcon.png");
		m_GameObjectIcon = Texture2D::Create("Resources/Icons/Panels/SceneHierachy/GameObjectIcon.png");
	}
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		if (m_Open)
		{
			ImGui::Begin("Scene Hierarchy", &m_Open);

			if (m_Context != nullptr)
			{
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
				if (m_Context->m_Registry.size() == 0)
					flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
				else
					flags |= ImGuiTreeNodeFlags_DefaultOpen;
				bool opened = UI::DrawTreeNodeWithImage((ImTextureID)m_SceneIcon->GetRendererID(), (void*)454655, flags, m_Context->m_Name.c_str());

				if (opened && m_Context->m_Registry.size() > 0)
				{
					m_Context->m_Registry.each([&](auto entityID)
						{
							GameObject entity{ entityID , m_Context.get() };
							DrawEntityNode(entity);
						});

					ImGui::TreePop();
				}

				if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
					m_SelectionContext = {};

				// Right-click on blank space
				if (ImGui::BeginPopupContextWindow(0, 1))
				{
					if (ImGui::MenuItem("Create Empty Game Object"))
						m_Context->CreateGameObject("GameObject");

					ImGui::EndPopup();
				}
			}

			ImGui::End();
		}
	}

	void SceneHierarchyPanel::SetSelectedGameObject(GameObject gameObject)
	{
		m_SelectionContext = gameObject;
	}

	void SceneHierarchyPanel::DrawEntityNode(GameObject gameObject)
	{
		if (gameObject.HasComponent<SceneComponent>())
			return;

		auto& tag = gameObject.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags node_flags = ((m_SelectionContext == gameObject) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
#ifndef HAS_CHILDREN
		node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
#endif
		bool opened = UI::DrawTreeNodeWithImage((ImTextureID)m_GameObjectIcon->GetRendererID(), (void*)(uint64_t)(uint32_t)gameObject, node_flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = gameObject;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Game Object"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

#ifdef HAS_CHILDREN
		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;;
			UI::DrawTreeNodeWithImage((ImTextureID)m_GameObjectIcon->GetRendererID(), (void*)9817239, flags, tag.c_str());
			ImGui::TreePop();
		}
#endif

		if (entityDeleted)
		{
			m_Context->DestroyGameObject(gameObject);
			if (m_SelectionContext == gameObject)
				m_SelectionContext = {};
		}
	}
}