#include "pcpch.h"
#include "Scene.h"

#include "Pinecone/Renderer/Renderer2D.h"
#include "Pinecone/Scene/GameObject.h"
#include "Pinecone/Scene/Components.h"
#include "Pinecone/Scene/ScriptableGameObject.h"

namespace Pinecone
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Update scripts
		m_Registry.view<NativeScriptComponent>().each([=](auto gameObject, auto& nsc)
			{
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_GameObject = GameObject{ gameObject, this };
					nsc.Instance->m_SceneContext = this;

					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
			});

		// Get the main camera
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		auto cameras = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto go : cameras)
		{
			auto [transform, camera] = cameras.get<TransformComponent, CameraComponent>(go);

			if (camera.Primary)
			{
				mainCamera = &camera.Camera;
				cameraTransform = &transform.GetTransform();
				break;
			}
		}

		// Render the 2D scene
		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto sprites = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto e : sprites)
			{
				auto [transform, sprite] = sprites.get<TransformComponent, SpriteComponent>(e);
				Renderer2D::DrawSprite(transform.GetTransform(), sprite);
			}

			Renderer2D::EndScene();
		}
	}

	GameObject Scene::CreateGameObject(const std::string& name)
	{
		GameObject gameObject = { m_Registry.create(), this };
		UUID uuid;
		gameObject.AddComponent<IDComponent>(uuid);
		gameObject.AddComponent<TransformComponent>();
		auto& tag = gameObject.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "GameObject" : name;
		m_GameObjectMap[uuid] = gameObject;
		return gameObject;
	}

	void Scene::DestroyGameObject(GameObject gameObject)
	{
		m_Registry.destroy(gameObject);
	}

	GameObject Scene::GetGameObjectByTag(std::string_view name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto gameObject : view)
		{
			const TagComponent& tc = view.get<TagComponent>(gameObject);
			if (tc.Tag == name)
				return GameObject{ gameObject, this };
		}
		return {};
	}

	GameObject Scene::GetGameObjectByUUID(UUID uuid)
	{
		if (m_GameObjectMap.find(uuid) != m_GameObjectMap.end())
			return { m_GameObjectMap.at(uuid), this };
		return {};
	}

	std::vector<GameObject> Scene::GetGameObjectsByTag(std::string_view name)
	{
		std::vector<GameObject> objects;
		auto view = m_Registry.view<TagComponent>();
		for (auto gameObject : view)
		{
			const TagComponent& tc = view.get<TagComponent>(gameObject);
			if (tc.Tag == name)
				objects.push_back(GameObject{ gameObject, this });
		}
		return objects;
	}

	GameObject Scene::GetPrimaryCameraGameObject()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto gameObject : view)
		{
			const auto& camera = view.get<CameraComponent>(gameObject);
			if (camera.Primary)
				return GameObject{ gameObject, this };
		}
		return {};
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto go : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(go);
			cameraComponent.Camera.SetViewportSize(width, height);
		}
	}
}