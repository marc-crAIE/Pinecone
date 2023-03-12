#pragma once

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

		GameObject CreateGameObject(const std::string& name = std::string());

		void OnUpdate(Timestep ts);

		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class GameObject;
	};
}