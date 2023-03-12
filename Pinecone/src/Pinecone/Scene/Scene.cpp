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
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
		for (auto e : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteComponent>(e);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
}