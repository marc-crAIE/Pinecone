#include "RuntimeLayer.h"

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

namespace Pinecone
{
	static Ref<Font> s_Font;

	RuntimeLayer::RuntimeLayer(std::string_view projectPath)
		: Layer("RuntimeLayer"), m_ProjectPath(projectPath)
	{
		s_Font = Font::GetDefault();
	}

	void RuntimeLayer::OnAttach()
	{
		PC_PROFILE_FUNCTION();

		m_RuntimeScene = CreateRef<Scene>();

		auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
		if (!m_ProjectPath.empty())
		{
			OpenProject(m_ProjectPath);
		}
		else
		{
#ifdef PC_DEBUG
			if (!OpenProject())
#endif
			Application::Get().Close();
		}

		// Set the window title
		Application::Get().GetWindow().SetTitle(Project::GetActive()->GetConfig().Name);

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		m_RuntimeScene->OnViewportResize(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });

		OnScenePlay();
	}

	void RuntimeLayer::OnDetach()
	{
		PC_PROFILE_FUNCTION();

		//OnSceneStop();
	}

	void RuntimeLayer::OnUpdate(Timestep ts)
	{
		PC_PROFILE_FUNCTION();

		// Render
		Renderer2D::ResetStats();
		RenderCommand::Clear();

		// Update sceneswitch (m_SceneState)
		switch (m_SceneState)
		{
		case SceneState::Edit:
		{
			m_EditorCamera.OnUpdate(ts);

			m_RuntimeScene->OnUpdateEditor(ts, m_EditorCamera);
			break;
		}
		case SceneState::Play:
		{
			m_RuntimeScene->OnUpdateRuntime(ts);
			break;
		}
		}
	}

	void RuntimeLayer::OnImGuiRender()
	{
		PC_PROFILE_FUNCTION();
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
		dispatcher.Dispatch<WindowResizeEvent>(PC_BIND_EVENT_FN(RuntimeLayer::OnWindowResized));
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
		case Key::F5:
		{
			// Play and stop function key
			if (m_SceneState == SceneState::Edit)
				OnScenePlay();
			else if (m_SceneState == SceneState::Play)
				OnSceneStop();
			break;
		}
		}

		return false;
	}

	bool RuntimeLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		return false;
	}

	bool RuntimeLayer::OnWindowDrop(WindowDropEvent& e)
	{
		return false;
	}

	bool RuntimeLayer::OnWindowResized(WindowResizeEvent& e)
	{
		m_RuntimeScene->OnViewportResize(e.GetWidth(), e.GetHeight());
		m_EditorCamera.SetViewportSize(e.GetWidth(), e.GetHeight());
		return false;
	}

	void RuntimeLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;

		m_RuntimeScene->OnRuntimeStart();
	}

	void RuntimeLayer::OnSceneStop()
	{
		PC_CORE_ASSERT(m_SceneState == SceneState::Play);

		if (m_SceneState == SceneState::Play)
			m_RuntimeScene->OnRuntimeStop();

		m_SceneState = SceneState::Edit;
	}

	void RuntimeLayer::OnScenePause()
	{
		if (m_SceneState == SceneState::Edit)
			return;

		m_RuntimeScene->SetPaused(true);
	}

	void RuntimeLayer::OnDuplicateGameObject()
	{
		if (m_SceneState != SceneState::Edit)
			return;
	}

	void RuntimeLayer::OnOverlayRender()
	{
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
		}
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

		m_RuntimeScene = newScene;
	}
}