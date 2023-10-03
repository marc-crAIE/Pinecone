#include "pcpch.h"
#include "Scene.h"

#include "Pinecone/Renderer/Renderer2D.h"
#include "Pinecone/Scripting/ScriptEngine.h"

#include "Pinecone/Scene/GameObject.h"
#include "Pinecone/Scene/Components.h"
#include "Pinecone/Scene/ScriptableGameObject.h"

namespace Pinecone
{
	static std::unordered_map<UUID, Scene*> s_ActiveScenes;

	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
			{
				auto view = src.view<Component>();
				for (auto srcGameObject : view)
				{
					entt::entity dstGameObject = enttMap.at(src.get<IDComponent>(srcGameObject));

					auto& srcComponent = src.get<Component>(srcGameObject);
					dst.emplace_or_replace<Component>(dstGameObject, srcComponent);
				}
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void CopyComponentIfExists(GameObject dst, GameObject src)
	{
		([&]()
			{
				if (src.HasComponent<Component>())
					dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, GameObject dst, GameObject src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}

	Scene::Scene(const std::string& name)
		: m_Name(name)
	{
		// Create Scene gameObject
		m_SceneEntity = m_Registry.create();
		m_Registry.emplace<SceneComponent>(m_SceneEntity, m_SceneID);
	}

	void Scene::OnRuntimeStart()
	{
		m_Running = true;

		s_ActiveScenes[m_SceneID] = this;

		// Scripting
		{
			ScriptEngine::OnRuntimeStart(this);
			// Instantiate all script entities

			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				GameObject gameObject = { e, this };
				ScriptEngine::OnCreateGameObject(gameObject);
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
		m_Running = false;

		s_ActiveScenes.erase(m_SceneID);

		// Stop the script engine
		ScriptEngine::OnRuntimeStop();
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		if (!m_Paused || m_StepFrames-- > 0)
		{
			// Update scripts
			{
				// C# GameObject OnUpdate
				auto view = m_Registry.view<ScriptComponent>();
				for (auto e : view)
				{
					GameObject gameObject = { e, this };
					ScriptEngine::OnUpdateGameObject(gameObject, ts);
				}

				m_Registry.view<NativeScriptComponent>().each([=](auto gameObject, auto& nsc)
					{
						// Check to make sure if the script component is not properly instantiated yet
						if (!nsc.Instantiated)
						{
							// If it is in fact not, set the game object to the one it is attached to and the scene in which
							// the game object exists in
							nsc.Instance->m_GameObject = GameObject{ gameObject, this };
							nsc.Instance->m_SceneContext = this;
							nsc.Instantiated = true;

							// Call the scripts OnCreate function
							nsc.Instance->OnCreate();
						}

						// Call the scripts OnUpdate function
						nsc.Instance->OnUpdate(ts);
					});
			}
		}

		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto gameObject : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(gameObject);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			// Draw sprites
			{
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
				for (auto gameObject : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(gameObject);

					Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)gameObject);
				}
			}

			// Draw text
			{
				auto view = m_Registry.view<TransformComponent, TextComponent>();
				for (auto gameObject : view)
				{
					auto [transform, text] = view.get<TransformComponent, TextComponent>(gameObject);

					Renderer2D::DrawString(text.TextString, transform.GetTransform(), text, (int)gameObject);
				}
			}

			Renderer2D::EndScene();
		}

	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		RenderScene(camera);
	}

	GameObject Scene::CreateGameObject(const std::string& name)
	{
		// Create a new game object with a new entity handle
		GameObject gameObject = { m_Registry.create(), this };
		// Generate a new random UUID
		UUID uuid;
		// Add the UUID and transform component
		gameObject.AddComponent<IDComponent>(uuid);
		gameObject.AddComponent<TransformComponent>();
		// Add the tag component and set it to be either "GameObject" or the specified name
		auto& tag = gameObject.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "GameObject" : name;
		// Add the game object to our map. This is used to easily search for game objects
		// by their UUID
		m_GameObjectMap[uuid] = gameObject;
		// Return the newly created game object
		return gameObject;
	}

	GameObject Scene::CreateGameObjectWithUUID(UUID uuid, const std::string& name)
	{
		GameObject gameObject = { m_Registry.create(), this };
		gameObject.AddComponent<IDComponent>(uuid);
		gameObject.AddComponent<TransformComponent>();
		auto& tag = gameObject.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "GameObject" : name;
		m_GameObjectMap[uuid] = gameObject;
		return gameObject;
	}

	void Scene::DestroyGameObject(GameObject gameObject)
	{
		// Check to make sure if the game object has a native script component
		if (gameObject.HasComponent<NativeScriptComponent>())
		{
			// Get the native script component
			auto nsc = gameObject.GetComponent<NativeScriptComponent>();
			// Call the OnDestroy function
			nsc.Instance->OnDestroy();
			// Then delete it from memory
			delete nsc.Instance;
			nsc.Instance = nullptr;
		}
		// Remove the game object from our game objects map and the entity registry
		m_GameObjectMap.erase(gameObject.GetUUID());
		m_Registry.destroy(gameObject);
	}

	GameObject Scene::DuplicateGameObject(GameObject gameObject)
	{
		// Copy name because we're going to modify component data structure
		std::string name = gameObject.GetName();
		GameObject newGameObject = CreateGameObject(name);
		CopyComponentIfExists(AllComponents{}, newGameObject, gameObject);
		return newGameObject;
	}

	GameObject Scene::GetGameObjectByTag(std::string_view name)
	{
		// Get all game objects with a tag component
		auto view = m_Registry.view<TagComponent>();
		for (auto gameObject : view)
		{
			// Get the tag component and return the game object if the tag matches
			const TagComponent& tc = view.get<TagComponent>(gameObject);
			if (tc.Tag == name)
				return GameObject{ gameObject, this };
		}
		return {};
	}

	GameObject Scene::GetGameObjectByUUID(UUID uuid)
	{
		// Find if there is a game object with the UUID
		if (m_GameObjectMap.find(uuid) != m_GameObjectMap.end())
			// return the game object with the UUID
			return { m_GameObjectMap.at(uuid), this };
		return {};
	}

	std::vector<GameObject> Scene::GetGameObjectsByTag(std::string_view name)
	{
		std::vector<GameObject> objects;
		// Get all game objects with a tag component
		auto view = m_Registry.view<TagComponent>();
		for (auto gameObject : view)
		{
			// Get the tag component and add the game object to our list of objects
			// if the tag matches
			const TagComponent& tc = view.get<TagComponent>(gameObject);
			if (tc.Tag == name)
				objects.push_back(GameObject{ gameObject, this });
		}
		// return all of the objects we found
		return objects;
	}

	GameObject Scene::GetPrimaryCameraGameObject()
	{
		// Get all game objects with a camera component
		auto view = m_Registry.view<CameraComponent>();
		for (auto gameObject : view)
		{

			// Get the camera component and return the game object if it is primary
			const auto& camera = view.get<CameraComponent>(gameObject);
			if (camera.Primary)
				return GameObject{ gameObject, this };
		}
		return {};
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		if (m_ViewportWidth == width && m_ViewportHeight == height)
			return;

		// Update the viewport width and height
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Make sure that the width or the height is not 0 as we cannot resize a
		// viewport with either value being 0
		if (width == 0 || height == 0)
			return;

		// Get all game objects with a cemar component
		auto view = m_Registry.view<CameraComponent>();
		for (auto go : view)
		{
			// Get the camera component and update its viewport
			auto& cameraComponent = view.get<CameraComponent>(go);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}

	void Scene::Step(int frames)
	{
		m_StepFrames = frames;
	}

	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// Create game objects in new scene
		auto idView = srcSceneRegistry.view<IDComponent>();
		for (auto e : idView)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
			const auto& name = srcSceneRegistry.get<TagComponent>(e);
			GameObject newGameObject = newScene->CreateGameObjectWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)newGameObject;
		}

		// Copy components (except IDComponent and TagComponent)
		CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
	}

	void Scene::RenderScene(EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		// Draw sprites
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto gameObject : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(gameObject);

				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)gameObject);
			}
		}

		// Draw text
		{
			auto view = m_Registry.view<TransformComponent, TextComponent>();
			for (auto gameObject : view)
			{
				auto [transform, text] = view.get<TransformComponent, TextComponent>(gameObject);

				Renderer2D::DrawString(text.TextString, transform.GetTransform(), text, (int)gameObject);
			}
		}

		Renderer2D::EndScene();
	}
}