#include "PropertiesPanel.h"

#include <Pinecone/Scene/Components.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

#include "../UIWidgets.h"

namespace Pinecone
{
	extern const std::filesystem::path g_AssetPath;

	PropertiesPanel::PropertiesPanel()
	{
		m_TransformIcon = Texture2D::Create("Resources/Icons/Panels/Properties/Components/TransformIcon.png");
		m_SpriteRendererIcon = Texture2D::Create("Resources/Icons/Panels/Properties/Components/SpriteRendererIcon.png");
		m_CircleRendererIcon = Texture2D::Create("Resources/Icons/Panels/Properties/Components/CircleRendererIcon.png");
		m_CameraIcon = Texture2D::Create("Resources/Icons/Panels/Properties/Components/CameraIcon.png");
		m_Rigidbody2DIcon = Texture2D::Create("Resources/Icons/Panels/Properties/Components/Rigidbody2DIcon.png");
		m_BoxCollider2DIcon = Texture2D::Create("Resources/Icons/Panels/Properties/Components/BoxCollider2DIcon.png");
		m_CircleCollider2DIcon = Texture2D::Create("Resources/Icons/Panels/Properties/Components/CircleCollider2DIcon.png");
	}

	PropertiesPanel::PropertiesPanel(const Ref<Scene>& sceneContext)
	{
		SetSceneContext(sceneContext);
	}

	void PropertiesPanel::SetSceneContext(const Ref<Scene>& sceneContext)
	{
		m_SceneContext = sceneContext;
	}

	void PropertiesPanel::OnImGuiRender(const GameObject& context)
	{
		if (m_Open)
		{
			ImGui::Begin("Properties", &m_Open);
			if (context)
			{
				DrawComponents(context);
			}

			ImGui::End();
		}
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, GameObject entity, ImTextureID texture, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = UI::DrawTreeNodeWithImage(texture, (void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	template<typename T>
	void PropertiesPanel::DisplayAddComponentEntry(GameObject gameObject, const std::string& entryName) {
		if (!gameObject.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				gameObject.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	void PropertiesPanel::DrawComponents(GameObject gameObject)
	{
		if (gameObject.HasComponent<TagComponent>())
		{
			auto& tag = gameObject.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<CameraComponent>(gameObject, "Camera");
			DisplayAddComponentEntry<SpriteComponent>(gameObject, "Sprite Renderer");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", gameObject, (ImTextureID)m_TransformIcon->GetRendererID(), [](auto& component)
			{
				UI::DrawVec3Control("Translation", component.Translation, 0.0f, 75.0f);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				UI::DrawVec3Control("Rotation", rotation, 0.0f, 75.0f);
				component.Rotation = glm::radians(rotation);
				UI::DrawVec3Control("Scale", component.Scale, 1.0f, 75.0f);
			});

		DrawComponent<SpriteComponent>("Sprite Renderer", gameObject, (ImTextureID)m_SpriteRendererIcon->GetRendererID(), [](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

				ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
						Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
						if (texture->IsLoaded())
							component.Texture = texture;
						else
							PC_WARN("Could not load texture {0}", texturePath.filename().string());
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
			});

		DrawComponent<CameraComponent>("Camera", gameObject, (ImTextureID)m_CameraIcon->GetRendererID(), [](auto& component)
			{
				auto& camera = component.Camera;

				ImGui::Checkbox("Primary", &component.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

					float perspectiveNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear))
						camera.SetPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar))
						camera.SetPerspectiveFarClip(perspectiveFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
				}
			});
	}
}