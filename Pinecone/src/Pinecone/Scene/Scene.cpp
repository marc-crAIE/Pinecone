#include "pcpch.h"
#include "Scene.h"

#include "Pinecone/Renderer/Renderer2D.h"
#include "Pinecone/Scene/GameObject.h"
#include "Pinecone/Scene/Components.h"
#include "Pinecone/Scene/ScriptableGameObject.h"

namespace Pinecone
{
	void Scene::OnUpdate(Timestep ts)
	{
		// Update scripts
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

		// Get the main camera
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		auto cameras = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto go : cameras)
		{
			// Get the camera and its transform
			auto [transform, camera] = cameras.get<TransformComponent, CameraComponent>(go);

			// Check if it is the primary camera
			// TODO: Probably move primary camera handling to scene as we can have multiple primary
			//		 cameras. All this will do is use the one first found and ignore the others.
			//		 Meaning that when switching cameras during runtime, the client has to turn off 
			//		 the previously set primary camera aswell.
			if (camera.Primary)
			{
				// Store the SceneCamera and the game objects transform 
				mainCamera = &camera.Camera;
				cameraTransform = &transform.GetTransform();
				// Exit the for loop as we found the primary camera
				break;
			}
		}

		// Render the 2D scene
		if (mainCamera)
		{
			// Begin our 2D scene drawing with our primary camera and the transform of its game objects position
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			// Get all game objects that have a sprite and a transform component
			auto sprites = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto e : sprites)
			{
				// Get the sprite and transform components of the game object and draw it to the screen
				auto [transform, sprite] = sprites.get<TransformComponent, SpriteComponent>(e);
				Renderer2D::DrawSprite(transform.GetTransform(), sprite);
			}

			// End our 2D scene drawing
			Renderer2D::EndScene();
		}
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

	bool Scene::GameObjectExists(GameObject go)
	{
		return m_Registry.valid(go);
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
			cameraComponent.Camera.SetViewportSize(width, height);
		}
	}
}