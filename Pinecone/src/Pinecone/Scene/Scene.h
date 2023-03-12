#pragma once

#include "Pinecone/Core/Timestep.h"

#include <entt.hpp>

namespace Pinecone
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateGameObject();

		// Temporary
		entt::registry& GetRegistry() { return m_Registry; }

		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;
	};
}