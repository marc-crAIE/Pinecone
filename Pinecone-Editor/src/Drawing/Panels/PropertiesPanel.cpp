#include "PropertiesPanel.h"

#include <Pinecone/Scene/Components.h>
#include <Pinecone/Scripting/ScriptEngine.h>
#include <Pinecone/ImGui/ImGuiUI.h>
#include <Pinecone/ImGui/UICore.h>

#include <Pinecone/Asset/AssetManager.h>
#include <Pinecone/Asset/AssetMetadata.h>
#include <Pinecone/Asset/TextureImporter.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/misc/cpp/imgui_stdlib.h>
#include <glm/gtc/type_ptr.hpp>

#include "../UIWidgets.h"

namespace Pinecone
{
	PropertiesPanel::PropertiesPanel()
	{
		m_TransformIcon = TextureImporter::LoadTexture2D("Resources/Icons/Panels/Properties/Components/TransformIcon.png");
		m_SpriteRendererIcon = TextureImporter::LoadTexture2D("Resources/Icons/Panels/Properties/Components/SpriteRendererIcon.png");
		m_CircleRendererIcon = TextureImporter::LoadTexture2D("Resources/Icons/Panels/Properties/Components/CircleRendererIcon.png");
		m_CameraIcon = TextureImporter::LoadTexture2D("Resources/Icons/Panels/Properties/Components/CameraIcon.png");
		m_Rigidbody2DIcon = TextureImporter::LoadTexture2D("Resources/Icons/Panels/Properties/Components/Rigidbody2DIcon.png");
		m_BoxCollider2DIcon = TextureImporter::LoadTexture2D("Resources/Icons/Panels/Properties/Components/BoxCollider2DIcon.png");
		m_CircleCollider2DIcon = TextureImporter::LoadTexture2D("Resources/Icons/Panels/Properties/Components/CircleCollider2DIcon.png");
	}

	PropertiesPanel::PropertiesPanel(const Ref<Scene>& sceneContext)
	{
		SetSceneContext(sceneContext);
	}

	void PropertiesPanel::SetSceneContext(const Ref<Scene>& sceneContext)
	{
		m_SceneContext = sceneContext;
	}

	void PropertiesPanel::SetSelection(const SelectionContext& context, const UUID& selectionID)
	{
		m_Selection.Context = context;
		m_Selection.ID = selectionID;
	}

	void PropertiesPanel::OnImGuiRender()
	{
		if (m_Open)
		{
			ImGui::Begin("Properties", &m_Open);

			if (m_Selection.Context == SelectionContext::Scene)
			{
				GameObject& go = m_SceneContext->GetGameObjectByUUID(m_Selection.ID);
				if (go)
					DrawComponents(go);
			}
			else if (m_Selection.Context == SelectionContext::ContentBrowser)
			{
				AssetHandle handle = m_Selection.ID;
				if (AssetManager::IsAssetHandleValid(handle))
					DrawAssetEditor(handle);
			}

			ImGui::End();
		}
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, GameObject gameObject, ImTextureID texture, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (gameObject.HasComponent<T>())
		{
			auto& component = gameObject.GetComponent<T>();
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
				gameObject.RemoveComponent<T>();
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
			strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
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
			DisplayAddComponentEntry<ScriptComponent>(gameObject, "Script");
			DisplayAddComponentEntry<TextComponent>(gameObject, "Text Component");

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

				std::string label = "None";
				bool isTextureValid = false;
				if (component.Texture != 0)
				{
					if (AssetManager::IsAssetHandleValid(component.Texture)
						&& AssetManager::GetAssetType(component.Texture) == AssetType::Texture2D)
					{
						const AssetMetadata& metadata = Project::GetActive()->GetEditorAssetManager()->GetMetadata(component.Texture);
						label = metadata.FilePath.filename().string();
						isTextureValid = true;
					}
					else
					{
						label = "Invalid";
					}
				}

				ImVec2 buttonLabelSize = ImGui::CalcTextSize(label.c_str());
				buttonLabelSize.x += 20.0f;
				float buttonLabelWidth = glm::max<float>(100.0f, buttonLabelSize.x);

				ImGui::Button(label.c_str(), ImVec2(buttonLabelWidth, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						AssetHandle handle = *(AssetHandle*)payload->Data;
						if (AssetManager::GetAssetType(handle) == AssetType::Texture2D)
						{
							component.Texture = handle;
						}
						else
						{
							PC_CORE_WARN("Wrong asset type!");
						}
					}
					ImGui::EndDragDropTarget();
				}

				if (isTextureValid)
				{
					ImGui::SameLine();
					ImVec2 xLabelSize = ImGui::CalcTextSize("X");
					float buttonSize = xLabelSize.y + ImGui::GetStyle().FramePadding.y * 2.0f;
					if (ImGui::Button("X", ImVec2(buttonSize, buttonSize)))
					{
						component.Texture = 0;
					}
				}

				ImGui::SameLine();
				ImGui::Text("Texture");

				ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
			});

		DrawComponent<TextComponent>("Text Renderer", gameObject, (ImTextureID)m_SpriteRendererIcon->GetRendererID(), [](auto& component)
			{
				UI::BeginPropertyGrid();

				UI::PropertyMultiline("Text String", component.TextString);
				UI::PropertyColor("Color", component.Color);
				UI::Property("Kerning", component.Kerning, 0.025f);
				UI::Property("Line Spacing", component.LineSpacing, 0.025f);

				UI::EndPropertyGrid();
			});

