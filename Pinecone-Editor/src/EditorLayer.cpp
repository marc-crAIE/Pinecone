#include "EditorLayer.h"

#include <Pinecone/Math/Math.h>
#include <Pinecone/Scene/SceneSerializer.h>
#include <Pinecone/Utils/PlatformUtils.h>
#include <Pinecone/Scripting/ScriptEngine.h>

#include <Pinecone/Asset/AssetManager.h>
#include <Pinecone/Asset/TextureImporter.h>
#include <Pinecone/Asset/SceneImporter.h>

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>

#include <iostream>

namespace Pinecone
{
	static Ref<Font> s_Font;

	RuntimeLayer::RuntimeLayer()
		: Layer("EditorLayer")
	{
		s_Font = Font::GetDefault();
	}

	void RuntimeLayer::OnAttach()
	{
		PC_PROFILE_FUNCTION();

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_EditorScene = CreateRef<Scene>();

		m_IconPlay = TextureImporter::LoadTexture2D("Resources/Icons/PlayButton.png");
		m_IconStop = TextureImporter::LoadTexture2D("Resources/Icons/StopButton.png");
		m_IconPause = TextureImporter::LoadTexture2D("Resources/Icons/PauseButton.png");
		m_IconStep = TextureImporter::LoadTexture2D("Resources/Icons/StepButton.png");

		auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
		if (commandLineArgs.Count > 1)
		{
			auto projectFilePath = commandLineArgs[1];
			OpenProject(projectFilePath);
		}
		else
		{
			// TODO: Prompt the user with the project selection and creation window

			// If no project is opened, close the editor
			// NOTE: this is while we don't have a new project path
			if (!OpenProject())
				Application::Get().Close();
		}

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_EditorScene);
		m_PropertiesPanel.SetSceneContext(m_EditorScene);

