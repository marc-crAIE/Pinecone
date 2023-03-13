#pragma once

#include "Pinecone/Core/UUID.h"
#include "Pinecone/Core/Timestep.h"

#include <entt.hpp>

namespace Pinecone
{
	class GameObject;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep ts);

		GameObject CreateGameObject(const std::string& name = std::string());
		void DestroyGameObject(GameObject gameObject);

		GameObject GetGameObjectByTag(std::string_view name);
		GameObject GetGameObjectByUUID(UUID uuid);

		std::vector<GameObject> GetGameObjectsByTag(std::string_view name);

		GameObject GetPrimaryCameraGameObject();

		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		entt::registry m_Registry;
		std::unordered_map<UUID, entt::entity> m_GameObjectMap;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class GameObject;
	};
}