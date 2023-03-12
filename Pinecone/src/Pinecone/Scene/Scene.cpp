#include "pcpch.h"
#include "Scene.h"

#include "Pinecone/Renderer/Renderer2D.h"
#include "Pinecone/Scene/Components.h"

namespace Pinecone
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	entt::entity Scene::CreateGameObject()
	{
		return m_Registry.create();
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto e : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(e);

			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
}