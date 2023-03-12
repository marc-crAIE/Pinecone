#include "pcpch.h"
#include "Scene.h"

#include "Pinecone/Renderer/Renderer2D.h"
#include "Pinecone/Scene/GameObject.h"
#include "Pinecone/Scene/Components.h"

namespace Pinecone
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	GameObject Scene::CreateGameObject(const std::string& name)
	{
		GameObject gameObject = { m_Registry.create(), this };
		gameObject.AddComponent<TransformComponent>();
		auto& tag = gameObject.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "GameObject" : name;
		return gameObject;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Get the main camera
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		auto cameras = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : cameras)
		{
			auto& [transform, camera] = cameras.get<TransformComponent, CameraComponent>(entity);

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
				auto& [transform, sprite] = sprites.get<TransformComponent, SpriteComponent>(e);
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			cameraComponent.Camera.SetViewportSize(width, height);
		}
	}
}