		m_ActiveScene = m_EditorScene;

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	}

	void RuntimeLayer::OnDetach()
	{
		PC_PROFILE_FUNCTION();
	}

	void RuntimeLayer::OnUpdate(Timestep ts)
	{
		PC_PROFILE_FUNCTION();

		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

		// Resize
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		// Render
		Renderer2D::ResetStats();
		m_Framebuffer->Bind();
		RenderCommand::Clear();

		// Clear our entity ID attachment to -1
		m_Framebuffer->ClearAttachment(1, -1);

		// Update sceneswitch (m_SceneState)
		switch (m_SceneState)
		{
		case SceneState::Edit:
		{
			m_EditorCamera.OnUpdate(ts);

			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
			break;
		}
		case SceneState::Play:
		{
			m_ActiveScene->OnUpdateRuntime(ts);
			break;
		}
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredGameObject = pixelData == -1 ? GameObject() : GameObject((entt::entity)pixelData, m_ActiveScene.get());
		}

		OnOverlayRender();

		m_Framebuffer->Unbind();
	}

	void RuntimeLayer::OnImGuiRender()
	{
		PC_PROFILE_FUNCTION();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Editor Dockspace", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 270.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("EditorDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
					OpenProject();

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
					SaveScene();

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Build"))
					BuildProject();

				if (ImGui::MenuItem("Build and Run", "", false, false))
				{

				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit")) 
					Application::Get().Close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Viewport"))
					m_ViewportOpen = true;

				if (ImGui::MenuItem("Scene Hierarchy"))
					m_SceneHierarchyPanel.Open();

				if (ImGui::MenuItem("Properties"))
					m_PropertiesPanel.Open();

				if (ImGui::MenuItem("Content Browser", "", false, m_ContentBrowserPanel ? true : false) && m_ContentBrowserPanel)
					m_ContentBrowserPanel->Open();

				if (ImGui::MenuItem("Stats"))
					m_StatsOpen = true;
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Script"))
			{
				if (ImGui::MenuItem("Reload Assembly", "Ctrl+R"))
					ScriptEngine::ReloadAssembly();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();
		m_PropertiesPanel.OnImGuiRender(m_SceneHierarchyPanel.GetSelectedGameObject());
		if (m_ContentBrowserPanel) m_ContentBrowserPanel->OnImGuiRender();

		// Stats
		if (m_StatsOpen)
		{
			ImGui::Begin("Stats", &m_StatsOpen);

			std::string name = "None";
			if (m_HoveredGameObject)
				name = m_HoveredGameObject.GetComponent<TagComponent>().Tag;
			ImGui::Text("Hovered Entity: %s", name.c_str());

			auto stats = Renderer2D::GetStats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			ImGui::End();
		}

		// The Viewport
		if (m_ViewportOpen)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Viewport", &m_ViewportOpen);
			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			auto viewportOffset = ImGui::GetWindowPos();
			m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
			m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportHovered);

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
			ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					AssetHandle handle = *(AssetHandle*)payload->Data;
					AssetType type = AssetManager::GetAssetType(handle);

					switch (type)
					{
					case Pinecone::AssetType::Scene:
					{
						OpenScene(handle);
						break;
					}
					case Pinecone::AssetType::Texture2D:
					{
						if (m_HoveredGameObject && m_HoveredGameObject.HasComponent<SpriteComponent>())
						{
							m_HoveredGameObject.GetComponent<SpriteComponent>().Texture = handle;
						}
						break;
					}
					}
				}
				ImGui::EndDragDropTarget();
			}

			// Gizmos
			GameObject selectedEntity = m_SceneHierarchyPanel.GetSelectedGameObject();
			if (selectedEntity && m_GizmoType != -1 && m_SceneState != SceneState::Play)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

				// Camera

				// Runtime camera from entity
				// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
				// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
				// const glm::mat4& cameraProjection = camera.GetProjection();
				// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

				// Editor camera
				const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
				glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

				// Entity transform
				auto& tc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = tc.GetTransform();

				// Snapping
				bool snap = Input::IsKeyPressed(Key::LeftControl);
				float snapValue = 0.5f; // Snap to 0.5m for translation/scale
				// Snap to 45 degrees for rotation
				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				float snapValues[3] = { snapValue, snapValue, snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
					nullptr, snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing())
				{
					glm::vec3 translation, rotation, scale;
					Math::DecomposeTransform(transform, translation, rotation, scale);

					glm::vec3 deltaRotation = rotation - tc.Rotation;
					tc.Translation = translation;
					tc.Rotation += deltaRotation;
					tc.Scale = scale;
				}
			}

			ImGui::End();
			ImGui::PopStyleVar();
		}

		UIToolbar();

		ImGui::End();
	}

	void RuntimeLayer::OnEvent(Event& e)
	{
		if (m_SceneState != SceneState::Play)
		{
			m_EditorCamera.OnEvent(e);
		}

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(PC_BIND_EVENT_FN(RuntimeLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(PC_BIND_EVENT_FN(RuntimeLayer::OnMouseButtonPressed));
	}

	bool RuntimeLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.IsRepeat())
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
		case Key::N:
		{
			if (control)
				NewScene();

			break;
		}
		case Key::O:
		{
			if (control)
				OpenProject();

			break;
		}
		case Key::S:
		{
			if (control)
			{
				if (shift)
					SaveSceneAs();
				else
					SaveScene();
			}

			break;
		}

		// Scene Commands
		case Key::D:
		{
			if (control)
				OnDuplicateGameObject();

			break;
		}

		case Key::F5:
		{
			// Play and stop function key
			if (m_SceneState == SceneState::Edit)
				OnScenePlay();
			else if (m_SceneState == SceneState::Play)
				OnSceneStop();
			break;
		}

		// Gizmos
		case Key::Q:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = -1;
			break;
		}
		case Key::W:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case Key::E:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case Key::R:
		{
			if (control)
			{
				ScriptEngine::ReloadAssembly();
			}
			else
			{
				if (!ImGuizmo::IsUsing())
					m_GizmoType = ImGuizmo::OPERATION::SCALE;
			}
			break;
		}
		case Key::Delete:
		{
			if (Application::Get().GetImGuiLayer()->GetActiveWidgetID() == 0)
			{
				GameObject selectedGameObject = m_SceneHierarchyPanel.GetSelectedGameObject();
				if (selectedGameObject)
				{
					m_SceneHierarchyPanel.SetSelectedGameObject({});
					m_ActiveScene->DestroyGameObject(selectedGameObject);
				}
			}
			break;
		}
		}

		return false;
	}

	bool RuntimeLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				m_SceneHierarchyPanel.SetSelectedGameObject(m_HoveredGameObject);
		}
		return false;
	}

	bool RuntimeLayer::OnWindowDrop(WindowDropEvent& e)
	{
		//AssetManager::ImportAsset();

		return true;
	}

	void RuntimeLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;

		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_PropertiesPanel.SetSceneContext(m_ActiveScene);
	}

	void RuntimeLayer::OnSceneStop()
	{
		PC_CORE_ASSERT(m_SceneState == SceneState::Play);

		if (m_SceneState == SceneState::Play)
			m_ActiveScene->OnRuntimeStop();

		m_SceneState = SceneState::Edit;

		m_ActiveScene = m_EditorScene;

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_PropertiesPanel.SetSceneContext(m_ActiveScene);
	}

	void RuntimeLayer::OnScenePause()
	{
		if (m_SceneState == SceneState::Edit)
			return;

		m_ActiveScene->SetPaused(true);
	}

	void RuntimeLayer::OnDuplicateGameObject()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		GameObject selectedGameObject = m_SceneHierarchyPanel.GetSelectedGameObject();
		if (selectedGameObject)
		{
			GameObject newGameObject = m_EditorScene->DuplicateGameObject(selectedGameObject);
			m_SceneHierarchyPanel.SetSelectedGameObject(newGameObject);
		}
	}

	void RuntimeLayer::OnOverlayRender()
	{
		if (m_SceneState == SceneState::Play)
		{
			GameObject camera = m_ActiveScene->GetPrimaryCameraGameObject();
			if (!camera)
				return;
			Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
		}
		else
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

		GameObject selectedGO = m_SceneHierarchyPanel.GetSelectedGameObject();
		if (selectedGO && m_SceneState != SceneState::Play)
		{
			if (selectedGO.HasComponent<SpriteComponent>())
			{
				TransformComponent tc = selectedGO.GetComponent<TransformComponent>();
				Renderer2D::DrawRect(tc.GetTransform(), glm::vec4(1, 0.8, 0.2, 1));
			}
		}

		Renderer2D::EndScene();
	}

	void RuntimeLayer::NewProject()
	{
		Project::New();
	}

	bool RuntimeLayer::OpenProject()
	{
		std::string filepath = FileDialogs::OpenFile("Pinecone Project (*.pcproj)\0*.pcproj\0");
		if (filepath.empty())
			return false;

		OpenProject(filepath);
		return true;
	}

	void RuntimeLayer::OpenProject(const std::filesystem::path& path)
	{
		if (Project::Load(path))
		{
			ScriptEngine::Init();

			AssetHandle startScene = Project::GetActive()->GetConfig().StartScene;
			if (startScene)
				OpenScene(startScene);
			m_ContentBrowserPanel = CreateScope<ContentBrowserPanel>(Project::GetActive());
		}
	}

	void RuntimeLayer::SaveProject()
	{
		// Project::SaveActive();
	}

	std::filesystem::path& RuntimeLayer::BuildProject()
	{
		std::string filepath = FileDialogs::SaveFile("Pinecone Project (*.pcproj)\0*.pcproj\0");
		auto basePath = std::filesystem::path(filepath).parent_path();

		Project::SaveActive(basePath / "App.pcproj", false);
		// Make sure that the asset registry has been saved
		Project::GetActive()->GetEditorAssetManager()->SerializeAssetRegistry();

		auto copyOptions = std::filesystem::copy_options::recursive | std::filesystem::copy_options::update_existing;
		// Copy mono files
		std::filesystem::copy("mono", basePath / "mono", copyOptions);
		// Copy core assets (TEMPORARY)
		std::filesystem::copy("assets", basePath / "Assets", copyOptions);
		// Copy application assets
		auto assetsDir = Project::GetActiveProjectDirectory() / Project::GetActiveAssetDirectory();
		std::filesystem::copy(assetsDir, basePath / "Assets", copyOptions);

		// Copy application scripts
		auto scriptCorePath = Project::GetActiveProjectDirectory() / Project::GetActive()->GetConfig().ScriptCorePath;
		std::filesystem::create_directories((basePath / Project::GetActive()->GetConfig().ScriptCorePath).parent_path());
		std::filesystem::copy(scriptCorePath, basePath / Project::GetActive()->GetConfig().ScriptCorePath, std::filesystem::copy_options::update_existing);

		auto scriptModulePath = Project::GetActiveProjectDirectory() / Project::GetActive()->GetConfig().ScriptModulePath;
		std::filesystem::create_directories((basePath / Project::GetActive()->GetConfig().ScriptModulePath).parent_path());
		std::filesystem::copy(scriptModulePath, basePath / Project::GetActive()->GetConfig().ScriptModulePath, std::filesystem::copy_options::update_existing);

		// Copy the runtime application
		std::filesystem::copy("Resources/Runtime/Pinecone-Runtime.exe", basePath / (Project::GetActive()->GetConfig().Name + ".exe"), std::filesystem::copy_options::update_existing);

		// Return the path the project was built to
		return basePath;
	}

	void RuntimeLayer::NewScene()
	{
		m_EditorScene = CreateRef<Scene>();
		m_SceneHierarchyPanel.SetContext(m_EditorScene);
		m_PropertiesPanel.SetSceneContext(m_EditorScene);

		m_ActiveScene = m_EditorScene;
	}

	void RuntimeLayer::OpenScene()
	{
		//OnSceneStop();
		//std::string filepath = FileDialogs::SaveFile("Pinecone Scene (*.pscene)\0*.pscene\0");
		//if (!filepath.empty())
		//	OpenScene(filepath);
	}

	void RuntimeLayer::OpenScene(AssetHandle handle)
	{
		PC_CORE_ASSERT(handle);

		if (m_SceneState != SceneState::Edit)
			OnSceneStop();

		Ref<Scene> readOnlyScene = AssetManager::GetAsset<Scene>(handle);
		Ref<Scene> newScene = Scene::Copy(readOnlyScene);

		m_EditorScene = newScene;
		m_SceneHierarchyPanel.SetContext(m_EditorScene);

		m_ActiveScene = m_EditorScene;
		m_EditorScenePath = Project::GetActive()->GetEditorAssetManager()->GetFilePath(handle);
	}

	void RuntimeLayer::SaveScene()
	{
		if (m_EditorScenePath.empty())
			SaveSceneAs();
		else
			SerializeScene(m_ActiveScene, m_EditorScenePath);
	}

	void RuntimeLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Pinecone Scene (*.pscene)\0*.pscene\0");
		if (!filepath.empty())
		{
			SerializeScene(m_ActiveScene, filepath);
			m_EditorScenePath = filepath;
		}
	}

	void RuntimeLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneImporter::SaveScene(scene, path);
	}

	void RuntimeLayer::UIToolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		bool toolbarEnabled = (bool)m_ActiveScene;

		ImVec4 tintColor = ImVec4(1, 1, 1, 1);
		if (!toolbarEnabled)
			tintColor.w = 0.5f;

		float size = ImGui::GetWindowHeight() - 2.5f;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		bool hasPlayButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play;
		bool hasPauseButton = m_SceneState != SceneState::Edit;

		if (hasPlayButton)
		{
			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit) ? m_IconPlay : m_IconStop;
			//ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - ((m_SceneState != SceneState::Edit) ? 0 : (size * 0.5f)));
			if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit)
					OnScenePlay();
				else if (m_SceneState == SceneState::Play)
					OnSceneStop();
			}
		}
		if (hasPauseButton)
		{
			bool isPaused = m_ActiveScene->IsPaused();
			ImGui::SameLine();
			{
				Ref<Texture2D> icon = m_IconPause;
				if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
				{
					m_ActiveScene->SetPaused(!isPaused);
				}
			}

			// Step button
			if (isPaused)
			{
				ImGui::SameLine();
				{
					Ref<Texture2D> icon = m_IconStep;
					bool isPaused = m_ActiveScene->IsPaused();
					if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
					{
						m_ActiveScene->Step();
					}
				}
			}
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}
}