		DrawComponent<CameraComponent>("Camera", gameObject, (ImTextureID)m_CameraIcon->GetRendererID(), [](auto& component)
			{
				UI::BeginPropertyGrid();

				auto& camera = component.Camera;

				UI::Property("Primary", component.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				int currentProj = (int)camera.GetProjectionType();
				if (UI::PropertyDropdown("Projection", projectionTypeStrings, 2, &currentProj))
				{					
					camera.SetProjectionType((SceneCamera::ProjectionType)currentProj);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (UI::Property("Vertical FOV", perspectiveVerticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

					float perspectiveNear = camera.GetPerspectiveNearClip();
					if (UI::Property("Near", perspectiveNear))
						camera.SetPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = camera.GetPerspectiveFarClip();
					if (UI::Property("Far", perspectiveFar))
						camera.SetPerspectiveFarClip(perspectiveFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (UI::Property("Size", orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (UI::Property("Near", orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (UI::Property("Far", orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					UI::Property("Fixed Aspect Ratio", component.FixedAspectRatio);
				}

				UI::EndPropertyGrid();
			});

		DrawComponent<ScriptComponent>("Script", gameObject, (ImTextureID)m_SpriteRendererIcon->GetRendererID(), [gameObject, scene = m_SceneContext](auto& component) mutable
			{
				bool scriptClassExists = ScriptEngine::GameObjectClassExists(component.ClassName);

				static char buffer[64];
				strcpy_s(buffer, sizeof(buffer), component.ClassName.c_str());

				UI::ScopedStyleColor textColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f), !scriptClassExists);

				if (ImGui::InputText("Class", buffer, sizeof(buffer)))
				{
					component.ClassName = buffer;
					return;
				}

				// Fields
				bool sceneRunning = scene->IsRunning();
				if (sceneRunning)
				{
					Ref<ScriptInstance> scriptInstance = ScriptEngine::GetGameObjectScriptInstance(gameObject.GetUUID());
					if (scriptInstance)
					{
						const auto& fields = scriptInstance->GetScriptClass()->GetFields();
						for (const auto& [name, field] : fields)
						{
							if (field.Type == ScriptFieldType::Float || field.Type == ScriptFieldType::Double)
							{
								float data = scriptInstance->GetFieldValue<float>(name);
								if (ImGui::DragFloat(name.c_str(), &data))
								{
									scriptInstance->SetFieldValue(name, data);
								}
							}
							else if (field.Type == ScriptFieldType::Vector3)
							{
								glm::vec3 vec3 = scriptInstance->GetFieldValue<glm::vec3>(name);
								if (UI::DrawVec3Control(name, vec3))
								{
									scriptInstance->SetFieldValue(name, vec3);
								}
							}
						}
					}
				}
				else
				{
					if (scriptClassExists)
					{
						Ref<ScriptClass> gameObjectClass = ScriptEngine::GetGameObjectClass(component.ClassName);
						const auto& fields = gameObjectClass->GetFields();

						auto& gameObjectFields = ScriptEngine::GetScriptFieldMap(gameObject);
						for (const auto& [name, field] : fields)
						{
							// Field has been set in editor
							if (gameObjectFields.find(name) != gameObjectFields.end())
							{
								ScriptFieldInstance& scriptField = gameObjectFields.at(name);

								// Display control to set it maybe
								if (field.Type == ScriptFieldType::Float || field.Type == ScriptFieldType::Double)
								{
									float data = scriptField.GetValue<float>();
									if (ImGui::DragFloat(name.c_str(), &data))
										scriptField.SetValue(data);
								}
								else if (field.Type == ScriptFieldType::Vector3)
								{
									glm::vec3 vec3 = scriptField.GetValue<glm::vec3>();
									if (UI::DrawVec3Control(name, vec3))
									{
										scriptField.SetValue(vec3);
									}
								}
							}
							else
							{
								// Display control to set it maybe
								if (field.Type == ScriptFieldType::Float)
								{
									float data = 0.0f;
									if (ImGui::DragFloat(name.c_str(), &data))
									{
										ScriptFieldInstance& fieldInstance = gameObjectFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
								}
								else if (field.Type == ScriptFieldType::Vector3)
								{
									glm::vec3 vec3 = glm::vec3{ 0.0f, 0.0f, 0.0f };
									if (UI::DrawVec3Control(name, vec3, 75.0f))
									{
										ScriptFieldInstance& fieldInstance = gameObjectFields[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(vec3);
									}
								}
							}
						}
					}
				}
			});
	}

	void PropertiesPanel::DrawAssetEditor(AssetHandle handle)
	{
		AssetType type = AssetManager::GetAssetType(handle);

		switch (type)
		{
		case AssetType::Texture2D:
		{
			Ref<Texture2D> texture = AssetManager::GetAsset<Texture2D>(handle);
			uint32_t textureWidth = texture->GetWidth();
			uint32_t textureHeight = texture->GetHeight();

			float imageSize = ImGui::GetWindowWidth() - 10;
			imageSize = glm::min(imageSize, 500.0f);

			ImGui::Image((ImTextureID)texture->GetRendererID(), { imageSize, imageSize }, { 0, 1 }, { 1, 0 });

			UI::BeginPropertyGrid();
			UI::BeginDisabled();
			UI::Property("Width", textureWidth);
			UI::Property("Height", textureHeight);
			UI::EndDisabled();
			UI::EndPropertyGrid();
			break;
		}
		}
	}
}